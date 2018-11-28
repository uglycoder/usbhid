// rawhiddevice.hpp

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include "common_stl_hdrs.hpp"
#include "usbhid.hpp"



namespace USBHID_ns
{
  constexpr std::uint8_t USB_PACKET_SIZE{64};

  /// <summary>
  /// The RawHidDevice class represents a USB HID device through which client
  /// code can read and write USB packets of 64 byte size. It also holds basic
  /// USB HID device information.
  /// The instances of this class are non-copyable but movable. This is because
  /// on destruction the file handle used to communicate with the associated
  /// device filepath is closed and having multiple instances with the same
  /// device filepath would be difficult to keep track with regard to their
  /// object lifetimes and the state of the file handle; the use of one instance could 
  /// change the state of others with the same file handle and device file path.
  /// The ideal is to have one instance per USB HID device.
  /// </summary>
  class RawHidDevice final
  {
  public:

    enum class COMMS_ERROR
    {
      SUCCESS
      ,FAIL
      ,TIMED_OUT
      ,CLOSE_USB_HANDLE_FAIL
      ,INVALID_USB_HANDLE
    };

    struct usbHidPacket
    {
      std::uint8_t packet[USB_PACKET_SIZE];
    };

    using usbHidPacketList_t = std::vector<usbHidPacket>;

    struct usbHidPackets_t
    {
      usbHidPacketList_t packets;
      COMMS_ERROR        error;
    };

    /// <summary>
    /// Constructs an instance using the given device information in the parameter devInfo
    /// </summary>
    USBHID_API explicit RawHidDevice(hidDeviceInfo const & devInfo);

    /// <summary>
    /// Destroys an instance and closes the file handle if it is open.
    /// </summary>
    USBHID_API ~RawHidDevice();

    // Movable
    USBHID_API RawHidDevice(RawHidDevice &&) = default;
    USBHID_API RawHidDevice & operator=(RawHidDevice &&) = default;

    // Non-copyable
    RawHidDevice(RawHidDevice const &) = delete;
    RawHidDevice & operator=(RawHidDevice const &) = delete;

    /// <summary>
    /// Attempts to open a valid file handle for the associated device filepath.
    /// If a valid file handle is obtained then true is returned otherwise false.
    /// </summary>
    USBHID_API [[nodiscard]] bool open() noexcept;

    /// <summary>
    /// Returns true if there is a valid file handle open for communication other false.
    /// </summary>
    USBHID_API [[nodiscard]] bool isOpen() const noexcept;

    /// <summary>
    /// Closes the file handle if open
    /// </summary>
    /// <returns>
    /// returns an error code depending on outcome.<br>
    /// COMMS_ERROR::SUCCESS. returned on successfully closing a valid open file handle
    /// and if the file handle is not open.<br>
    /// COMMS_ERROR::INVALID_USB_HANDLE, an atttempt to close an invalid file handle.<br>
    /// COMMS_ERROR::CLOSE_USB_HANDLE_FAIL, an error occurred on closing the file handle.<br>
    /// </returns>
    USBHID_API [[nodiscard]] COMMS_ERROR close() noexcept;

    /// <summary>
    /// Sends a single USB packet to the USB HID device.
    /// </summary>
    /// <param name="packet">The USB packet of data to send.</param>
    /// <param name="milliSecondTimeout">The number of mS to block for completion of the send. The default is to block forever.</param>
    /// <returns>
    /// returns an error code depending on outcome.<br>
    /// COMMS_ERROR::SUCCESS, returned on successfully sending packet.<br>
    /// COMMS_ERROR::TIMED_OUT, the function could not complete the send within the specified timeout. <br>
    /// COMMS_ERROR::FAIL, an unknown error occurred.
    /// </returns>
    USBHID_API [[nodiscard]] COMMS_ERROR send(usbHidPacket const & packet, DWORD milliSecondTimeout = INFINITE) const noexcept;

    /// <summary>
    /// Receives a single packet of data from the USB HID device.
    /// </summary>
    /// <param name="packet">The USB packet which will receive data.</param>
    /// <param name="milliSecondTimeout">The number of mS to block for completion of the send. The default is to block forever.</param>
    /// <returns>
    /// returns an error code depending on outcome.<br>
    /// COMMS_ERROR::SUCCESS, returned on successfully receiving a packet.<br>
    /// COMMS_ERROR::TIMED_OUT, the function could not complete the receive within the specified timeout. <br>
    /// COMMS_ERROR::FAIL, an unknown error occurred.
    /// </returns>
    USBHID_API [[nodiscard]] COMMS_ERROR receive(usbHidPacket & packet, DWORD milliSecondTimeout = INFINITE) const noexcept;

    /// <summary>
    /// Receives a number of packets of data from the USB HID device.
    /// </summary>
    /// <param name="numUsbPacketsRequested">The number of requested USB packets to receive.</param>
    /// <param name="milliSecondTimeout">The number of mS to block for completion of the send. The default is to block forever.</param>
    /// <returns>
    /// returns a usbHidPackets_t structure containing a vector of usbHidPackets
    /// with an error code with possible values.<br>
    /// COMMS_ERROR::SUCCESS, returned on successfully receiving the requested number of packets.<br>
    /// COMMS_ERROR::TIMED_OUT, the function could not complete the receive within the specified timeout.<br>
    /// COMMS_ERROR::FAIL, an unknown error occurred.
    /// </returns>
    USBHID_API [[nodiscard]] usbHidPackets_t receive(DWORD numUsbPacketsRequested, DWORD milliSecondTimeout = INFINITE) const noexcept;

    /// <summary>
    /// Returns the basic USB HID data associated with the device.
    /// </summary>
    /// <returns>
    /// Returns a copy of the hidDeviceInfo held by the instance.
    /// </returns>
    USBHID_API [[nodiscard]] hidDeviceInfo devInfo() const noexcept;

  private:
    hidDeviceInfo m_devInfo;
    ::HANDLE m_usbHandle{ INVALID_HANDLE_VALUE };
  }; 
}