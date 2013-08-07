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

}

#int_EXT1
void  EXT1_isr(void) 
{

}

#int_EXT2
void  EXT2_isr(void) 
{

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

ramNow=ramStart;

while(input(pin_A0)==0)
{
}
t=0;
hit=1;count=0;
output_high(pin_D3);
WRITE_EEPROM(0x00,0x45);
while(true)
{
if((hit==0&&(input(pin_A0)==1))||(hit==1&&(input(pin_A0)==0)))
{
   //output_toggle(pin_D3);//test
   if(hit==1)
      hit=0;
   else
      hit=1;
   *ramNow=t;
   ramNow++;count++;
   t=0;
   //delay_ms(1000);
}

//testing
if(input(pin_A1)==1)
{
output_high(pin_D2);

   output_high(pin_D2);
   store();   
   break;
}
//testing ends
 
}
output_low(pin_D2);

}


// here the 'addressEEPROM is incremented within the' writeInt()' function.
void store()
{
   ramNow=ramStart;
   addressEEPROM=0x1;
   for(j=0;j<count;j++)
   {
       value=*ramNow;
       writeInt();
       ramNow++;
   }
}


// global variable 'addressEEPROM' should be set to the starting address of the int in EEPROM
int32 readInt()
{
   addressRAM = &value;
   for(i=0;i<4;i++)
   {
      *addressRAM = READ_EEPROM(addressEEPROM);
      addressRAM++;
      addressEEPROM++;
   }
   return value;
}

// global variable 'addressEEPROM' should be set to the starting address of the int in EEPROM and 'value' to the int to be written
void writeInt()
{
   addressRAM = &value;
   for(i=0;i<4;i++)
   {
      WRITE_EEPROM(addressEEPROM,*addressRAM);
      addressRAM++;
      addressEEPROM++;
      DELAY_MS(8);
   }
   
   
}
