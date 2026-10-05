#include <reg51.h>

void delay(unsigned int time)
{
    unsigned int i, j;

    for(i = 0; i < time; i++)
    {
        for(j = 0; j < 1275; j++);
    }
}

void main(void)
{
    while(1)
    {
        P1 = 0x00;
        delay(100);

        P1 = 0xFF;
        delay(100);
    }
}