// macros.h

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#pragma once

#ifdef USBHID_EXPORTS
#define USBHID_API __declspec(dllexport)
#else
#define USBHID_API __declspec(dllimport)
#endif