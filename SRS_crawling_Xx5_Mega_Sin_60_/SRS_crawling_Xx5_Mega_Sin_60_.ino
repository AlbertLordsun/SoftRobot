// Crawling with five segments via sin wave;
// Stride duration is 1s, each segment contracts 0.2s without signal overlapping;
// PWM control: 1)timer1: 11-OCR1A-249;  12-OCR1B;
//              2)timer3: 5-OCR3A-249;   2-OCR3B; 3-OCR3C;
//              3)timer4: 6-OCR4A-249;   7-OCR4B; 8-OCR4C; 
// PWM signal sequence: 2-3-7-8-12;

const float pi = 3.14;
int valve1 = 12; int valve2 = 8; int valve3 = 7; int valve4 = 3; int valve5 = 2;
//configure for the PWM parameter within different waveform
float x1=0, x2=0, x3=0, x4=0, x5=0;
int y1=0, y2=0, y3=0, y4=0, y5=0;

void setup() {
  Serial.begin(9600);
  // Input mode for the pressure sensor
  pinMode(A1, INPUT); 
  pinMode(A2, INPUT);
  pinMode(A3, INPUT); 
  pinMode(A4, INPUT); 
  pinMode(A5, INPUT); 
  // Output mode for the solenoid valve
  pinMode(valve1, OUTPUT); 
  pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT); 
  pinMode(valve4, OUTPUT);  
  pinMode(valve5, OUTPUT); 
  cli(); // configure interrupts;
//  TCCR2A = 0; TCCR2B = 0;
//  TCCR4A = 0; TCCR4B = 0; 

  //configure the port with 1000Hz;(16MHz/250(WGM)/64(CS)=1000Hz)
  TCCR1A = _BV(COM1A0) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10); 
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10) ;       
  TCCR3A = _BV(COM3A0) | _BV(COM3B1) | _BV(COM3C1) | _BV(WGM31) | _BV(WGM30); 
  TCCR3B = _BV(WGM33) | _BV(WGM32) | _BV(CS31) | _BV(CS30) ;       
  TCCR4A = _BV(COM4A0) | _BV(COM4B1) | _BV(COM4C1) | _BV(WGM41) | _BV(WGM40); 
  TCCR4B = _BV(WGM43) | _BV(WGM42) | _BV(CS41) | _BV(CS40) ; 
  //enable comparation in timer/encounter      
//  TIMSK1 |= (1 << OCIE1A);                                           
  TIMSK1 |= (1 << OCIE1B); 
//  TIMSK3 |= (1 << OCIE3A);      
  TIMSK3 |= (1 << OCIE3B);     
  TIMSK3 |= (1 << OCIE3C); 
//  TIMSK4 |= (1 << OCIE4A);      
  TIMSK4 |= (1 << OCIE4B);     
  TIMSK4 |= (1 << OCIE4C);  
  // define the 
  OCR1A = 249;
  OCR3A = 249;
  OCR4A = 249;  
  sei();  //corresponding to function cli();
}

// function for square waveform, stride duration is 1s, PWM signal frequency is 1000Hz = 16MHz/250/64;
// First test: phase difference is 2*pi/5, equal to 0.2s;
// Stride duration can be changed via step length in x;

// from posterior to anterior;
ISR(TIMER1_COMPB_vect) //port12
{
  if(x1<=0.4)
  {
    if(0.32<=x1 && x1<=0.4){y1 = 59*sin((x1-0.32)/0.2*pi)+190;}
    else if(0<=x1 && x1<=0.12){y1 = 59*sin((x1+0.08)/0.2*pi)+190;}
    else y1=0;
    OCR1B = y1;
    x1 = x1+0.0002;           // stride duration = 1/(1000Hz*step length);
  }
  else x1=0;     
}
ISR(TIMER4_COMPC_vect) //port8
{
  if(x2<=0.4)
  {
    if(0.24<=x2 && x2<=0.4){y2 = 59*sin((x2-0.24)/0.2*pi)+190;}
    else if(0<=x2 && x2<=0.04){y2 = 59*sin((x2+0.16)/0.2*pi)+190;}
    else y2=0;
    OCR4C = y2;
    x2 = x2+0.0002;
  }
  else x2=0;
}
ISR(TIMER4_COMPB_vect) //port7
{
  if(x3<=0.4)
  {
    if(0.16<=x3 && x3<=0.36){y3 = 59*sin((x3-0.16)/0.2*pi)+190;}
    else y3=0;
    OCR4B = y3;
    x3 = x3+0.0002;           
  }
  else x3=0;     
}
ISR(TIMER3_COMPC_vect) //port3
{
  if(x4<=0.4)
  {
    if(0.08<=x4 && x4<=0.28){y4 = 59*sin((x4-0.08)/0.2*pi)+190;}
    else y4=0;
    OCR3C = y4;
    x4 = x4+0.0002;
  }
  else x4=0;
}
ISR(TIMER3_COMPB_vect) //port2
{
  if(x5<=0.4)
  {
    if(0<=x5 && x5<=0.2){y5 = 59*sin(x5/0.2*pi)+190;}
    else y5=0;
    OCR3B = y5;
    x5 = x5+0.0002;           
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

  // consistent with the figure update frequency
  delay(33);
}
