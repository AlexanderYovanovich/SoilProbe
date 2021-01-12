//Attiny84 interrupt test
//LED is on PA7
//Pushbutton is on PA0 (using external INT0 interrupt)

#include <Interrupt.h>

int led = 7;
int button = 0;
//for setting register bits with AVR code
//cbi and sbi are standard (AVR) methods for setting, or clearing, bits in PORT (and other) variables.
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif



void setup(void)
{
  cli(); //disable global interrupts
  pinMode(led, OUTPUT);
  pinMode(button,INPUT_PULLUP);
 
  // Set ISC01= 1 and ISC00 = 0 to generate an external interrupt request on the falling edge of INT0
  sbi(MCUCR,ISC01);
  cbi(MCUCR,ISC00);
 
  //enable external interrupt request 0
  sbi(GIMSK,INT0);
 
  sei(); //enable global interrupts
 
}

void loop(void)
{
 //nothing right now
 
}


//external interrupt ISR (for INT0 pin)
ISR(INT0_vect)
{
  blink_led();
}

void blink_led(void)
{
  digitalWrite(led,HIGH);
  delay(1000);  //delay one second
  digitalWrite(led,LOW);
}