 /* Raw HID Example
   Teensy can send/receive 64 byte packets with a
   dedicated program running on a PC or Mac.

   You must select Raw HID from the "Tools > USB Type" menu

  MIT License
  See LICENSE.txt file in root of project
  Copyright(c) 2018 Simon Parmenter
*/

#define IN_START_MSG 0x01
#define IN_STOP_MSG  0x00

#define OUT_STOP_MSG 0xFF

const int ledPin = 13;
char buffer1[64];

void setup() {
  pinMode(ledPin, OUTPUT);
}

// RawHID packets are always 64 bytes
// byte buffer[64];
unsigned int packetCount = 0;

void Blink()
{  
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(100);                  // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(100);                  // wait for a second
}

void SendStop()
{
  int n;
  buffer1[2] = OUT_STOP_MSG;
  n = RawHID.send(buffer1, 100);
  if (n > 0) {
    Serial.print(F("STOP"));
  } else {
    Serial.println(F("Failed to send STOP message"));
  }
  buffer1[2] = 0;
}

void SendData()
{
  int n;
  buffer1[0] = highByte(packetCount);
  buffer1[1] = lowByte(packetCount);
  n = RawHID.send(buffer1, 100);
  if (n > 0) {
    Serial.print(F("Transmit packet "));
    Serial.println(packetCount);
    ++packetCount;
  } else {
    Serial.println(F("Unable to transmit packet"));
  }
}

void (*FuncPtr)() = Blink;

void loop() {
  buffer1[0] = 0xFF;
  
  int n;
  n = RawHID.recv(buffer1, 0); // 0 timeout = do not wait
  if (n > 0) {
    // the computer sent a message. Ignore the
    // other 63 bytes!
    if(buffer1[0] == IN_STOP_MSG)
    {
      FuncPtr = Blink;
      packetCount = 0;
      SendStop();
    }
    else if(buffer1[0] == IN_START_MSG)
    {
      FuncPtr = SendData;
    }
    buffer1[0] = 0xFF;
  }
  FuncPtr();
  delay(50);
}
