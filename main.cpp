#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t flag[8] = {0x8,0xc,0xe,0xc,0x8,0x8,0x8}; 
uint8_t back[8] = {0x4,0xc,0x1f,0xd,0x5,0x1,0xf};
uint8_t play[8] = {0x0,0x8,0xc,0xe,0xc,0x8,0x0};
uint8_t stop[8] = {0x0,0xa,0xa,0xa,0xa,0xa,0x0};
int SELECT = 0 ,TIMER = 1 ,START = 2 ,STOP = 3 ,SPLIT = 4 ,SCORE = 5;
unsigned long begin = 0 ,count = 0 ,Pcount = 0 ,minu = 0 ,sec = 0, mil = 0;
unsigned long Fmin = 0,Fsec =0 ,Fmil = 0;
int state,number,i;
int A = D6;
int B = D5;
void Select();
void Timer();
void Score();
void Start();
void Stop();
void time();
String TIME;
String score[5];


void setup() {
    lcd.init();
    lcd.backlight();
    pinMode(B,INPUT);
    pinMode(A,INPUT);
    lcd.createChar(0,flag);
    lcd.createChar(1,back);
    lcd.createChar(2,play);
    lcd.createChar(3,stop);
    state = 0;
    number = 0;
    i = 0;
}

void loop() {
    if(state == SELECT){
        Select();
        if(digitalRead(A)==HIGH){
            i = 0;
            number = 1;
            lcd.clear();
            Score();
            while (digitalRead(A)==HIGH)
            {
                delay(10);
            }
            state = SCORE;
        }
        if(digitalRead(B)==HIGH){
            lcd.clear();
            Timer();
            while (digitalRead(B)==HIGH)
            {
                delay(10);
            }
            state = TIMER;
        }
    }
    else if(state == SCORE){
        Score();
        if(digitalRead(A)==HIGH){
            lcd.clear();
            Select();
            while (digitalRead(A)==HIGH)
            {
                delay(10);
            }
            state = SELECT;
        }
        if(digitalRead(B)==HIGH){
            lcd.clear();
            i += 1;
            number += 1;
            if(i > 4){
                i = 0;
                number = 1;
            }
            Score();
            while (digitalRead(B)==HIGH)
            {
                delay(10);
            }
        }
    }
    else if(state == TIMER){
        Timer();
        if(digitalRead(A)==HIGH){
            lcd.clear();
            Select();
            while (digitalRead(A)==HIGH)
            {
                delay(10);
            }
            state = SELECT;
        }
        if(digitalRead(B)==HIGH){
            for (int t = 0;t < 5;t++){
                score[t] = NULL;
            }
            i = 0;
            number = 1;
            lcd.clear();
            lcd.setCursor(8,0);
            lcd.print("00:00:00");
            begin = millis();
            Start();
            while (digitalRead(B)==HIGH)
            {
                delay(10);
                time();
            }
            state = START;
        }
    }
    else if(state == START){
        Start();
        if(digitalRead(A)==HIGH){
            lcd.clear();
            Fmin = minu;
            Fsec = sec;
            Fmil = mil;
            TIME = String(Fmin < 10 ? "0" + String(Fmin) : String(Fmin)) + ":" +
            String(Fsec < 10 ? "0" + String(Fsec) : String(Fsec)) + ":" +
            String(Fmil < 10 ? "00" + String(Fmil) : (Fmil < 100 ? "0" + String(Fmil) : String(Fmil)));
            lcd.setCursor(8,0);
            lcd.print(TIME);
            Pcount = count;
            Stop();
            while (digitalRead(A)==HIGH)
            {
                delay(10);
            }
            state = STOP;
        }
        if(digitalRead(B)==HIGH){
            Fmin = minu;
            Fsec = sec;
            Fmil = mil;
            lcd.setCursor(5,1);
            lcd.print("(");
            lcd.print(number);
            lcd.print(")");
            TIME = String(Fmin < 10 ? "0" + String(Fmin) : String(Fmin)) + ":" +
            String(Fsec < 10 ? "0" + String(Fsec) : String(Fsec)) + ":" +
            String(Fmil < 10 ? "00" + String(Fmil) : (Fmil < 100 ? "0" + String(Fmil) : String(Fmil)));
            lcd.setCursor(8,1);
            lcd.print(TIME);
            score[i] = TIME;
            while (digitalRead(B)==HIGH)
            {
                delay(10);
                time();
            }
            i += 1;
            number += 1;
            if(i > 4){
                i = 0;
                number = 0;
            }
        }
    }
    else if(state == STOP){
        Stop();
        if(digitalRead(A)==HIGH){
            lcd.clear();
            Pcount = 0;
            Timer();
            while (digitalRead(A)==HIGH)
            {
                delay(10);
            }
            state = TIMER;
        }
        if(digitalRead(B)==HIGH){
            lcd.clear();
            begin = millis();
            Start();
            while (digitalRead(B)==HIGH)
            {
                delay(10);
                time();
            }
            state = START;
        }
    }
}

void Select(){
    lcd.setCursor(2,0);
    lcd.print("A");
    lcd.setCursor(5,0);
    lcd.print("SELECT");
    lcd.setCursor(13,0);
    lcd.print("B");
    lcd.setCursor(0,1);
    lcd.print("SCORE");
    lcd.setCursor(11,1);
    lcd.print("TIMER");
}

void Timer(){
    lcd.setCursor(0,0);
    lcd.print("TIMER");
    lcd.setCursor(6,0);
    lcd.print("B");
    lcd.setCursor(7,0);
    lcd.print(char(2));
    lcd.setCursor(0,1);
    lcd.print("A");
    lcd.setCursor(1,1);
    lcd.print(char(1));
}

void Score(){
    lcd.setCursor(5,1);
    lcd.print("(");
    lcd.print(number);
    lcd.print(")");
    lcd.setCursor(8,1);
    lcd.print(score[i]);
    lcd.setCursor(0,0);
    lcd.print("SCORE");
    lcd.setCursor(0,1);
    lcd.print("A");
    lcd.setCursor(1,1);
    lcd.print(char(1));
}

void Start(){
    time();
    lcd.setCursor(0,0);
    lcd.print("TIMER");
    lcd.setCursor(6,0);
    lcd.print("A");
    lcd.setCursor(7,0);
    lcd.print(char(3));
    lcd.setCursor(0,1);
    lcd.print("B");
    lcd.setCursor(1,1);
    lcd.print(char(0));
    
}

void Stop(){
    lcd.setCursor(0,0);
    lcd.print("TIMER");
    lcd.setCursor(6,0);
    lcd.print("B");
    lcd.setCursor(7,0);
    lcd.print(char(2));
    lcd.setCursor(0,1);
    lcd.print("A)");
    lcd.setCursor(2,1);
    lcd.print("RESET");
}

void time(){
    count = (millis() - begin)+Pcount;
    minu = (count / 60000);
    sec = (count % 60000)/1000;
    mil = count % 1000;
    TIME = String(minu < 10 ? "0" + String(minu) : String(minu)) + ":" +
    String(sec < 10 ? "0" + String(sec) : String(sec)) + ":" +
    String(mil < 10 ? "00" + String(mil) : (mil < 100 ? "0" + String(mil) : String(mil)));
    lcd.setCursor(8,0);
    lcd.print(TIME);
}