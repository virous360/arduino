/*
 Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS4BT.h>
#include <usbhub.h>
#include <Servo.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb);  // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
// PS4BT PS4(&Btd);


#define motorp1 2
#define motorp2 4
#define motor1speed 5
#define motor2p1 A1
#define motor2p2 A2
#define motor2speed 3
Servo myservo;

int offset = 0;
int speed;
int x;
int r2;
int pos;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial)
    ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));

  pinMode(motorp1, OUTPUT);
  pinMode(motorp2, OUTPUT);
  pinMode(motor1speed, OUTPUT);
  pinMode(motor2p1, OUTPUT);
  pinMode(motor2p2, OUTPUT);
  pinMode(motor2speed, OUTPUT);

  myservo.attach(6);
  delay(250);
  myservo.write(90);
  delay(250);
}
void loop() {
  Usb.Task();

  if (PS4.connected()) {
    PS4.setLed(Green);
    if (PS4.getButtonClick(CROSS)) {
      Serial.print(F("\r\nCross"));
      digitalWrite(motorp1, LOW);
      digitalWrite(motorp2, HIGH);
      digitalWrite(motor2p1, LOW);
      digitalWrite(motor2p2, HIGH);
    }
    if (PS4.getButtonClick(CIRCLE)) {
      Serial.print(F("\r\CIRCLE"));
      offset++;
    }
    if (PS4.getButtonClick(TRIANGLE)) {
      Serial.print(F("\r\nTriangle"));
      digitalWrite(motorp1, HIGH);
      digitalWrite(motorp2, LOW);
      digitalWrite(motor2p1, HIGH);
      digitalWrite(motor2p2, LOW);
    }
    if (PS4.getButtonClick(SQUARE)) {
      Serial.print(F("\r\SQUARE"));
      offset -= 1;
    }
    x = PS4.getAnalogHat(LeftHatX);
    r2 = PS4.getAnalogButton(R2);
    if (x <= 127 && x > 110) {  // Center Servo
      myservo.write(90 + offset);
    } else if (x <= 255 && x > 127) {  // Move Servo Right
      pos = map(x, 127, 255, 90, 130);
      myservo.write(pos);
    } else if (x < 110 && x >= 0) {  // Move Servo Left
      pos = map(x, 110, 0, 90, 50);
      myservo.write(pos);
    }
    // These are the only analog buttons on the PS4 controller
    speed = map(r2, 0, 200, 0, 255);
    if (speed > 255) {
      speed = 255;
    }

    // analogWrite(motor2speed, speed);
    analogWrite(motor2speed, speed);
    analogWrite(motor2speed, speed);


    Serial.print(F("\tservo angle: "));
    Serial.print(pos);
    Serial.print(F(" speed: "));
    Serial.print(speed);
    Serial.print(F(" offset: "));
    Serial.print(offset);
    Serial.print("\r\n");


    if (PS4.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS4.disconnect();
    }
  }
}
