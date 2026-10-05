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
    unsigned int s, z;

    while(1)
    {
        /* UP COUNTING */

        for(s = 0; s <= 9; s++)
        {
            for(z = 0; z <= 9; z++)
            {
                P1 = (s << 4) | z;
                delay(100);
            }
        }

        /* DOWN COUNTING */

        for(s = 9; s <= 9; s--)
        {
            for(z = 9; z <= 9; z--)
            {
                P1 = (s << 4) | z;
                delay(100);

                if(z == 0)
                    break;
            }

            if(s == 0)
                break;
        }
    }
}