/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
� Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 6/30/2019
Author  : 
Company : 
Comments: 


Chip type               : ATmega32
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32.h>
#include <mega32a.h>
#include <delay.h>
#include <stdio.h>
#include <math.h>



// Graphic Display functions
#include <glcd.h>

// Font used for displaying text
// on the graphic display
#include <font5x7.h>

// Declare your global variables here

// Standard Input/Output functions
#include <stdio.h>

// Voltage Reference: Int., cap. on AREF
#define ADC_VREF_TYPE ((1<<REFS1) | (1<<REFS0) | (0<<ADLAR))

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}


int get_key(void){  
    DDRD=0b00000000;
    PORTD=0b00001111;
 
    DDRD=0b00010000; 
    if(PIND.0==0)return 7;
    if(PIND.1==0)return 4;
    if(PIND.2==0)return 1;
    if(PIND.3==0)return '*';
 
    DDRD=0b00100000;
    if(PIND.0==0)return 8;
    if(PIND.1==0)return 5;
    if(PIND.2==0)return 2;
    if(PIND.3==0)return 0;
 
    DDRD=0b01000000;
    if(PIND.0==0)return 9;
    if(PIND.1==0)return 6;
    if(PIND.2==0)return 3;
    if(PIND.3==0)return '=';
    
    DDRD=0b10000000;
    if(PIND.0==0)return 100;
    if(PIND.1==0)return 101;
    if(PIND.2==0)return 102;
    if(PIND.3==0)return 103;
 
    return 255;
}

