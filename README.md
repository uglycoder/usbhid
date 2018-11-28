# usbhid
usnhid is a C++17 Visual Studio 2017 Solution to interrogate a Windows platform
for any attached USB HID devices and to communicate with them.

The VS Solution comprises 2 projects:
## usbhid
This is a DLL project and is the main project component that does all the work in
interrogating the system for its attached USB HID devices. It also provides the
means to read and write to them if the devices allow it.

Note: only the public interface is documented.

## consoleTest
This is a console test application for the **usbhid** dll.
It shows how to use the **usbhid** dll.
Provided in the VS solution is a C source code file **RawHid_test1.ino** in the
**teensyCode** filter folder and directory folder of the same name.
This is the code that is run by a Teensy 3.2 miro-controller - see
 https://www.pjrc.com/store/teensy32.html.
 The **consoleTest** project and the **RawHid_test1.ino** consitute a test
 harness for the **usbhid** project.
 An example of output is in the text file **example-output.txt** in the VS solution
 folder and visible in the VS Solution Explorer under filter folder **example output**.

Note: this project is not documented.



##### Notes
Version of Visual Studio 2017 Professional: 15.9.3<br/>
[The latest Visual C++ redistributables.](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)

