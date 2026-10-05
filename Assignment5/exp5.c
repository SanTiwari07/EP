#include <p18f4550.h>

#define RS PORTEbits.RE0
#define RW PORTEbits.RE1
#define EN PORTEbits.RE2

void delay(unsigned int time)
{
    unsigned int i, j;

    for(i = 0; i < time; i++)
        for(j = 0; j < 180; j++);
}

void Lcd_cmd(unsigned char command)
{
    PORTD = command;

    RS = 0;
    RW = 0;
    EN = 1;

    delay(250);

    EN = 0;
    delay(250);
}

void Lcd_init()
{
    Lcd_cmd(0x38);
    delay(100);

    Lcd_cmd(0x01);
    delay(100);

    Lcd_cmd(0x0E);
    delay(100);

    Lcd_cmd(0x06);
    delay(100);

    Lcd_cmd(0x80);
    delay(100);
}

void Lcd_data(unsigned char data)
{
    PORTD = data;

    RS = 1;
    RW = 0;
    EN = 1;

    delay(250);

    EN = 0;
    delay(250);
}

void main()
{
    unsigned char msg[] = "HELLO WORLD";
    unsigned char i;

    ADCON1 = 0x0F;
    TRISD = 0x00;
    TRISE = 0x00;

    Lcd_init();

    for(i = 0; i < 11; i++)
    {
        Lcd_data(msg[i]);
    }

    while(1);
}