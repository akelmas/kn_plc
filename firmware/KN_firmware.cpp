
const int ADDR_DIN  [8]={22,24,26,28,30,32,34,36};
const int ADDR_DOUT [8]={4,5,6,7,8,9,10,11};
const int ADDR_TIM1EN_INT   =0;
const int ADDR_TIM1STOP_INT =1;
const int ADDR_CNT1SET_INT  =5;
const int ADDR_CNT1RST_INT  =4;
const int ADDR_CNT2SET_INT  =3;
const int ADDR_CNT2RST_INT  =2;


const int ADDR_TIM1EN   =38;
const int ADDR_TIM1STOP =40;
const int ADDR_CNT1SET  =42;
const int ADDR_CNT1RST  =44;
const int ADDR_CNT2SET  =46;
const int ADDR_CNT2RST  =48;

byte DIN  [8];
byte DOUT [8];
byte AUX  [8];
volatile byte TIM1EN=0;
volatile byte TIM1STOP=0;
byte CNT1SET;
byte CNT1RST;
byte CNT2SET;
byte CNT2RST;

volatile byte TIM1=0;
volatile byte CNT1=0;
volatile byte CNT2=0;
volatile byte CNT1VAL=0;
volatile byte CNT2VAL=0;
volatile int TIM1VAL=0;

void _read();
void _process();
void _write();
void _init_peripherals();
void _TIM1TICK();
void _TIM1EN();
void _TIM1STOP();
void _CNT1SET();
void _CNT2SET();
void _CNT1RST();
void _CNT2RST();
void setup(){
    for (int  i = 0; i < 8; i++)
    {
        pinMode(ADDR_DIN[i],INPUT);
        pinMode(ADDR_DOUT[i],OUTPUT);
    }
    pinMode(ADDR_CNT1SET,OUTPUT);
    pinMode(ADDR_CNT1RST,OUTPUT);
    pinMode(ADDR_CNT2SET,OUTPUT);
    pinMode(ADDR_CNT2RST,OUTPUT);
    pinMode(ADDR_TIM1EN,OUTPUT);
    pinMode(ADDR_TIM1STOP,OUTPUT);
    pinMode(ADDR_CNT1SET_INT,INPUT);
    pinMode(ADDR_CNT1RST_INT,INPUT);
    pinMode(ADDR_CNT2SET_INT,INPUT);
    pinMode(ADDR_CNT2RST_INT,INPUT);
    pinMode(ADDR_TIM1EN_INT,INPUT);
    pinMode(ADDR_TIM1STOP_INT,INPUT);
    _init_peripherals();

}
void loop(){
    _read();
    _process();
    _write();
}

void _read(){
    for (int  i = 0; i < 8; i++)
    {
        DIN[i]=digitalRead(ADDR_DIN[i]);
    }
#ifdef CNT1ON
    digitalWrite(ADDR_CNT1SET,CNT1SET);
    digitalWrite(ADDR_CNT1RST,CNT1RST);

    Serial.println(CNT1VAL);
#endif
#ifdef CNT2ON
    digitalWrite(ADDR_CNT2SET,CNT2SET);
    digitalWrite(ADDR_CNT2RST,CNT2RST);
#endif

#ifdef TIM1ON
digitalWrite(ADDR_TIM1EN,TIM1EN);
digitalWrite(ADDR_TIM1STOP,TIM1STOP);
#endif
}
void _write(){
    for (int  i = 0; i < 8; i++)
    {
        digitalWrite(ADDR_DOUT[i],DOUT[i]);
    }
}

void _init_peripherals(){
    cli();
#ifdef TIM1ON
    TCCR1A = 0;
    TCCR1B = 0;

    OCR1A = 15624;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    TIMSK1 |= (1 << OCIE1A);
    attachInterrupt(ADDR_TIM1EN_INT,&_TIM1EN,RISING);
    attachInterrupt(ADDR_TIM1STOP_INT,&_TIM1STOP,RISING);

#endif
#ifdef CNT1ON
    attachInterrupt(ADDR_CNT1SET_INT,&_CNT1SET,CNT1EDGE);
    attachInterrupt(ADDR_CNT1RST_INT,&_CNT1RST,CNT1EDGE);
#endif
#ifdef CNT2ON
    attachInterrupt(ADDR_CNT2SET_INT,&_CNT2SET,CNT2EDGE);
    attachInterrupt(ADDR_CNT2RST_INT,&_CNT2RST,CNT2EDGE);

#endif
    sei();

}
#ifdef TIM1ON
void _TIM1TICK(){
    unsigned char sreg;

    sreg = SREG;

    cli();
    if(TIM1EN){
    if(TIM1VAL==TIM1DURATION){
        TIM1VAL=0;
        TIM1=1;
    }
    else{
    TIM1VAL++;


}

}

    SREG = sreg;
    sei();

}

void _TIM1EN(){
}

void  _TIM1STOP(){
    TIM1=0;
    TIM1VAL=0;

}



ISR(TIMER1_COMPA_vect)
{
    _TIM1TICK();
}
#endif

#ifdef CNT1ON
void _CNT1SET(){

    CNT1VAL++;
    if(CNT1VAL==CNT1RELOAD){
        CNT1=1;
        CNT1VAL=0;
    }
}
void _CNT1RST(){

    CNT1VAL=0;
    CNT1=0;
}
#endif

#ifdef CNT2ON
void _CNT2SET(){

    CNT2VAL++;
    if(CNT2VAL==CNT2RELOAD){
        CNT2=1;
        CNT2VAL=0;
    }
}
void _CNT2RST(){

    CNT2VAL=0;
    CNT2=0;
}
#endif


