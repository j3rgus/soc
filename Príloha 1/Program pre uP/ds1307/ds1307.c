//*****************************************************************************
//									      *
// Meno súboru		: 'ds1307.c'					      *
// Popis		: Funkcie pre obsluhu DS1307 - RTC		      *
// Autor		: Jerguš Lysý					      *
// Vytvorené		: 5.10.2009					      *
//									      *
//									      *
//*****************************************************************************

/*
   Funkcia:	 char i2c_start(void)
   Funkcia nastartuje vysielanie
   Vrati 0 ak sa senzor nenachadza na zbernici.
*/

char i2c_start(void)
{
  asm("cli");
  i2c_DDR &= ~(1 << SCL);
  i2c_DDR &= ~(1 << SDA);
  asm("nop");
  asm("nop");
  if (bit_is_clear(i2c_PIN, SDA))
    return 0;
  if (bit_is_clear(i2c_PIN, SCL))
    return 0;
  _delay_us(5);
  i2c_DDR |= (1 << SDA);
  i2c_PORT &= ~(1 << SDA);
  _delay_us(5);
  i2c_DDR |= (1 << SCL);
  i2c_PORT &= ~(1 << SCL);
  asm("sei");
  return 1;
}

/*
   Funkcia:	 void i2c_stop(void)
   Funkcia ukonci vysielanie
   Nevrati ziadnu hodnotu a nepozaduje parametre
*/

void i2c_stop(void)
{
  asm("cli");
  i2c_DDR |= (1 << SDA);
  i2c_PORT &= ~(1 << SDA);
  i2c_DDR |= (1 << SCL);
  i2c_PORT &= ~(1 << SCL);
  _delay_us(10);
  i2c_DDR &= ~(1 << SCL);
  _delay_us(5);
  i2c_DDR &= ~(1 << SDA);
  asm("sei");
}

/*
   Funkcia:	 char i2c_write(char data)
   Vysle byte na slave
   Vrati 1 ak bol zaznamenany ACK od slave
*/

char i2c_write(char data)
{
  char i, res = 1;
  
  asm("cli");
  for (i = 0; i < 8; i++) {
    if ((data & 0x80) == 0) {
	  i2c_DDR |= (1 << SDA);
	  i2c_PORT &= ~(1 << SDA);
	}
	else
	  i2c_DDR &= ~(1 << SDA);
	
	_delay_us(10);
	i2c_DDR &= ~(1 << SCL);
	_delay_us(5);
	while (bit_is_clear(i2c_PIN, SCL))
	  ;
	_delay_us(5);
	i2c_DDR |= (1 << SCL);
	i2c_PORT &= ~(1 << SCL);
	data <<= 1;
  }
  i2c_DDR &= ~(1 << SDA);
  _delay_us(5);
  i2c_DDR &= ~(1 << SCL);
  _delay_us(10);
  if (bit_is_set(i2c_PIN, SDA))
    res = 0;
  i2c_DDR |= (1 << SCL);
  i2c_PORT &= ~(1 << SCL);
  asm("sei");
  return res;
}

/*
   Funkcia:	 char i2c_read(void)
   Funkcia nacita byte zo slave
   Vrati nacitane data
*/

char i2c_read(void)
{
  char i, data = 0;
  
  asm("cli");
  for (i = 0; i < 8; i++) {
    data <<= 1;
    i2c_DDR &= ~(1 << SCL);
	while (bit_is_clear(i2c_PIN, SCL))
	  ;
	_delay_us(5);
	if (bit_is_clear(i2c_PIN, SDA))
	  data |= 0;
	else data |= 1;
	_delay_us(10);
	i2c_DDR |= (1 << SCL);
	i2c_PORT &= ~(1 << SCL);
	_delay_us(10);
  }
  i2c_DDR &= ~(1 << SDA);
  _delay_us(5);
  i2c_DDR &= ~(1 << SCL);
  _delay_us(10);
  i2c_DDR |= (1 << SCL);
  i2c_PORT &= ~(1 << SCL);
  _delay_us(5);
  i2c_DDR &= ~(1 << SDA);
  _delay_us(5);
  asm("sei");
  return data;
}

/*
   Funkcia:	 void write_ds1307(char addr, char data)
   Funkcia zapise 'data' na adresu 'addr'
*/

void write_ds1307(char addr, char data)
{
 i2c_start();
 i2c_write(0xD0);
 i2c_write(addr);
 i2c_write(data);
 i2c_stop();
}

/*
   Funkcia:	 char read_ds1307(char addr)
   Funkcia nacita byte z adresy 'addr'
   Vrati nacitane byte
*/

char read_ds1307(char addr)
{       
 char data;
 i2c_start();
 i2c_write(0xD0);
 i2c_write(addr);
 i2c_start();
 i2c_write(0xD1);
 data = i2c_read();
 i2c_stop();
 return data;
}