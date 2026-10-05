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
        P1 = 0x01;

        for(i = 0; i < 8; i++)
        {
            delay(100);
            P1 = P1 << 1;
        }

        P1 = 0x80;

        for(i = 0; i < 8; i++)
        {
            delay(100);
            P1 = P1 >> 1;
        }
    }
}