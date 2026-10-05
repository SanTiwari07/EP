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
    unsigned int i;

    while(1)
    {
        for(i = 0; i <= 255; i++)
        {
            P1 = i;
            delay(100);
        }

        for(i = 255; i > 0; i--)
        {
            P1 = i;
            delay(100);
        }

        P1 = 0x00;
        delay(100);
    }
}