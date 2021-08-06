//utilize arduino mega to control the five-segment soft body
//PWM control:1) Timer2(8-bit): pin9-OC2B; pin10-OC2A; 
//            2) Timer4(16bit): pin6-OC4A; pin7-OC4B; pin8-OC4C;
//PWM waveform is triangle form;

const float pi=3.14;
int valve1 = 10; int valve2 = 9; valve3 = 8; valve4 = 7; valve5 = 6;
//configure for the PWM parameter within different waveform
float x1=0, x2=0, x3=0, x4=0, x5=0;
int y1=0, y2=0, y3=0, y4=0, y5=0;  
 
void setup() {
  Serial.begin(9600);
  pinMode(valve1, OUTPUT); pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT); pinMode(valve4, OUTPUT);  pinMode(valve5, OUTPUT); 

  cli(); // configure interrupts;
  //configure the port with around 1000Hz;(16MHz/256(WGM)/64(CS))
  TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);     //Compare Output Mode (Toggle compare) & Waveform Generation Mode(111)--fast PWM
  TCCR2B = _BV(WGM22) | _BV(CS22);      //prescaler configured bia TCCR: 8 bit ->1/64;
  TCCR4A = _BV(COM4A0) | _BV(COM4B1) | _BV(COM4C1) | _BV(WGM41) | _BV(WGM40); 
  TCCR4B = _BV(WGM42) | _BV(CS20);      //different in CS configuration     
  TCCR4C = _BV(CS21) ;
  TIMSK2 |= (1 << OCIE2A);              //enable comparation in timer/encounter
  TIMSK2 |= (1 << OCIE2B); 
  TIMSK4 |= (1 << OCIE4A);      
  TIMSK4 |= (1 << OCIE4B);     
  TIMSK4 |= (1 << OCIE4C);    
  sei();  //corresponding to function cli();
}

// waveform with sin curve -- frequency:1Hz, phase difference: pi/4;
//the first test is for 1000Hz, with the frequency = 16MHz/256/64;
ISR(TIMER2_COMPA_vect) //port10
{
  if(x1<=2)
  {
    if(0<=x1 && x1<=0.2){y1 = 635*x1+128;}
    else if(x1<=0.4){y1 = -635*x1+382};
    else y1=0;
    OCR2A = y1;
    x1 = x1+0.0005;           // count for 4000 times = 4s;
  }
  else x1=0;     
}
ISR(TIMER2_COMPB_vect) //port9
{
  if(x2<=2)
  {
    if(0.4<=x2 && x2<=0.6){y2 = 635*(x2-0.4)+128;}
    else if((0.6<x2 && x2<=0.8){y2 = -635*(x2-0.4)+382;}
    else y2=0;
    OCR2B = y2;
    x2 = x2+0.0005;
  }
  else x2=0;
}
ISR(TIMER4_COMPC_vect) //port8
{
  if(x3<=2)
  {
    if(0.8<=x3 && x3<=1){y3 = 635*(x3-0.8)+128;}
    else if(1<=x3 && x3<=1.2){y3 = -635*(x3-0.8)+382;}
    else y3=0;
    OCR4C = y3;
    x3 = x3+0.0005;           
  }
  else x3=0;     
}
ISR(TIMER4_COMPB_vect) //port7
{
  if(x4<=2)
  {
    if(1.2<=x4 && x4<=1.4){y4 = 635*(x4-1.2)+128;}
    else if(1.4<=x4 && x4<=1.6){y4 = -635*(x4-1.2)+382;}
    else y4=0;
    OCR4B = y4;
    x4 = x4+0.0005;
  }
  else x4=0;
}
ISR(TIMER4_COMPA_vect) //port6
{
  if(x5<=2)
  {
    if(1.6<=x5 && x5<=1.8){y5 = 635*(x5-1.6)+128;}
    else if(1.8<=x5 && x5<=2){y5 = -635*(x5-1.6)+382;}
    else y5=0;
    OCR4A = y5;
    x5 = x5+0.0005;           
  }
  else x5=0;     
}

void loop() 
{}
