#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>

#define CHSC6X_I2C_ID 0x2e
#define CHSC6X_MAX_POINTS_NUM 1
#define CHSC6X_READ_POINT_LEN 5
#define TOUCH_INT D7

uint8_t screen_rotation;
TFT_eSPI tft = TFT_eSPI(TFT_WIDTH,TFT_HEIGHT);

void disp_init(void)
{
    pinMode(TFT_BL,OUTPUT);
    digitalWrite(TFT_BL,HIGH);
    tft.begin();
    tft.setRotation(screen_rotation);
    tft.fillScreen(TFT_WHITE);
}

bool chsc6x_is_pressed(void)
{
    if(digitalRead(TOUCH_INT) != LOW) {
        delay(3);
        if(digitalRead(TOUCH_INT) != LOW)
        return false;
    }
    return true;
}

void touch_init(void)
{
    pinMode(TOUCH_INT,INPUT_PULLUP);
    Wire.begin();
}

void chsc6x_convert_xy(uint8_t *x, uint8_t *y)
{
    uint8_t x_tmp = *x, y_tmp = *y, _end = 0;
    for(int i=1; i<=screen_rotation; i++){
        x_tmp = *x;
        y_tmp = *y;
        _end = (i % 2) ? TFT_WIDTH : TFT_HEIGHT;
        *x = y_tmp;
        *y = _end - x_tmp;
    }
}

void chsc6x_get_xy(uint8_t * x, uint8_t * y)
{
    uint8_t temp[CHSC6X_READ_POINT_LEN] = {0};
    uint8_t read_len = Wire.requestFrom(CHSC6X_I2C_ID, CHSC6X_READ_POINT_LEN);
    if(read_len == CHSC6X_READ_POINT_LEN){
        Wire.readBytes(temp, read_len);
        if (temp[0] == 0x01) {
        chsc6x_convert_xy(&temp[2], &temp[4]);
        *x = temp[2];
        *y = temp[4];
        }
    }
}