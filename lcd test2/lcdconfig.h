/*
 * lcdconfig.h
 *
 * Created: 8/17/2015 2:06:28 PM
 *  Author: MOSTAFA
 */ 


#ifndef LCDCONFIG_H_
#define LCDCONFIG_H_


#include <avr/io.h>
// lcd connection port configuration

#define lcd_port				 PORTE	//chose lcd connection port
#define lcd_port_DDR			 DDRE
#define lcd_port_0				 PORTE0
#define lcd_port_1				 PORTE1
#define lcd_port_2				 PORTE2
#define lcd_port_3				 PORTE3
#define lcd_port_4				 PORTE4
#define lcd_port_5				 PORTE5
#define lcd_port_6				 PORTE6
#define lcd_port_7				 PORTE7

#define lcd_control_port		 PORTB
#define lcd_conrtol_DDR			 DDRB
#define lcd_RS_bit				 PORTB2
#define lcd_RW_bit				 PORTB1
#define lcd_E_bit				 PORTB0

#define Four_bit_bus_mode		 1 // 0 means 8 bit mode bus and 1 means 4 bit mode bus
#define enable_read_data_mode	 0  // 1 means enable read data from CGRAM and 0 to disable read data from CGRAM
#define display_font			 0  // 0 means 5x8 dot and 1 means 5x11 dot
#define display_line			 0  // 0 means 1 line display mode and 1 means 2 lines display mode

// 1-clear display instruction
#define clear_display		     0b00000001

// 2-return home instruction
#define return_home			     0b00000011

// 3-Entry mode set instruction
#define ID						 1		// 0 to move cursor/blink  to right 
#define SH						 0		// 1 to enable shift of entire  display according to ID value
#define EntryMode_set		     (1<<2|(SH_bit<<SH) |(ID_bit<<ID))
#define SH_bit					 0		
#define ID_bit					 1

// 4-display on/off control instruction

#define cursor_control			 0		//1 to turn on cursor		
#define blink					 0		//1 to turn on blinking
#define display_on_off_control(display)   (1<<3|(display<<display_bit)|(cursor_control<<cursor_control_bit)|(blink<<blink_bit) )
#define display_bit				 2
#define cursor_control_bit		 1
#define blink_bit				 0

// 5-cursor or display shift  instruction

#define CS						 0     //
#define RL						 1	   //right/left cursor control bit, 1 means shift cursor to right and 0 means shift cursor to left
#define cursor_or_display_shift  ((1<<4)|(CS<<CS_bit)|(RL<<RL_bit))
#define CS_bit					 3
#define RL_bit					 2

// 6-Lcd function set instruction
#if (display_font==0)
#define F						 0 // character format control bit, 0 means 5x8 dot and 1 means 5x11 dot
#else
#define F						 1 // character format control bit, 0 means 5x8 dot and 1 means 5x11 dot
#endif
#if (display_line==0)
#define N						 0 // line number control but, 0 means 1 line display mode set and 1 means 2 lines display mode set
#else
#define N						 1 // line number control but, 0 means 1 line display mode set and 1 means 2 lines display mode set
#endif
#if (Four_bit_bus_mode)
#define DL						 0 // data length control bit 1 means 8 bit bus mode 0 means 4 bit bus mode 
#else 
#define DL						 1 // data length control bit 1 means 8 bit bus mode 0 means 4 bit bus mode
#endif
#define F_bit					 2
#define N_bit					 3
#define DL_bit					 4
#define Function_set		     ((1<<5)|(F<<F_bit)|(N<<N_bit)|(DL<<DL_bit)|(1<<1)|(1<<0))


// 7-set CGRAM address instruction
#define Set_CGRAM_Address(CG_RAM_Address)		((1<<6)|(CG_RAM_Address))    //set AC0->AC5 to CGRAM Address

// 8-set DDRAM Address instruction
#define Set_DDRAM_Adress(DD_RAM_Address)		((1<<7)|(DD_RAM_Address))   //set AC0->AC6 to DDRAM Address

#endif /* LCDCONFIG_H_ */