//*****************************************************************************
//									      *
// Meno súboru		: 'ds1307.h'					      *
// Popis		: Knižnica pre obsluhu DS1307 - RTC		      *
// Autor		: Jerguš Lysý					      *
// Vytvorené		: 5.10.2009					      *
//									      *
//									      *
//*****************************************************************************

#ifndef DS_1307
#define DS_1307

#define i2c_PORT (*(volatile unsigned char *) 0x2B)
#define i2c_PIN  (*(volatile unsigned char *) 0x29)
#define i2c_DDR  (*(volatile unsigned char *) 0x2A)
#define SDA 5
#define SCL 6

char i2c_start(void);
void i2c_stop(void);
char i2c_write(char data);
char i2c_read(void);
void write_ds1307(char addr, char data);
char read_ds1307(char addr);

#include "ds1307.c"
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#endif