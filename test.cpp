#include <stdio.h>


int main()
{
   unsigned char reg = 0xF0;

   printf("%d\n",(reg>>7)&1);

   int gain_value =
        (((reg>>7)&1)+1) *
        (((reg>>6)&1)+1) *
        (((reg>>5)&1)+1) *
        (((reg>>4)&1)+1) *
        ((reg&0x0F)/16);


     printf("gain_value : %d\n", gain_value);
}
