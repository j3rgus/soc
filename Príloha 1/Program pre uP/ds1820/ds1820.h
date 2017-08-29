//*****************************************************************************
//									      *
// Meno súboru		: 'ds1820.h'					      *
// Popis		: Knižnica pre obsluhu DS1820			      *
// Autor		: Jerguš Lysý					      *
// Vytvorené		: 15.8.2009					      *
//									      *
//									      *
//*****************************************************************************

#ifndef DS_1820
#define DS_1820

#define uchar char
#define PORT (*(volatile unsigned char *)0x2B)
#define PIN  (*(volatile unsigned char *)0x29)
#define DDR  (*(volatile unsigned char *)0x2A)
#define DQ	0b10000000			/* Pin na ktorom je pripojeny senzor */

char one_reset(void);
char one_read_byte(void);
void one_write_byte(char data);
void send_convert(void);
void send_getTemp(void);
char refresh_temp(void);
//void delay_us(uchar us);

#include "ds1820.c"
#include <avr/io.h>
#endif