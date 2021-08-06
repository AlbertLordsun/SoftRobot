// Crawling with five segments via saw1 wave;
// Stride duration is 1s, each segment contracts 0.2s without signal overlapping;
// PWM control: 1)timer1: 11-OCR1A-249;  12-OCR1B;
//              2)timer3: 5-OCR3A-249;   2-OCR3B; 3-OCR3C;
//              3)timer4: 6-OCR4A-249;   7-OCR4B; 8-OCR4C; 
// PWM signal sequence: 2-3-7-8-12;

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
//  TCCR3A = _BV(COM3A0) | _BV(COM3B1) | _BV(COM3C1) | _BV(WGM31) | _BV(WGM30); 
//  TCCR3B = _BV(WGM33) | _BV(WGM32) | _BV(CS31) | _BV(CS30) ;       
//  TCCR4A = _BV(COM4A0) | _BV(COM4B1) | _BV(COM4C1) | _BV(WGM41) | _BV(WGM40); 
//  TCCR4B = _BV(WGM43) | _BV(WGM42) | _BV(CS41) | _BV(CS40) ; 

  //enable comparation in timer/encounter      
  TIMSK1 |= (1 << OCIE1B); 
  // define the OCRnX range for PWM frequency
  OCR1A = 249;
  
  sei();  //corresponding to function cli();
}


// 0) here is for the deformation test of saw1 wave
// stride duration is 2s;
ISR(TIMER1_COMPB_vect) //port12
{
  if(x1<=1)
  {
    if(0<=x1 && x1<=0.5){y1 = -118*x1+249;}
    else y1=0;
    OCR1B = y1;
    x1 = x1+0.0005;           
  }
  else x1=0;     
}

void loop()
{
  // calibrate for the vacuum pressure values
  int P1 = analogRead(A1);
  float P1v = (P1/1023.0-0.92)/0.007652;
  Serial.println(P1v);
  // consistent with the update frequency;
  delay(25);
}
