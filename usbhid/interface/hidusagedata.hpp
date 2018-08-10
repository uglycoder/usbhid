// hidusagedata.hpp

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#include <string>

#include "macros.h"

namespace USBHID_ns
{
  enum class HID_USAGEPAGE : std::uint16_t
  {
    UNDEFINED                       = 0x00
    , GENERIC                       = 0x01
    , SIMULATION                    = 0x02
    , VR                            = 0x03
    , SPORT                         = 0x04
    , GAME                          = 0x05
    , GENERIC_DEVICE                = 0x06
    , KEYBOARD                      = 0x07
    , LED                           = 0x08
    , BUTTON                        = 0x09
    , ORDINAL                       = 0x0A
    , TELEPHONY                     = 0x0B
    , CONSUMER                      = 0x0C
    , DIGITIZER                     = 0x0D
    , HAPTICS                       = 0x0E
    , PID                           = 0x0F
    , UNI_CODE                      = 0x10
    , ALPHANUMERIC                  = 0x14
    , SENSOR                        = 0x20
    , BARCODE_SCANNER               = 0x8C
    , WEIGHING_DEVICE               = 0x8D
    , MAGNETIC_STRIPE_READER        = 0x8E
    , CAMERA_CONTROL                = 0x90
    , ARCADE                        = 0x91
    , MICROSOFT_BLUETOOTH_HANDSFREE = 0xFFF3
    , VENDOR_DEFINED_BEGIN          = 0xFF00
    , VENDOR_DEFINED_END            = 0xFFFF
  };

  enum class HID_GENERIC_USAGE : std::uint16_t
  {
    POINTER                   = 0x01
    , MOUSE                   = 0x02
    , JOYSTICK                = 0x04
    , GAMEPAD                 = 0x05
    , KEYBOARD                = 0x06
    , KEYPAD                  = 0x07
    , MULTI_AXIS_CONTROLLER   = 0x08
    , TABLET_PC_SYSTEM_CTL    = 0x09
    , PORTABLE_DEVICE_CONTROL = 0x0D
    , INTERACTIVE_CONTROL     = 0x0E
    , COUNTED_BUFFER          = 0x3A
    , SYSTEM_CTL              = 0x80

    , X                                          = 0x30
    , Y                                          = 0x31
    , Z                                          = 0x32
    , RX                                         = 0x33
    , RY                                         = 0x34
    , RZ                                         = 0x35
    , SLIDER                                     = 0x36
    , DIAL                                       = 0x37
    , WHEEL                                      = 0x38
    , HATSWITCH                                  = 0x39
    , BYTE_COUNT                                 = 0x3B
    , MOTION_WAKEUP                              = 0x3C
    , START                                      = 0x3D
    , SELECT                                     = 0x3E
    , VX                                         = 0x40
    , VY                                         = 0x41
    , VZ                                         = 0x42
    , VBRX                                       = 0x43
    , VBRY                                       = 0x44
    , VBRZ                                       = 0x45
    , VNO                                        = 0x46
    , FEATURE_NOTIFICATION                       = 0x47
    , RESOLUTION_MULTIPLIER                      = 0x48
    , SYSCTL_POWER                               = 0x81
    , SYSCTL_SLEEP                               = 0x82
    , SYSCTL_WAKE                                = 0x83
    , SYSCTL_CONTEXT_MENU                        = 0x84
    , SYSCTL_MAIN_MENU                           = 0x85
    , SYSCTL_APP_MENU                            = 0x86
    , SYSCTL_HELP_MENU                           = 0x87
    , SYSCTL_MENU_EXIT                           = 0x88
    , SYSCTL_MENU_SELECT                         = 0x89
    , SYSCTL_MENU_RIGHT                          = 0x8A
    , SYSCTL_MENU_LEFT                           = 0x8B
    , SYSCTL_MENU_UP                             = 0x8C
    , SYSCTL_MENU_DOWN                           = 0x8D
    , SYSCTL_COLD_RESTART                        = 0x8E
    , SYSCTL_WARM_RESTART                        = 0x8F
    , DPAD_UP                                    = 0x90
    , DPAD_DOWN                                  = 0x91
    , DPAD_RIGHT                                 = 0x92
    , DPAD_LEFT                                  = 0x93
    , SYSCTL_DOCK                                = 0xA0
    , SYSCTL_UNDOCK                              = 0xA1
    , SYSCTL_SETUP                               = 0xA2
    , SYSCTL_SYS_BREAK                           = 0xA3
    , SYSCTL_SYS_DBG_BREAK                       = 0xA4
    , SYSCTL_APP_BREAK                           = 0xA5
    , SYSCTL_APP_DBG_BREAK                       = 0xA6
    , SYSCTL_MUTE                                = 0xA7
    , SYSCTL_HIBERNATE                           = 0xA8
    , SYSCTL_DISP_INVERT                         = 0xB0
    , SYSCTL_DISP_INTERNAL                       = 0xB1
    , SYSCTL_DISP_EXTERNAL                       = 0xB2
    , SYSCTL_DISP_BOTH                           = 0xB3
    , SYSCTL_DISP_DUAL                           = 0xB4
    , SYSCTL_DISP_TOGGLE                         = 0xB5
    , SYSCTL_DISP_SWAP                           = 0xB6
    , SYSCTL_DISP_AUTOSCALE                      = 0xB7
    , SYSTEM_DISPLAY_ROTATION_LOCK_BUTTON        = 0xC9
    , SYSTEM_DISPLAY_ROTATION_LOCK_SLIDER_SWITCH = 0xCA
    , CONTROL_ENABLE                             = 0xCB
  };

  /// <summary>
  /// For a given value of the HID_USAGEPAGE enumeration the function returns
  /// a string representation of it
  /// </summary>
  /// <param name="usagePage">
  /// [in] The value of the HID_USAGEPAGE enumeration
  /// </param>
  /// <returns>
  /// On success returns a wide string representation of the given 'usagePage' parameter
  /// otherwise an empty string is returned.
  /// </returns>
  USBHID_API std::wstring GetString(HID_USAGEPAGE usagePage) noexcept;

  /// <summary>
  /// For a given value of the HID_GENERIC_USAGE enumeration the function returns
  /// a string representation of it
  /// </summary>
  /// <param name="usage">
  /// [in] The value of the HID_GENERIC_USAGE enumeration
  /// </param>
  /// <returns>
  /// On success returns a wide string representation of the given 'usage' parameter
  /// otherwise an empty string is returned.
  /// </returns>
  USBHID_API std::wstring GetString(HID_GENERIC_USAGE usage) noexcept;
  
}