/*
 * lcd.c
 *
 * Created: 8/17/2015 2:06:02 PM
 *  Author: MOSTAFA
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
/*============================== 4-bit LCD Functions ======================*/
/*
  Name:     lcd_init_4d
  Purpose:  initialize the LCD module for a 4-bit data interface
  Entry:    equates (LCD instructions) set up for the desired operation
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/
void lcd_init(void)
{
	// Power-up delay
    //vTaskDelay(MS_TO_TICK*40);                                 // initial 40 mSec delay
	
	/* configure the microprocessor pins for the data and control lines */
	
{
    lcd_D7_ddr |= (1<<lcd_D7_bit);                  // 8 data lines - output
    lcd_D6_ddr |= (1<<lcd_D6_bit);
    lcd_D5_ddr |= (1<<lcd_D5_bit);
    lcd_D4_ddr |= (1<<lcd_D4_bit);
	
	#if (!Four_bit_bus_mode)	
	lcd_D3_ddr |= (1<<lcd_D3_bit);                  // 4 data lines - output
    lcd_D2_ddr |= (1<<lcd_D2_bit);
    lcd_D1_ddr |= (1<<lcd_D1_bit);
    lcd_D0_ddr |= (1<<lcd_D0_bit);
	#endif

// configure the microprocessor pins for the control lines
    lcd_E_ddr  |= (1<<lcd_E_bit);                    // E line - output
    lcd_RS_ddr |= (1<<lcd_RS_bit);					 // RS line - output
	lcd_RW_ddr |=(1<<lcd_RW_bit);					 // RW line -output

/* IMPORTANT - At this point the LCD module is in the 8-bit mode and it is expecting to receive  
   8 bits of data, one bit on each of its 8 data lines, each time the 'E' line is pulsed.

 Since the LCD module is wired for the 4-bit mode, only the upper four data lines are connected to 
   the microprocessor and the lower four data lines are typically left open.  Therefore, when 
   the 'E' line is pulsed, the LCD controller will read whatever data has been set up on the upper 
   four data lines and the lower four data lines will be high (due to internal pull-up circuitry).

 Fortunately the 'FunctionReset' instruction does not care about what is on the lower four bits so  
   this instruction can be sent on just the four available data lines and it will be interpreted 
  properly by the LCD controller.  The 'lcd_write' subroutine will accomplish this if the 
   control lines have previously been configured properly.*/
// Set up the RS and E lines for the 'lcd_write' subroutine.
    lcd_RS_port &= ~(1<<lcd_RS_bit);                // select the Instruction Register (RS low )
    lcd_E_port |= (1<<lcd_E_bit);					// make sure E is initially high
	lcd_RW_port &=~(1<<lcd_RW_bit);					// select the Read Write Register (RW low )
}	

// 2-Function Set instruction
    lcd_write_instruction(lcd_FunctionSet);   // set mode, lines, and font
    //vTaskDelay(3);  
// 1-Turn off instruction
    lcd_write_instruction(lcd_Display_on_off_control(0));        // turn display OFF
    //vTaskDelay(1);                                  // 40uS delay (min)
	
  /*  lcd_write_4(lcd_FunctionSet);                 // second part of reset sequence
    vTaskDelay(MS_TO_TICK*1);                                 // 100uS delay (min)

    lcd_write_4(lcd_FunctionSet);                 // third part of reset sequence
    vTaskDelay(MS_TO_TICK*200);                     // this delay is omitted in the data sheet 

 Preliminary Function Set instruction - used only to set the 4-bit mode.
 The number of lines or the font cannot be set at this time since the controller is still in the
  8-bit mode, but the data transfer mode can be changed since this parameter is determined by one 
  of the upper four bits of the instruction.
 
    lcd_write_4(lcd_FunctionSet);               // set 4-bit mode
    vTaskDelay(MS_TO_TICK*40);                                  // 40uS delay (min) */
                     
/* The next three instructions are specified in the data sheet as part of the initialization routine, 
    so it is a good idea (but probably not necessary) to do them just as specified and then redo them 
    later if the application requires a different configuration.*/



	//lcd_write_instruction(lcd_SetCursor_possition);
// 3- Entry Mode Set instruction
    lcd_write_instruction(lcd_EntryMode_set);         // set desired shift characteristics
    //vTaskDelay(1);                                  // 40uS delay (min) 

/* This is the end of the LCD controller initialization as specified in the data sheet, but the display
  has been left in the OFF condition.  This is a good time to turn the display back ON.
  */
 
// 4-Turn on display instruction
    lcd_write_instruction(lcd_Display_on_off_control(1));         // turn the display ON
    //vTaskDelay(1);                                  // 40uS delay (min)
	
		// 5-Clear Display instruction
    lcd_write_instruction(lcd_Clear_display);             // clear display RAM
    _delay_ms(3); 
}

/*...........................................................................
  Name:     lcd_write_instruction
  Purpose:  send a byte of information to the LCD instruction register
  Entry:    (theInstruction) is the information to be sent to the instruction register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/
void lcd_write_instruction(unsigned char theInstruction)
{
	
    lcd_RS_port &= ~(1<<lcd_RS_bit);                // select the Instruction Register (RS low)
    lcd_E_port |= (1<<lcd_E_bit);                  // make sure E is initially high
    lcd_write(theInstruction);                    // write the upper 4-bits of the data
	#if (Four_bit_bus_mode)
		//_delay_ms(1); 
		lcd_write(theInstruction << 4);               // write the lower 4-bits of the data
	#endif
}

/*...........................................................................
  Name:     lcd_write_character_4d
  Purpose:  send a byte of information to the LCD data register
  Entry:    (theData) is the information to be sent to the data register
  Exit:     no parameters
  Notes:    chose between wait for busy flag or delay time 
*/
void lcd_write_character(unsigned char theData)
{
    lcd_RS_port |= (1<<lcd_RS_bit);                   // select the Data Register (RS high)
    lcd_E_port |= (1<<lcd_E_bit);                     // make sure E is initially high
    lcd_write(theData);                               // write the upper 4-bits of the data
 #if (Four_bit_bus_mode)
    lcd_write(theData << 4);                      // write the lower 4-bits of the data
 #endif
}

