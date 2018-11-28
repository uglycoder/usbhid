// rawhiddevice.cpp

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "stdafx.h"
#include "../interface/rawhiddevice.hpp"


namespace
{
  struct RawUSBPacket
  {
    char leadByte;
    USBHID_ns::RawHidDevice::usbHidPacket usbPacket;
  };

  [[nodiscard]] std::pair<USBHID_ns::RawHidDevice::COMMS_ERROR, DWORD> ReadUSBFile(::HANDLE usbFileHandle, DWORD milliSecondTimeout, DWORD numPacketsToRead, void * buffer) noexcept;

  [[nodiscard]] USBHID_ns::RawHidDevice::COMMS_ERROR WriteUSBFile(::HANDLE usbFileHandle, DWORD milliSecondTimeout, DWORD numBytesToWrite, void * buffer) noexcept;
}

USBHID_ns::RawHidDevice::RawHidDevice(hidDeviceInfo const & devInfo) : m_devInfo(devInfo)
{
}

USBHID_ns::RawHidDevice::~RawHidDevice()
{
    auto const & result{ close() };

    assert(result == COMMS_ERROR::SUCCESS);
}

bool USBHID_ns::RawHidDevice::open() noexcept
{
  m_usbHandle = ::CreateFile(m_devInfo.path.c_str(), GENERIC_READ | GENERIC_WRITE,
    FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
    OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);

  return isOpen();
}

bool USBHID_ns::RawHidDevice::isOpen() const noexcept
{
  return m_usbHandle != INVALID_HANDLE_VALUE;
}

USBHID_ns::RawHidDevice::COMMS_ERROR USBHID_ns::RawHidDevice::close() noexcept
{
  if(isOpen())
  {
    auto const & result{ ::CloseHandle(m_usbHandle) };
    m_usbHandle = INVALID_HANDLE_VALUE;
    if(result == TRUE)
    {
      return  COMMS_ERROR::SUCCESS;
    }
    else if(::GetLastError() == ERROR_INVALID_HANDLE)
    {
      return COMMS_ERROR::INVALID_USB_HANDLE;
    }
    else
    {
      return COMMS_ERROR::CLOSE_USB_HANDLE_FAIL;
    }
  }
  else
  {
    return  COMMS_ERROR::SUCCESS;
  }
}

USBHID_ns::RawHidDevice::COMMS_ERROR USBHID_ns::RawHidDevice::send(usbHidPacket const & packet, DWORD milliSecondTimeout) const noexcept
{
  assert(isOpen());

  RawUSBPacket tmpBuffer{ 0, packet };

  return WriteUSBFile(m_usbHandle, milliSecondTimeout, sizeof tmpBuffer, &tmpBuffer);
}

USBHID_ns::RawHidDevice::COMMS_ERROR USBHID_ns::RawHidDevice::receive(usbHidPacket & packet, DWORD milliSecondTimeout /*= INFINITE*/) const noexcept
{
  assert(isOpen());

  RawUSBPacket tmpBuffer;

  auto const & result{ ReadUSBFile(m_usbHandle, milliSecondTimeout, 1UL, &tmpBuffer).first };

  if(result == COMMS_ERROR::SUCCESS)
  {
    packet = tmpBuffer.usbPacket;
  }

  return result;
}

USBHID_ns::RawHidDevice::usbHidPackets_t USBHID_ns::RawHidDevice::receive(DWORD numUsbPacketsRequested, DWORD milliSecondTimeout /*= INFINITE*/) const noexcept
{
  assert(isOpen());

  try
  {
    std::vector<RawUSBPacket> tmpBuffer(numUsbPacketsRequested);

    auto const[result, numPacketsRead] = ReadUSBFile(m_usbHandle, milliSecondTimeout, numUsbPacketsRequested, tmpBuffer.data());

    std::vector<usbHidPacket> packetsReceived(numPacketsRead);
    for(DWORD n{}; n < numPacketsRead; ++n)
    {
      packetsReceived[n] = tmpBuffer[n].usbPacket;
    }

    return { packetsReceived, result };
  }
  catch(...)
  {
    return { {},COMMS_ERROR::FAIL };
  }
}

USBHID_ns::hidDeviceInfo USBHID_ns::RawHidDevice::devInfo() const noexcept
{
  return m_devInfo;
}



namespace
{
  std::pair<USBHID_ns::RawHidDevice::COMMS_ERROR, DWORD> ReadUSBFile(::HANDLE usbFileHandle, DWORD milliSecondTimeout, DWORD numPacketsToRead, void * buffer) noexcept
  {
    using COMMS_ERROR = USBHID_ns::RawHidDevice::COMMS_ERROR;

    DWORD numBytesRead{};
    auto const & numBytesToRead{ numPacketsToRead * static_cast<DWORD>(sizeof(RawUSBPacket)) };


    ::OVERLAPPED ov{ 0 };
    ov.hEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
    ::ReadFile(usbFileHandle, buffer, numBytesToRead, nullptr, &ov);
    auto const waitResult{ ::WaitForSingleObject(ov.hEvent, milliSecondTimeout) };
    auto const ovResult{ ::GetOverlappedResult(usbFileHandle, &ov, &numBytesRead, FALSE) };

    ::CloseHandle(ov.hEvent);

    bool const result{
      waitResult == WAIT_OBJECT_0 &&
      ovResult == TRUE &&
      numBytesRead == numBytesToRead };

    auto const & numPacketsRead{ numBytesRead / static_cast<DWORD>(sizeof(RawUSBPacket)) };

    if(result)
    {
      return std::make_pair(COMMS_ERROR::SUCCESS, numPacketsRead);
    }

    if(waitResult == WAIT_TIMEOUT)
    {
      ::CancelIo(usbFileHandle);
      return std::make_pair(COMMS_ERROR::TIMED_OUT, numPacketsRead);
    }

    return std::make_pair(COMMS_ERROR::FAIL, numPacketsRead);
  }

  USBHID_ns::RawHidDevice::COMMS_ERROR WriteUSBFile(::HANDLE usbFileHandle, DWORD milliSecondTimeout, DWORD numBytesToWrite, void * buffer) noexcept
  {
    using COMMS_ERROR = USBHID_ns::RawHidDevice::COMMS_ERROR;

    DWORD numBytesWritten{};

    ::OVERLAPPED ov{ 0 };
    ov.hEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
    ::WriteFile(usbFileHandle, buffer, numBytesToWrite, nullptr, &ov);
    auto const waitResult{ ::WaitForSingleObject(ov.hEvent, milliSecondTimeout) };
    auto const ovResult{ ::GetOverlappedResult(usbFileHandle, &ov, &numBytesWritten, FALSE) };

    ::CloseHandle(ov.hEvent);

    bool const result{
      waitResult == WAIT_OBJECT_0 &&
      ovResult == TRUE &&
      numBytesWritten == numBytesToWrite };

    if(result)
    {
      return COMMS_ERROR::SUCCESS;
    }

    if(waitResult == WAIT_TIMEOUT)
    {
      ::CancelIo(usbFileHandle);
      return COMMS_ERROR::TIMED_OUT;
    }

    return COMMS_ERROR::FAIL;
  }

}

