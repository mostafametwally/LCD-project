/*
 * lcd_test2.c
 *
 * Created: 8/17/2015 2:21:09 PM
 *  Author: MOSTAFA
 */ 

#include <avr/io.h>

int main(void)
{
	//lcd_init();
	LCD_Init();
	while(1)
	{
       lcd_write_string("Hello world");
	   
	}    
}