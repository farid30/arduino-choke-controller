#include <stdint.h>
#include <SeeedTouchScreen.h> 
#include <TFTv2.h>
#include <SPI.h>
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 8; 
int servo_set_start = 0;
int servo_set_break = 0;
int servo_set_warm = 0;
int servo_set_hot = 0;
// Create a servo object 
Servo Servo1; 

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // mega
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 54   // can be a digital pin, this is A0
#define XP 57   // can be a digital pin, this is A3 

#elif defined(__AVR_ATmega32U4__) // leonardo
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 18   // can be a digital pin, this is A0
#define XP 21   // can be a digital pin, this is A3 

#else //168, 328, something else
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 14   // can be a digital pin, this is A0
#define XP 17   // can be a digital pin, this is A3 

#endif

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1

#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2


// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM);

void setup(void) { 
Tft.TFTinit(); //init TFT library
Tft.drawRectangle(130, 40, 90,90,RED);
Tft.drawRectangle(130, 190, 90,90,YELLOW);              
Tft.drawRectangle(15, 40, 90,90,GREEN);           
Tft.drawRectangle(15, 190, 90,90,BLUE);
Tft.setDisplayDirect(UP2DOWN);
Tft.drawString("START",180,45,2,WHITE);
Tft.drawString("BREAK",180,195,2,YELLOW);
Tft.drawString("WARM",65,55,2,GREEN);
Tft.drawString("HOT",65,210,2,BLUE);
Serial.begin(9600);
// We need to attach the servo to the used pin number 
Servo1.attach(servoPin); 
//Init the servo into the closed position
Servo1.write(0);
servo_set_start = 1;
servo_set_break = 0;
servo_set_warm = 0;
servo_set_hot = 0;
}

void loop(void) {
  // a point object holds x y and z coordinates
  Point p = ts.getPoint();
  
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
  
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > __PRESSURE) {
     if ((p.x > 130) && (p.x < 205)) {
       if ((p.y > 50) && (p.y < 105)) {
          Serial.println("start press detect");
          Serial.println(p.y);
          if (servo_set_start == 0) {
            Servo1.write(0);
            servo_set_start = 1;
            servo_set_break = 0;
            servo_set_warm = 0;
            servo_set_hot = 0;
            Tft.drawString("START",180,45,2,WHITE);
            Tft.drawString("BREAK",180,195,2,YELLOW);
            Tft.drawString("WARM",65,55,2,GREEN);
            Tft.drawString("HOT",65,210,2,BLUE);
          }
       }
       else if ((p.y > 200) && (p.y < 270)) {
        Serial.println("break press detect");
        Serial.println(p.y);
        if (servo_set_break == 0) {
          Servo1.write(10);
            servo_set_start = 0;
            servo_set_break = 1;
            servo_set_warm = 0;
            servo_set_hot = 0;
            Tft.drawString("START",180,45,2,RED);
            Tft.drawString("BREAK",180,195,2,WHITE);
            Tft.drawString("WARM",65,55,2,GREEN);
            Tft.drawString("HOT",65,210,2,BLUE);
        }
       }
     }
     else if ((p.x > 15) && (p.x < 70)){
      if ((p.y > 50) && (p.y < 105)) {
        Serial.println("warm press detect");
        Serial.println(p.y);
        if (servo_set_warm == 0) {
          Servo1.write(45);
            servo_set_start = 0;
            servo_set_break = 0;
            servo_set_warm = 1;
            servo_set_hot = 0;
            Tft.drawString("START",180,45,2,RED);
            Tft.drawString("BREAK",180,195,2,YELLOW);
            Tft.drawString("WARM",65,55,2,WHITE);
            Tft.drawString("HOT",65,210,2,BLUE);
        }
      }
      else if ((p.y > 200) && (p.y < 270)) {
        Serial.println("hot press detect");
          Serial.println(p.y);
          if (servo_set_hot == 0) {
          Servo1.write(90);
            servo_set_start = 0;
            servo_set_break = 0;
            servo_set_warm = 0;
            servo_set_hot = 1;
            Tft.drawString("START",180,45,2,RED);
            Tft.drawString("BREAK",180,195,2,YELLOW);
            Tft.drawString("WARM",65,55,2,GREEN);
            Tft.drawString("HOT",65,210,2,WHITE);
        }
      }
     }
     }

  delay(200);
}
