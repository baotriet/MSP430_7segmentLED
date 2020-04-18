
#include "msp430g2553.h"
char segment[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x83,0xf8,0x80,0x98};
int i=0,j=0,counter;

void display (unsigned char num)
{
    i = counter / 10;
    j= counter % 10;
   
  P1OUT |= BIT6;
  P1OUT &= ~BIT7;
  P2OUT = segment[i];
  __delay_cycles(500);
  P2OUT = 0XFF;
  
 
  P1OUT &= ~BIT6;
  P1OUT |= BIT7;
  P2OUT = segment[j];
  __delay_cycles(500);
  P2OUT = 0XFF;
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  
 
  // Turn on led 7 segments
  P2DIR = 0xff;
  P2SEL &= ~BIT6;
  
  // enable interrupt in BIT5 with button
  P1DIR &= ~BIT5;
  P1IE |= BIT5;
  P1IES &= ~BIT5;
  P1DIR |= BIT6 + BIT7;
  __enable_interrupt();

  while(1)
  {
   display(counter); 
  }
  
}
#pragma vector = PORT1_VECTOR
__interrupt void First_time (void)
{
  if (P1IFG & BIT5)
  {
     if(TACTL != 0X00)
    {
      TACTL = 0X00;
      counter = 0;
      P1IFG &= ~BIT5;
    }
    else 
    {
      CCTL0 = CCIE;
      CCR0 = 0X8FFF;
      TACTL = TASSEL_2 + ID_3 + MC_2 ;
      P1IFG &= ~BIT5;
    }
   }
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
   counter++;
     
     if(counter >= 100)
       counter = 0x00;
        
     CCTL0 &= ~CCIFG;
}
