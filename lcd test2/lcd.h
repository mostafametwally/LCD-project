/*
 * lcd.h
 *
 * Created: 8/17/2015 2:06:14 PM
 *  Author: MOSTAFA
 */ 


#ifndef LCD_H_
#define LCD_H_


#include "lcdconfig.h"
/****************************************************************************
    LCD-AVR-4d.c  - Use an HD44780U based LCD with an Atmel ATmega processor
 
    Copyright (C) 2013 Donald Weiman    (weimandn@alfredstate.edu)
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/****************************************************************************
         File:    LCD-AVR-4d.c
         Date:    September 16, 2013
 
       Target:    ATmega328
     Compiler:    avr-gcc (AVR Studio 6)
       Author:    Donald Weiman
 
      Summary:    4-bit data interface, busy flag not implemented.
                  Any LCD pin can be connected to any available I/O port.
                  Includes a simple write string routine.
 */
/******************************* Program Notes ******************************
 
            This program uses a 4-bit data interface but does not use the
              busy flag to determine when the LCD controller is ready.  The
              LCD RW line (pin 5) is not connected to the uP and it must be
              connected to GND for the program to function.
 
            All time delays are longer than those specified in most datasheets
              in order to accommodate slower than normal LCD modules.  This
              requirement is well documented but almost always ignored.  The
              information is in a note at the bottom of the right hand
              (Execution Time) column of the instruction set.
 
  ***************************************************************************
 
            The four data lines as well as the two control lines may be
              implemented on any available I/O pin of any port.  These are
              the connections used for this program:
 
                 -----------                   ----------
                | ATmega328 |                 |   LCD    |
                |           |                 |          |
                |        PD7|---------------->|D7        |
                |        PD6|---------------->|D6        |
                |        PD5|---------------->|D5        |
                |        PD4|---------------->|D4        |
                |           |                 |D3        |
                |           |                 |D2        |
                |           |                 |D1        |
                |           |                 |D0        |
                |           |                 |          |
                |        PF0|---------------->|E         |
                |        PF1|---------------->|RW        |
                |        PF2|---------------->|RS        |
                 -----------                   ----------
 
  **************************************************************************/

// LCD interface (should agree with the diagram above)

// LCD port and control bits connection

#define lcd_D7_port     lcd_port                   // lcd D7 connection
#define lcd_D7_bit      lcd_port_7
#define lcd_D7_ddr      lcd_port_DDR

#define lcd_D6_port     lcd_port                   // lcd D6 connection
#define lcd_D6_bit      lcd_port_6
#define lcd_D6_ddr      lcd_port_DDR

#define lcd_D5_port     lcd_port                   // lcd D5 connection
#define lcd_D5_bit      lcd_port_5
#define lcd_D5_ddr      lcd_port_DDR

#define lcd_D4_port     lcd_port                   // lcd D4 connection
#define lcd_D4_bit      lcd_port_4
#define lcd_D4_ddr      lcd_port_DDR

#define lcd_D3_port     lcd_port                   // lcd D3 connection
#define lcd_D3_bit      lcd_port_3
#define lcd_D3_ddr      DDRD

#define lcd_D2_port     lcd_port                   // lcd D2 connection
#define lcd_D2_bit      lcd_port_2
#define lcd_D2_ddr      lcd_port_DDR

#define lcd_D1_port     lcd_port                   // lcd D1 connection
#define lcd_D1_bit      lcd_port_1
#define lcd_D1_ddr      lcd_port_DDR

#define lcd_D0_port     lcd_port                   // lcd D0 connection
#define lcd_D0_bit      lcd_port_0
#define lcd_D0_ddr      lcd_port_DDR

#define lcd_RS_port     lcd_control_port                   // lcd Register Select pin
#define lcd_RS_ddr      lcd_conrtol_DDR

#define lcd_RW_port     lcd_control_port					   // lcd read write pin
#define lcd_RW_ddr	    lcd_conrtol_DDR

#define lcd_E_port      lcd_control_port                   // lcd Enable pin
#define lcd_E_ddr       lcd_conrtol_DDR

//*********************************************************************

// LCD module information

#define	  lcd_LineOne     0x00                      // start of line 1
#define   lcd_LineTwo     0x40                      // start of line 2
//#define   lcd_LineThree   0x14                  // start of line 3 (20x4)
//#define   lcd_lineFour    0x54                  // start of line 4 (20x4)
//#define   lcd_LineThree   0x10                  // start of line 3 (16x4)
//#define   lcd_lineFour    0x50                  // start of line 4 (16x4)

//*********************************************************************

// LCD initialization instructions
#define lcd_Clear_display				clear_display          // replace all characters with ASCII 'space'
#define lcd_return_home					return_home          // return cursor to first position on first line
//#define lcd_EntryMode					0b00000110          // shift cursor from left to right on read/write
#define lcd_EntryMode_set				EntryMode_set
#define lcd_Display_on_off_control      display_on_off_control          //display_on_off_control(0) turn display off and display_on_off_control(1) turn display on
//#define lcd_DisplayOn					0b00001100          // display on, cursor off, don't blink character
//#define lcd_FunctionReset				0b00100000          // reset the LCD
#define lcd_FunctionSet					Function_set          // 4 or 8-bit data, 1 or 2-line display, 5x8 or 5x11 font
#define lcd_SetCursor					Set_CGRAM_Address              // set cursor position
#define lcd_SetCursor_possition			Set_DDRAM_Adress(0x02)			  // set cursor position
//*************************************************
#define lcd_cursor_or_display_shift   cursor_or_display_shift


// Function Prototypes
void lcd_init(void);
void lcd_write_instruction(unsigned char);
void lcd_write(unsigned char);
void lcd_write_character(unsigned char);
void lcd_write_string(unsigned char *);

void LCD_Init(void);





#endif /* LCD_H_ */