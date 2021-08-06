//utilize arduino mega to control the two-segment soft body.
//PWM control pin: 10 and 9;--Timer2(8-bit): 9-OC2B; 10-OC2A;

#include<SimpleKalmanFilter.h>
#define NOP do { __asm__ __volatile__ ("nop"); } while (0)
const float pi=3.14;
float x1=0, x2=0;
int y1=0, y2=0;
int valve1 = 10; int valve2 = 9;
SimpleKalmanFilter P1value(0.1, 0.1, 0.01); SimpleKalmanFilter P2value(0.1, 0.1, 0.01);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1, INPUT); 
  pinMode(A2, INPUT);
  pinMode(A3, INPUT); 
  pinMode(A4, INPUT); 
  pinMode(A5, INPUT); 
 
  pinMode(valve1, OUTPUT); pinMode(valve2, OUTPUT);
  cli(); // stop interrupts;
  int Ones = 255;

  //configure the port with around 1000Hz;(16MHz/256(WGM)/64(CS))
  TCCR2A =  _BV(COM2A1) | _BV(COM2B1)|_BV(WGM20);      
  TCCR2B = _BV(CS21) | _BV(CS20);                      //prescaler: 64;
  TIMSK2 |=(1 << OCIE2A);      
  TIMSK2 |=(1 << OCIE2B);     
  sei();  //corresponding to function cli();
}

// waveform with sin curve -- frequency:1Hz, phase difference: pi/4;
//the first test is for 1Hz, with the frequency = 16MHz/256/64 /1000;
ISR(TIMER2_COMPA_vect) //port10
{
  if(x1<=2)
  {
    y1=127*sin(x1*pi)+128;
    OCR2A = y1;
    x1 = x1+0.0005;
  }
  else x1=0;     
}
ISR(TIMER2_COMPB_vect) //port9
{
  if(x2<=2)
  {
    y2=127*sin((x2-0.2)*pi)+128;
    OCR2B = y2;
    x2 = x2+0.0005;
  }
  else x2=0;
}

void loop() 
{
  // calibrate for the vacuum pressure values
  int P1 = analogRead(A1);
  float P1v = (P1/1023.0-0.92)/0.007652;
  int P2 = analogRead(A2);
  float P2v = (P2/1023.0-0.92)/0.007652;
  Serial.print(P1v);
  Serial.print('\t');
  Serial.println(P2v);
}

void delay_(int ms) // configure the delay function due to the use of timer0
{
  for(int i=0; i<ms; i++)
  {for(unsigned long j=0;j<1985;j++) NOP;}        
}
