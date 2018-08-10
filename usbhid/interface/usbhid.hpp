// usbhid.hpp

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <hidsdi.h>

#include "common_stl_hdrs.hpp"


#include "../interface/macros.h"

namespace USBHID_ns
{
  struct hidAttributes
  {
    std::uint16_t  VendorID;
    std::uint16_t  ProductID;
    std::uint16_t  VersionNumber;
  };

  struct hidDeviceInfo
  {
    std::wstring  path;
    std::wstring  manufacturerName;
    hidAttributes attributes;
    ::HIDP_CAPS   caps;
  };

  using installedDeviceInfoList_t = std::vector<hidDeviceInfo>;

  /// <summary>
  /// Retreives the basic USB HID information of all currently installed USB HID devices 
  /// </summary>
  /// <returns>
  /// Returns a vector of hidDeviceInfo structures.
  /// </returns>
  USBHID_API installedDeviceInfoList_t GetlInstalledDevicesInfo() noexcept;
}