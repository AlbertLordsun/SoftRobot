// target:soft robotic system with the control of multi-thread for 10 chambers
// result wanted: gait switching with shifting of left and right chambers;
// pump configuration:1:pin13;2:pin12;
// valve configuration:1-5:pin11,10,9,8,7; 6-10:pin6,5,4,3,2;
// pressure configuration: from A1 to A10

#include<SCoop.h>
int pump1 = 13; int pump2 = 12;
int valve1 = 11; int valve2 = 10; int valve3 = 9; int valve4 = 8; int valve5 = 7;
int valve6 = 6; int valve7 = 5; int valve8 = 4; int valve9 = 3; int valve10 = 2;
int pressure1 = A1; int pressure2 = A2; int pressure3 = A3; int pressure4 = A4; int pressure5 = A5;
int pressure6 = A6; int pressure7 = A7; int pressure8 = A8; int pressure9 = A9; int pressure10 = A10;

defineTaskLoop(Display)
{
  // pressure calibration;
  float P1 = map(analogRead(pressure1),5,100,0,40); float P2 = map(analogRead(pressure2),5,100,0,40);
  float P3 = map(analogRead(pressure3),5,100,0,40); float P4 = map(analogRead(pressure4),5,100,0,40);
  float P5 = map(analogRead(pressure5),5,100,0,40); float P6 = map(analogRead(pressure6),5,100,0,40);
  float P7 = map(analogRead(pressure7),5,100,0,40); float P8 = map(analogRead(pressure8),5,100,0,40);
  float P9 = map(analogRead(pressure9),5,100,0,40); float P10 = map(analogRead(pressure10),5,100,0,40);
  // display of pressure value;
  Serial.println("Values of pressure sensor island:");
  Serial.print(P1); Serial.print("/t"); Serial.print(P2); Serial.print("/t");
  Serial.print(P3); Serial.print("/t"); Serial.print(P4); Serial.print("/t");
  Serial.println(P5);                   Serial.print(P6); Serial.print("/t");
  Serial.print(P7); Serial.print("/t"); Serial.print(P8); Serial.print("/t");
  Serial.print(P9); Serial.print("/t"); Serial.println(P10);
  Serial.println("...............");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pump1,OUTPUT); pinMode(pump2,OUTPUT);
  pinMode(valve1,OUTPUT); pinMode(valve2,OUTPUT);
  pinMode(valve3,OUTPUT); pinMode(valve4,OUTPUT);
  pinMode(valve5,OUTPUT); pinMode(valve6,OUTPUT);
  pinMode(valve7,OUTPUT); pinMode(valve8,OUTPUT);
  pinMode(valve9,OUTPUT); pinMode(valve10,OUTPUT);
  int Ones = 255;
  TCCR0A &= ~Ones； TCCR0B &= ~Ones;    // for pin 13,4;
  TCCR1A &= ~Ones； TCCR1B &= ~Ones;    // for pin 11,12;
  TCCR2A &= ~Ones； TCCR2B &= ~Ones;    // for pin 10,9;
  TCCR3A &= ~Ones； TCCR3B &= ~Ones;    // for pin 5,2,3;
  TCCR4A &= ~Ones； TCCR4B &= ~Ones;    // for pin 6,7,8;
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM02) | _BV(WGM00);
  TCCR0B = _BV(CS02) | _BV(CS00);
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM12) | _BV(WGM10);
  TCCR1B = _BV(CS12) | _BV(CS10);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM22) | _BV(WGM20);
  TCCR2B = _BV(CS22) | _BV(CS20);
  TCCR3A = _BV(COM3A1) | _BV(COM3B1) | _BV(COM3C1) | _BV(WGM32) | _BV(WGM30);
  TCCR3B = _BV(CS32) | _BV(CS30);
  TCCR4A = _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1) | _BV(WGM42) | _BV(WGM40);
  TCCR4B = _BV(CS42) | _BV(CS40);

  mySCoop.start();
}

void loop() {

  // 1) for sequential wave propagation
  OCR0A = 255; OCR1B = 255;  //configuration for the pins of pumps;
  // valve sequnce for peristalsis: 11-6; 10-5; 9-4; 8-3; 7-2;
  OCR1A = 255; OCR4A = 255;
  delay(2000);
  OCR1A = 0; OCR4A = 0; OCR2A = 255; OCR3A = 255;
  delay(2000);
  OCR2A = 0; OCR3A = 0; OCR2B = 255; OCR0B = 255;
  delay(2000);
  OCR2B = 0; OCR0B = 0; OCR4C = 255; OCR3C = 255;
  delay(2000);
  OCR4C = 0; OCR3C = 0; OCR4B = 255; OCR3B = 255;
  delay(2000);
  OCR4B = 0; OCR3B = 0;

  
//  // 2) gait switch bilaterally 
//  OCR0A = 255; OCR1B = 255;  //configuration for the pins of pumps;
//  // valve sequnce for gait swiching: 11-5-9-3-7; 6-10-4-8-2; 
//  OCR1A = 255; OCR3A = 255; OCR2B = 255; OCR3C = 255; OCR4B = 255;
//  delay(2000);
//  OCR1A = 0; OCR3A = 0; OCR2B = 0; OCR3C = 0; OCR4B = 0;
//  OCR4A = 255; OCR2A = 255; OCR0B = 255; OCR4C = 255; OCR3B = 255;
//  delay(2000);
//  OCR4A = 0; OCR2A = 0; OCR0B = 0; OCR4C = 0; OCR3B = 0;

  yield();
}
