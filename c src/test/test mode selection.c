#include <test timer.h>

int32 readInt(void);
void writeInt(void);
void store(void);


int32  t=0;
int count,hit;
int16 ramStart=0x50;
int32 *ramNow;

// Variable declarations for functions (mainly for EEPROM write/read functions)
int32 value;
int8 i,j;
byte addressEEPROM;
byte* addressRAM;


#int_TIMER2
void  TIMER2_isr(void) 
{
t++;
}

#int_EXT
void  EXT_isr(void) 
{
while (true)
{
clear_interrupt(INT_EXT);
   output_high(pin_D2);
   delay_ms(10);
   output_low(pin_D2);
   delay_ms(10);
}
}

#int_EXT1
void  EXT1_isr(void) 
{
while (true)
{
clear_interrupt(INT_EXT1);
   output_high(pin_D3);
   delay_ms(10);
   output_low(pin_D3);
   delay_ms(10);
}
}

#int_EXT2
void  EXT2_isr(void) 
{
reset_cpu();
}



void main()
{

set_tris_D(0);

setup_timer_2(T2_DIV_BY_1,249,16);      //50.0 us overflow, 800 us interrupt
  // setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   enable_interrupts(INT_TIMER2);
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_EXT1);
   enable_interrupts(INT_EXT2);
   enable_interrupts(GLOBAL);
   
   while(true)
   {
   output_high(pin_D3);
   delay_ms(10);
   output_low(pin_D3);
   delay_ms(10);
   }
}