eeprom char avg_eeprom[100];
void main(void)
{
char str[15];
int key;
int adc_data[4];
char adc_buffer[16];
int i;
int m=0,n;
int x=1;
int T=0;
int delta_T;
int V1;
int V2;
int temp=0;
int keypress=0;
float var,avg;
float fan;
bool print_fan=false;
char avgg[100];


// Declare your local variables here
// Variable used to store graphic display
// controller initialization data
GLCDINIT_t glcd_init_data;

// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
DDRB=(1<<DDB7) | (1<<DDB6) | (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
// State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
DDRC=(1<<DDC7) | (1<<DDC6) | (1<<DDC5) | (1<<DDC4) | (1<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);
// State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Toggle on compare match
TCCR0=(0<<WGM00) | (0<<COM01) | (1<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Low level
// INT1: Off
// INT2: Off
GICR|=(0<<INT1) | (1<<INT0) | (0<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (1<<INTF0) | (0<<INTF2);

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: Off
// USART Mode: Asynchronous
// USART Baud Rate: 9600
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x33;

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);

// ADC initialization
// ADC Clock frequency: 125.000 kHz
// ADC Voltage Reference: Int., cap. on AREF
// ADC Auto Trigger Source: ADC Stopped
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

// Graphic Display Controller initialization
// The KS0108 connections are specified in the
// Project|Configure|C Compiler|Libraries|Graphic Display menu:
// DB0 - PORTC Bit 0
// DB1 - PORTC Bit 1
// DB2 - PORTC Bit 2
// DB3 - PORTC Bit 3
// DB4 - PORTC Bit 4
// DB5 - PORTC Bit 5
// DB6 - PORTC Bit 6
// DB7 - PORTC Bit 7
// E - PORTB Bit 0
// RD /WR - PORTB Bit 1
// RS - PORTB Bit 2
// /RST - PORTB Bit 6
// CS1 - PORTB Bit 4
// CS2 - PORTB Bit 5

// Specify the current font for displaying text
glcd_init_data.font=font5x7;
// No function is used for reading
// image data from external memory
glcd_init_data.readxmem=NULL;
// No function is used for writing
// image data to external memory
glcd_init_data.writexmem=NULL;

glcd_init(&glcd_init_data);



      glcd_clear();
      glcd_line(5,5,5,55);
      glcd_line(120,5,120,55);
      glcd_line(5,55,120,55);
      glcd_line(5,5,120,5);    
      glcd_outtextxy(9,25,"In the name of God");
      delay_ms(300);

      glcd_clear();       
      glcd_outtextxy(10,15,"Tempreture?");   
      glcd_outtextxy(10,35,"(click '=' to continue)");
      z: 
      while(keypress==0){
        key=get_key();
        if(key<255){
          if(key!=102){
            if(key=='='){
              keypress=1;
            }
            if(key!=255 && key!='='){                       
              glcd_clear();
              T=key+T*10;   
              sprintf(str,"%d",T); 
              glcd_outtextxy(10,10,str);   
              delay_ms(20);
              key=255;
            }
          }
          if(key==102){
            glcd_clear();
            T=0;
            goto z;
          }
        } 
      }
      glcd_clear();
      sprintf(str,"T is:%02d ",T);
      glcd_outtextxy(16,0,str);
      
      delay_ms(200);
      x=1;
      temp=0;
      keypress=0;
      glcd_clear();       
      glcd_outtextxy(10,15,"V1?");   
      glcd_outtextxy(10,35,"(click '=' to continue)");
      z2: 
      while(keypress==0){
        key=get_key();
        if(key<255){
          if(key!=102){
            if(key=='='){
              keypress=1;
            }
            if(key!=255 && key!='='){                       
              glcd_clear();
              V1=key+V1*10;   
              sprintf(str,"%d",V1); 
              glcd_outtextxy(10,10,str);   
              delay_ms(20);
              key=255;
            }
          }
          if(key==102){
            glcd_clear();
            V1=0;
            goto z2;
          }
        } 
      }
      glcd_clear();
      sprintf(str,"V1 is:%02d ",V1);
      glcd_outtextxy(16,0,str);
      
      
      delay_ms(200);
      x=1;
      temp=0;
      keypress=0;
      glcd_clear();       
      glcd_outtextxy(10,15,"V2?");   
      glcd_outtextxy(10,35,"(click '=' to continue");
      z3: 
      while(keypress==0){
        key=get_key();
        if(key<255){
          if(key!=102){
            if(key=='='){
              keypress=1;
            }
            if(key!=255 && key!='='){                       
              glcd_clear();
              V2=key+V2*10;   
              sprintf(str,"%d",V2); 
              glcd_outtextxy(10,10,str);   
              delay_ms(20);
              key=255;
            }
          }
          if(key==102){
            glcd_clear();
            V2=0;
            goto z3;
          }
        } 
      }
      glcd_clear();
      sprintf(str,"V2 is:%02d ",V2);
      glcd_outtextxy(16,0,str);
      
      
      delay_ms(200);
      x=1;
      temp=0;
      keypress=0;
      glcd_clear();       
      glcd_outtextxy(10,15,"delta_T?");   
      glcd_outtextxy(10,35,"(click '=' to continue)");
      z4: 
      while(keypress==0){
        key=get_key();
        if(key<255){
          if(key!=102){
            if(key=='='){
              keypress=1;
            }
            if(key!=255 && key!='='){                       
              glcd_clear();
              delta_T=key+delta_T*10;   
              sprintf(str,"%d",delta_T); 
              glcd_outtextxy(10,10,str);   
              delay_ms(20);
              key=255;
            }
          }
          if(key==102){
            glcd_clear();
            delta_T=0;
            goto z3;
          }
        } 
      }
      glcd_clear();
      sprintf(str,"delta_T is:%02d ",delta_T);
      glcd_outtextxy(16,0,str);
      delay_ms(200);

          
    
    glcd_clear();
    glcd_outtextxy(25,25,"start?");
    while(1){
      temp=get_key();
      if(temp=='=' || temp=='*') break;
    }
    
    DDRB.3=1;
    OCR0=128;
    TCCR0=0x69; 
      
while (1)
    {
                               
      avg=0;
      var=0;
      glcd_clear();
      for(i=0;i<4;i++){
        adc_data[i]=read_adc(i)/4;
        sprintf(adc_buffer,"TEMP%d=%d C",i+1,adc_data[i]);
        glcd_outtextxy(16,10*i,adc_buffer);
        //delay_ms(10);
      }
      
      
      for(i=0;i<4;i++)
        avg+=adc_data[i];
      avg/=4;
      sprintf(adc_buffer,"avg=%1.2f",avg);
      glcd_outtextxy(16,10*i,adc_buffer);
      //delay_ms(10);
       
      
      for(i=0;i<4;i++)
        var+=(adc_data[i]-avg)*(adc_data[i]-avg);
      var/=4;
      sprintf(adc_buffer,"var=%1.2f",var);
      glcd_outtextxy(16,50,adc_buffer);
      
      
      avg_eeprom[m]=(adc_data[0]+adc_data[1]+adc_data[2]+adc_data[3])/4;
      m++;
        
      
      
      
      delay_ms(100);
      fan=var;
      if(fan>V2){
         PORTB.7=1;                         
         if(print_fan==false){
            glcd_clear();
            glcd_outtextxy(15,25,"Fan: ON");
            print_fan=true;
         }   
      }
                  
      else if(fan<V1) { 
        PORTB.7=0; 
        if(print_fan==true){
          glcd_clear();
          glcd_outtextxy(15,25,"Fan: OFF");
          print_fan=false; 
        }
      }
      
      delay_ms(100);
      
      glcd_clear();
      glcd_outtextxy(15,25,"chart?");
      key=get_key();
      delay_ms(20);
      
      //for(i=0;i<delta_T*100;i++){
        avgg[m]=(adc_data[0]+adc_data[1]+adc_data[2]+adc_data[3])/4;
        if(avgg[m]>T) PORTB.3=0;
        if(avgg[m]<T) PORTB.3=1;
        OCR0=(T-avgg[m])*10;
        if(OCR0>255){
          OCR0=255;
        }
       //}
      
      if(key=='*'){
                               
        glcd_clear(); 
        glcd_line(5,5,5,55); 
        glcd_line(5,55,155,55);        
        glcd_line(6,T,155,T);
        
        for(n=0;n<m;n++){
          glcd_setpixel(6+delta_T*n,55-avgg[n]);
        }
        
        delay_ms(1000);
      }              
    }
}