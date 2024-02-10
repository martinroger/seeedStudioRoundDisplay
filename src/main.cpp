#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <displayExtra.h>


uint8_t touchX;
uint8_t touchY;

int16_t angle = 0;
float finger_angle = 0.0;
TFT_eSprite ringSpr = TFT_eSprite(&tft);
TFT_eSprite background = TFT_eSprite(&tft);

void setup() {

  disp_init();
  touch_init();
  Serial.begin(115200);

  ringSpr.createSprite(240,240);
  background.createSprite(240,240);
  background.fillSprite(TFT_WHITE);
  ringSpr.drawSmoothArc(120,120,120,100,45,315,TFT_RED,TFT_RED,true);
  ringSpr.pushSprite(120,120,TFT_BLACK);
  
}

void loop() {

  if(chsc6x_is_pressed())
  {
    chsc6x_get_xy(&touchX,&touchY);
    //Serial.print(touchX);
    //Serial.print(',');
    //Serial.println(touchY);
    finger_angle = (atan2(120-(float)touchX,120-(float)touchY)*180.0/PI+180);
    finger_angle = (float)map((long)finger_angle,360,0,0,360);
    Serial.print(">finger_angle:");
    Serial.println(finger_angle);
    Serial.print(">A0:");
    Serial.println(analogRead(A0));
    angle = (int16_t)finger_angle;
    //tft.drawCircle(touchX,touchY,10,TFT_BLACK);
    //tft.drawCircle(120,120,map(analogRead(A0),0,4096,20,40),TFT_RED);
    background.fillSprite(TFT_WHITE);
    ringSpr.fillSprite(TFT_BLACK);
    ringSpr.drawSmoothArc(120,120,119,80,(angle+45)%360,(angle+315)%360,TFT_RED,TFT_RED,true);
    ringSpr.pushToSprite(&background,0,0,TFT_BLACK);
    background.pushSprite(0,0);  
    delay(5);

  }
  else
  {
    background.fillSprite(TFT_WHITE);
    ringSpr.pushRotated(&background,angle,TFT_BLACK);
    background.pushSprite(0,0);
    //angle++;
    //angle = angle%360;
    Serial.print(">A0:");
    Serial.println(analogRead(A0));
    angle = map(analogRead(A0),0,4095,1,360);
    //tft.fillScreen(TFT_WHITE);
  }
  delay(10);


}