/*...........................................................................
  Name:     lcd_write_string_4d
; Purpose:  display a string of characters on the LCD
  Entry:    (theString) is the string to be displayed
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/
void lcd_write_string(unsigned char theString[])
{
    volatile int i = 0;                             // character counter*/
    while (theString[i] !=0)
    {
        lcd_write_character(theString[i++]);
        
        //vTaskDelay(1);                              // 40 uS delay (min)
    }
}

/*...........................................................................
  Name:     lcd_write_4
  Purpose:  send a byte of information to the LCD module
  Entry:    (theByte) is the information to be sent to the desired LCD register
            RS is configured for the desired LCD register
            E is low
            RW is low
  Exit:     no parameters
  Notes:    use either time delays or the busy flag
*/
void lcd_write(unsigned char theByte)
{
    lcd_D7_port &= ~(1<<lcd_D7_bit);                        // assume that data is '0'
    if (theByte & 1<<7) lcd_D7_port |= (1<<lcd_D7_bit);     // make data = '1' if necessary

    lcd_D6_port &= ~(1<<lcd_D6_bit);                        // repeat for each data bit
    if (theByte & 1<<6) lcd_D6_port |= (1<<lcd_D6_bit);

    lcd_D5_port &= ~(1<<lcd_D5_bit);
    if (theByte & 1<<5) lcd_D5_port |= (1<<lcd_D5_bit);

    lcd_D4_port &= ~(1<<lcd_D4_bit);
    if (theByte & 1<<4) lcd_D4_port |= (1<<lcd_D4_bit);
	
  #if (!Four_bit_bus_mode)
    lcd_D3_port &= ~(1<<lcd_D3_bit);                        // assume that data is '0'
    if (theByte & 1<<3) lcd_D3_port |= (1<<lcd_D3_bit);     // make data = '1' if necessary

    lcd_D2_port &= ~(1<<lcd_D2_bit);                        // repeat for each data bit
    if (theByte & 1<<2) lcd_D2_port |= (1<<lcd_D2_bit);

    lcd_D1_port &= ~(1<<lcd_D1_bit);
    if (theByte & 1<<1) lcd_D1_port |= (1<<lcd_D1_bit);

    lcd_D0_port &= ~(1<<lcd_D0_bit);
    if (theByte & 1<<0) lcd_D0_port |= (1<<lcd_D0_bit);
  #endif
// write the data
                                                    
    //vTaskDelay(1);                                 // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
    lcd_E_port &= ~(1<<lcd_E_bit);                   // Enable pin low
    //vTaskDelay(1);                                   // implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
    lcd_E_port |= (1<<lcd_E_bit);                    // Enable pin high
} 
void LCD_Init()
{
	 lcd_D7_ddr |= (1<<lcd_D7_bit);                  // 8 data lines - output
    lcd_D6_ddr |= (1<<lcd_D6_bit);
    lcd_D5_ddr |= (1<<lcd_D5_bit);
    lcd_D4_ddr |= (1<<lcd_D4_bit);
	
	#if (!Four_bit_bus_mode)	
	lcd_D3_ddr |= (1<<lcd_D3_bit);                  // 4 data lines - output
    lcd_D2_ddr |= (1<<lcd_D2_bit);
    lcd_D1_ddr |= (1<<lcd_D1_bit);
    lcd_D0_ddr |= (1<<lcd_D0_bit);
	#endif
	// configure the microprocessor pins for the control lines
    lcd_E_ddr  |= (1<<lcd_E_bit);                    // E line - output
    lcd_RS_ddr |= (1<<lcd_RS_bit);					 // RS line - output
	lcd_RW_ddr |=(1<<lcd_RW_bit);					 // RW line -output

/* IMPORTANT - At this point the LCD module is in the 8-bit mode and it is expecting to receive  
   8 bits of data, one bit on each of its 8 data lines, each time the 'E' line is pulsed.

 Since the LCD module is wired for the 4-bit mode, only the upper four data lines are connected to 
   the microprocessor and the lower four data lines are typically left open.  Therefore, when 
   the 'E' line is pulsed, the LCD controller will read whatever data has been set up on the upper 
   four data lines and the lower four data lines will be high (due to internal pull-up circuitry).

 Fortunately the 'FunctionReset' instruction does not care about what is on the lower four bits so  
   this instruction can be sent on just the four available data lines and it will be interpreted 
  properly by the LCD controller.  The 'lcd_write' subroutine will accomplish this if the 
   control lines have previously been configured properly.*/
// Set up the RS and E lines for the 'lcd_write' subroutine.
    lcd_RS_port &= ~(1<<lcd_RS_bit);                // select the Instruction Register (RS low )
    lcd_E_port |= (1<<lcd_E_bit);					// make sure E is initially high
	lcd_RW_port &=~(1<<lcd_RW_bit);					// select the Read Write Register (RW low )
	
	lcd_write_instruction(0x33);
	lcd_write_instruction(0x32);
	lcd_write_instruction(0x28);
	lcd_write_instruction(0x0c);
	lcd_write_instruction(0x06);
	lcd_write_instruction(0x01);
	_delay_ms(3);
}