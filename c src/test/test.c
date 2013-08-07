#include <test.h>


void main()
{

while (true)
{
   output_A(0xff);
   output_B(0xff);
   output_C(0xff);
   output_D(0xff);
   output_E(0xff);/*
   output_high(pin_C1);
   output_high(pin_C0);
   output_high(pin_A4);*/
   Delay_ms(1000);
   output_A(0x0);
   output_B(0x0);
   output_C(0x0);
   output_D(0x0);
   output_E(0x0);/*
   output_low(pin_C1);
   output_low(pin_C0);
   output_low(pin_A4);*/
   Delay_ms(1000);
}

}
