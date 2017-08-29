//*****************************************************************************
//									      *
// Meno súboru		: 'ds1820.c'					      *
// Úèel			: Funkcie pre obsluhu DS1820			      *
// Autor		: Jerguš Lysý					      *
// Vytvorené		: 15.8.2009					      *
//									      *
//									      *
//*****************************************************************************

/* Oneskorenie pracuje spravne pri frekvencii 4MHz
   Pre inu frekvenciu treba funkciu upravit alebo pouzit kniznicu 
   WinAvr (util/delay.h)
*/

/*
   Funkcia:	 char one_reset(void)
   Tato funkcia vysle reset na slave.
   Vrati 0 ak sa senzor nenachadza na zbernici.
*/
#include <util/delay.h>

char one_reset(void)
{
  char r;
  asm("cli");
  DDR |= DQ;
  PORT &= ~DQ;
  _delay_us(500);
  DDR &= ~DQ;
  _delay_us(40);
  r = (DDR & DQ) ? 0 : 1;
  _delay_us(240);
  asm("sei");
  return r;
}

/*
   Funkcia:	 char one_read_byte(void)
   Funkcia cita byte zo senzoru
   Vrati nacitany byte
*/

char one_read_byte(void)
{
  char i, data = 0;
  asm("cli");
  for (i = 0; i < 8; i++) {
    data >>= 1;
    DDR |= DQ;
    PORT &= ~DQ;
    _delay_us(10);
    DDR &= ~DQ;
    _delay_us(10);
    data |= (PIN & DQ) ? 0b10000000 : 0;
    _delay_us(60);
  }   
  asm("sei");
  return data;
}

/*
   Funkcia:	 void one_write_byte(char data)
   Funkcia zapisuje byte do senzoru
   Posle byte 'data' na zbernicu
*/

void one_write_byte(char data)
{
  char i;
  asm("cli");
  for (i = 0; i < 8; i++) {
    DDR |= DQ;
    PORT &= ~DQ;
    if ((data & 1) == 0) {
      _delay_us(50);
      DDR &= ~DQ;
      _delay_us(10);
    }
    else {
      _delay_us(10);
      DDR &= ~DQ;
      _delay_us(50);
    }
    data >>= 1;
  }
  asm("sei");
}

/*
   Funkcia:	 void send_convert(void)
   Funkcia vysle prikaz pre konvertovanie
*/

void send_convert(void)
{
  one_write_byte(0xcc);
  one_write_byte(0x44);
}

/*
   Funkcia:	 void send_getTemp(void)
   Funkcia vysle prikaz pre obdrzanie teploty
   Pre DS1820 je nutne pockat min 400ms na skonvertovanie
*/

void send_getTemp(void)
{
  one_write_byte(0xcc);
  one_write_byte(0xbe);
}

/*char refresh_temp(void)
{
  char temp;
  asm("cli");
  one_reset();
  send_convert();
  //_delay_ms(400);
  one_reset();
  send_getTemp();
  temp = one_read_byte();
  one_reset();
  asm("sei");
  return (temp>>1);
}*/

/* Oneskorenie je 'us'*2, tj. max oneskorenie moze byt 510us !! */

/*void delay_us(char us)
{
  asm volatile (
  	"clr r16"	  "\n\t"
	"mov r17,%0"  "\n\t"
    "1:" 		  "\n\t"
	"inc r16"  	  "\n\t"
	"nop"		  "\n\t"
	"nop"		  "\n\t"
	"nop"		  "\n\t"
	"nop"		  "\n\t"
	"cp r16,r17"  "\n\t"
	"brne 1b"	  "\n\t"
	:
	: "r" (us)
  );
}
*/
