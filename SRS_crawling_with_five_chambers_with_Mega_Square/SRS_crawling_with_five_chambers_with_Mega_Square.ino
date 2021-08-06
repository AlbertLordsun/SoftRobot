//utilize arduino mega to control the five-segment soft body
//PWM control:1) Timer2(8-bit): pin9-OC2B; pin10-OC2A; 
//            2) Timer4(16bit): pin6-OC4A; pin7-OC4B; pin8-OC4C;
//PWM waveform is square form;

const float pi=3.14;
int valve1 = 10; int valve2 = 9; 
int valve3 = 8; int valve4 = 7; int valve5 = 6;
//configure for the PWM parameter within different waveform
float x1=0, x2=0, x3=0, x4=0, x5=0;
int y1=0, y2=0, y3=0, y4=0, y5=0;  
 
void setup() {
  Serial.begin(9600);
  // Input modes for the vacuum pressure sensor
  pinMode(A1, INPUT); 
  pinMode(A2, INPUT);
  pinMode(A3, INPUT); 
  pinMode(A4, INPUT); 
  pinMode(A5, INPUT); 
  // Output modes for the solenoid valve
  pinMode(valve1, OUTPUT); 
  pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT); 
  pinMode(valve4, OUTPUT);  
  pinMode(valve5, OUTPUT); 
  cli(); // configure interrupts;
  // Initialization
//  TCCR2A = 0; TCCR2B = 0;
  //TNT2 = 0;
//  TCCR4A = 0; TCCR4B = 0; 
  //TNT4 = 0;

  //configure the port with around 1000Hz;(16MHz/256(WGM)/64(CS))
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);     //Compare Output Mode (Toggle compare) & Waveform Generation Mode(111)--fast PWM
  TCCR2B = _BV(CS22);                                   //prescaler configured bia TCCR: 8 bit ->1/64 （CS22）;
  TCCR4A = _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1) | _BV(WGM40); 
  TCCR4B = _BV(WGM42) | _BV(CS41) | _BV(CS40) ;                               //different in CS configuration     
  TIMSK2 |= (1 << OCIE2A);                                           //enable comparation in timer/encounter
  TIMSK2 |= (1 << OCIE2B); 
  TIMSK4 |= (1 << OCIE4A);      
  TIMSK4 |= (1 << OCIE4B);     
  TIMSK4 |= (1 << OCIE4C);    
  sei();  //corresponding to function cli(); or use the combination of noInterrupts() and interrupts();
}

// waveform with sin curve -- frequency:1Hz, phase difference: pi/4;
//the first test is for 1000Hz, with the frequency = 16MHz/256/64;
ISR(TIMER2_COMPA_vect) //port10
{
  if(x1<=2)
  {
    if(0<=x1 && x1<=0.4){y1 = 255;}
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
    if(0.4<=x2 && x2<=0.8){y2 = 255;}
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
    if(0.8<=x3 && x3<=1.2){y3 = 255;}
    else y3=0;
    OCR4C = y3;
    x3 = x3+0.001;           
  }
  else x3=0;     
}
ISR(TIMER4_COMPB_vect) //port7
{
  if(x4<=2)
  {
    if(1.2<=x4 && x4<=1.6){y4 = 255;}
    else y4=0;
    OCR4B = y4;
    x4 = x4+0.001;
  }
  else x4=0;
}
ISR(TIMER4_COMPA_vect) //port6
{
  if(x5<=2)
  {
    if(1.6<=x5 && x5<=2){y5 = 255;}
    else y5=0;
    OCR4A = y5;
    x5 = x5+0.001;           
  }
  else x5=0;     
}

void loop() 
{
  // calibrate for the vacuum pressure values
  int P1 = analogRead(A1);
  float P1v = (P1/1023.0-0.92)/0.007652;
  int P2 = analogRead(A2);
  float P2v = (P2/1023.0-0.92)/0.007652;
  int P3 = analogRead(A3);
  float P3v = (P3/1023.0-0.92)/0.007652;
  int P4 = analogRead(A4);
  float P4v = (P4/1023.0-0.92)/0.007652;
  int P5 = analogRead(A5);
  float P5v = (P5/1023.0-0.92)/0.007652;
  Serial.print(P1v);
  Serial.print('\t');
  Serial.print(P2v);
  Serial.print('\t');
  Serial.print(P3v);
  Serial.print('\t');
  Serial.print(P4v);
  Serial.print('\t');
  Serial.println(P5v);
}
