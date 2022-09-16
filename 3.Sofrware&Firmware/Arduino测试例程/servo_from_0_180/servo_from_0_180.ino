/*************************************************** 
 对于ARDUINO UNOS，这是SCL->模拟5，SDA - >模拟4
  ****************************************************/
  
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
 
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
 
#define SERVOMIN  102               //0.5/20 * 4096 = 102
#define SERVOMAX  512               //2.5/20 * 4096 = 512
 
void setup() {
  Serial.begin(115200);
  Serial.println("16 channel Servo test!");
  
  pwm.begin();
  pwm1.begin();
  pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
  pwm1.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
}
 
void loop() {
   for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        for(int i=0;i<16;i++)
        {
          pwm.setPWM(i, 0, pulselen);
          pwm1.setPWM(i, 0, pulselen);
        }
        delayMicroseconds(200);
        ESP.wdtFeed();                    //喂狗防止复位
   }
   for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        for(int i=0;i<16;i++)
        {
          pwm.setPWM(i, 0, pulselen);
          pwm1.setPWM(i, 0, pulselen);
        }
        delayMicroseconds(200);
        ESP.wdtFeed();                    //喂狗防止复位
   }
}
