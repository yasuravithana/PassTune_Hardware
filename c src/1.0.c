
/*
20MHz crystal
timer2 interrupts at 800us
int0,1,2 enabled
portD is output
A0 is the touch input
*/

/*usable RAM addresses starts from 0x019 variables declared in the code starts from here*/


#include <1.0.h>

#define inputDelay 100
#define touchTreshhold 0x1900

int32 readInt(void);
void writeInt(void);
void store(void);
void check(void);
void storePassword(void);
void checkPassword(void);

int32  t=0;
int count,hit,timerReset=0;
int16 ramStart=0x50;
int32 *ramNow;
int32 touch;

// Variable declarations for functions (mainly for EEPROM write/read functions)
int32 value;
int8 i,j;
byte addressEEPROM;
byte* addressRAM;



#int_TIMER2
void  TIMER2_isr(void) 
{
   if(timerReset==0)
      t++;
   else
   {
      t=0;
      timerReset=0;
      output_toggle(pin_D6);//test
   }
      
}

#int_EXT
void  EXT_isr(void) 
{
   if(READ_EEPROM(0x0)==0)
   {
      WRITE_EEPROM(0x0,1);
   }
   
   else
   {
      WRITE_EEPROM(0x0,0);
   }
   reset_cpu();
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
   //set_tris_D(0);// this created trouble when resetting the cpu
      
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(AN0);//to read the touch input
   set_adc_channel(0);
   
   delay_ms(500);
   
   setup_timer_2(T2_DIV_BY_1,249,16);      //50.0 us overflow, 800 us interrupt
   enable_interrupts(INT_TIMER2);
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_EXT1);
   enable_interrupts(INT_EXT2);
   enable_interrupts(GLOBAL);
   
   
   
   count=0;
   
   if(READ_EEPROM(0x0)==0)
   {
      storePassword();
   }
   
   else
   {
      checkPassword();
   }
  
}

void storePassword(void)
{
   output_high(pin_D4);//indicate store mode
   ramNow=ramStart;
   
   while(touch<touchTreshhold)
   {
   delay_us(10);
 //a small delay is required after setting the channel
 
 
 //and before read
 
touch=read_adc();
   output_high(pin_D7);//test
   }
   output_low(pin_D7);//test
   timerReset=1;
   hit=1;   
   delay_ms(inputDelay);//make sure no repititive inputs are taken
   while(true)
   {
   /*test
   output_toggle(pin_D7);//test
   delay_ms(500);//test
   */
   
   delay_us(10);
 //a small delay is required after setting the channel
 
 
 //and before read
 
touch=read_adc();
      if(((hit==0)&&(touch>touchTreshhold))||((hit==1)&&(touch<touchTreshhold)))
      {
         if(hit==1)
            hit=0;
         else
            hit=1;
         *ramNow=t;
         ramNow++;
         count++;
         timerReset=1;
         delay_ms(inputDelay);//make sure no repititive inputs are taken
      }
   
      if(input(pin_A1)==1)
      {
         store();
         output_high(pin_D7);//test
         break;
      }    
   }
}


// here the 'addressEEPROM is incremented within the' writeInt()' function.
void store(void)
{
   ramNow=ramStart;
   addressEEPROM=0x2;
   WRITE_EEPROM(0x1,count);
   DELAY_MS(8);
   for(j=0;j<count;j++)
   {
       value=*ramNow;
       writeInt();
       ramNow++;
   }
}


void checkPassword(void)
{
   
   output_high(pin_D5);//indicate check mode
   ramNow=ramStart;
   
   while(input(pin_A0)==0)
   {
   output_high(pin_D6);//test
   }
   output_low(pin_D6);//test
   timerReset=1;
   hit=1;
   while(true)
   {
   
   output_toggle(pin_D6);//test
   delay_ms(500);//test
      if((hit==0&&(input(pin_A0)==1))||(hit==1&&(input(pin_A0)==0)))
      {
         if(hit==1)
            hit=0;
         else
            hit=1;
         *ramNow=t;
         ramNow++;count++;
         timerReset=1;
      }
      
      if(input(pin_A1)==1)
      {      
         check();
         output_high(pin_D6);//test
         break;
      }    
   }
}


// here the 'addressEEPROM is incremented within the' writeInt()' function.
void check(void)
{
   ramNow=ramStart;
   addressEEPROM=0x2;
   if(READ_EEPROM(0x1)!=count)
   {
      //light the error led
      while(true)
      {
      }
   }
   for(j=0;j<count;j++)
   {
       readInt();
       if(value!=*ramNow)
       {
         //light the error led
         while(true)
         {
         }
       }
       ramNow++;
   }

}


// global variable 'addressEEPROM' should be set to the starting address of the int in EEPROM
int32 readInt(void)
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
void writeInt(void)
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
