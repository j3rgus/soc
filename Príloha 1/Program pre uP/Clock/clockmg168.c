#include <avr\io.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include "moje/clock.h"
#include "moje/clock.c"
#define var(x) #x 

void nastav_casovac0(char, char, char, char);
void nastav_casovac2(char, char, char, char);
void skenuj_alarm(void);
void citaj_hodn(void);
void delete_eeprom(void);
char *tst_monudaje(void);
char *tst_alarm(void);
char *tst_alres(void);
char *tst_com(void);
//void vykonaj(void);

//----------------------------------------------------------------------
ISR(TIMER0_OVF_vect) 		// Prerusenie kazdych 8,16ms
{
  TIMSK0 = 0;

  timer_ref++;
  if ((udaje.mon_temp) == 1 && ((stav & 2) != 2))  // skenuj monitorovanu teplotu
    skenuj_tep(); 
  if ((alarm.al_flag == 1) && ((stav & 2) != 2)) {	// skenuj alarm - nesnimaj pocas nastavovania
    skenuj_alarm();
  }	
  citaj_hodn();
  if ((timer_ref % 32) == 0) {
    if (cursor == 0)
	  LCDcursorOFF();
	if (al_res.pipa == 1) {
	  if (bit_is_set(PINC, 5)) {
	    PORTC &= ~8;
		al_res.pipa = 0;
	  }
	  if (++al_res.ref == 32) {
	    PORTC &= ~8;
		al_res.pipa = 0;
	  }
	} else al_res.ref = 0;
  }
  /* --- S⁄»ASç PRACOVN…HO REéÕMU, frekvencia v˝stupu --- */
  if (prac_stav == 1) {
  /* SPECIFICK… KRIT…RIA PRE DAN⁄ ⁄LOHU */
    alarm.al_flag = 0;
	alarm.clock = 0;
	alarm.temp = 0;

  /*------- KONIEC --------*/
  }
  if ((prac_stav == 1) && (freq_nastavena != 0)) {
    if (freq == 1)
	  PORTC |= 8;
	if (freq == 2)
	  PORTC ^= 8;
  } else PORTC &= ~8;
  /*------- KONIEC --------*/
  TIMSK0 = 1;
}
//----------------------------------------------------------------------
ISR(TIMER2_OVF_vect) 		// Prerusenie kazdych 32,64 ms
{
  //unsigned char vypis[40];
  unsigned char *check1;
  TIMSK2 = 0;
  timer2_ref++;

  if ((zmena == 1) && ((cak+10) == timer2_ref))
    zmena = 0;

  if (bit_is_set(PIND, 4) && (zmena == 0)) {
    podsv_dis ^= 1;
    PORTD ^= 4;
	zmena = 1;
	cak = (timer2_ref <= 245) ? timer2_ref : 0;
  }
  if ((timer2_ref % 250) == 0) {
    check1 = tst_monudaje();
    if (check1 != 0) {
      LCDclr();
	  strcpy(lcd_dis[0], "Opravena chyba v");
	  strcpy(lcd_dis[1], "                ");
	  strcpy(lcd_dis[1], check1);
	  LCDstringXY(0, 0, lcd_dis[0], 16);
      LCDstringXY(0, 1, lcd_dis[1], 16);
	  _delay_ms(1000);
    }
    //check1 = tst_udaje();
    if (check1 != 0) {
      LCDclr();
	  strcpy(lcd_dis[0], "Najdena chyba v ");
	  strcpy(lcd_dis[1], "                ");
	  strcpy(lcd_dis[1], check1);
	  LCDstringXY(0, 0, lcd_dis[0], 16);
      LCDstringXY(0, 1, lcd_dis[1], 16);
	  _delay_ms(1000);
    }
    tst_alarm();
    tst_alres();
    //tst_com();
  }
  
  TIMSK2 = 1;
}
//----------------------------------------------------------------------
ISR(USART_RX_vect)
{
  char byte, i, j, check;
  
  UCSR0B &= ~0x80;
  TIMSK0 = 0;
  byte = UDR0; 


//		OBSLUHA PRE PROGRAM
  if (byte == 104)			//otazka 104
    USART_Transmit(100);	//odpoved 100
  if (byte == 90)			//poziadavka cas 115
    com.PC = 1;
  if (byte == 106)
    com.PC = 0;
  if (byte == 115) {
    for (i = 0; i < 9; i++) {
	  USART_Transmit(p_udaje[i]);
	}
    for (i = 0; i < 8; i++) {
	  USART_Transmit(p_mon[i]);
	}
    for (i = 0; i < 8; i++) {
	  USART_Transmit(p_alarm[i]);
	}
	USART_Transmit((al_res.pipa | al_res.pipalo | al_res.ref));
	USART_Transmit(timer_ref);
	USART_Transmit(stav);
	USART_Transmit(cursor);
	for (i = 0; i < 2; i++) {
	  for (j = 0; j < 16; j++) {
	    USART_Transmit(lcd_dis[i][j]);
	  }
	} 
	USART_Transmit(udaje.teplota);
	USART_Transmit(timer2_ref);
	USART_Transmit(PORTD);
	for (i = 0; i < 8; i++) {
      EEPROM_CAKAJ;
      USART_Transmit(eeprom_read_byte(i));
    }
	USART_Transmit(prac_stav);
	USART_Transmit(freq);
	USART_Transmit(freq_nastavena);
  }
  if (byte == 116) {
    _delay_ms(1);
    if ((check = USART_Receive()) <= 0x59)
      write_ds1307(0, check);
	_delay_ms(1);
	if ((check = USART_Receive()) <= 0x59)  
	  write_ds1307(1, check);
	_delay_ms(1);
	if ((check = USART_Receive()) <= 0x23)
	  write_ds1307(2, check);
	_delay_ms(1);
  }
  if (byte == 117) {
    _delay_ms(1);
    if ((check = USART_Receive()) <= 0x99)
      write_ds1307(4, check);
	_delay_ms(1);
	if ((check = USART_Receive()) <= 0x12)
	  write_ds1307(5, check);
	_delay_ms(1);
	if ((check = USART_Receive()) <= 0x31)
	  write_ds1307(6, check);
	_delay_ms(1);
  }
  if (byte == 118) {
    for (i = 0; i < 7; i++) {
	  _delay_ms(1);
	  p_alarm[i] = USART_Receive();
	}
	udaje.mon_temp = USART_Receive();
	_delay_ms(1);
	alarm.al_den = USART_Receive();
	_delay_ms(1);
  }	
  if (byte == 120) {
    delete_eeprom();
  }
  // --- PRACOVN¡ »ASç ---
  if (byte == 121) {	/* POéIADAVKA PRE PRACOVN› STAV */
    prac_stav ^= 1;
  }
  if (byte == 95) {
  /* PRÕJEM POTREBN›CH ⁄DAJOV PRE DAN⁄ PR¡CU */  
    freq = USART_Receive();
	freq_nastavena = USART_Receive();
  }
  // --- KONIEC ---
  UCSR0B |= 0x80; 
  TIMSK0 = 1;
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
int main(void)
{
  unsigned char p;

  nastav_casovac0(0, 4, 0, 1);
  nastav_casovac2(0, 7, 0, 1);
  udaje.teplota = 84;
  USART_Init();
  LCDinit();
  LCDcursorOFF();
  CHbit_nuluj();			// pre istotu (pri "nekorektnom" vypnuti, tj. pocas konfiguracie casu - vid pozn.)  

  DDRD &= ~0x10;			//tlacitko pre podsvietenie
  DDRD |= 4;				//vystup pre podsvietenie displeja
  DDRC = 0x8;				//DDRC (vstup) 0,1,2,4,5 - tlacitka, (vystup) 3 - speaker
  PORTC &= ~0x8;			// PORTC 3 - log 0 na vystupe
  alarm.al_den = 1;

  for (p = 0; p < 8; p++) {
    EEPROM_CAKAJ;
    p_mon[p] = eeprom_read_byte(p);
  }
  
  asm("sei");

  while (1) {
    display1();
	if (prac_stav == 0) {
	  if (bit_is_set(PINC, 0))
	    konfig_hodiny();
	  if (bit_is_set(PINC, 1)) {
	    if (udaje.mon_temp == 1)
		  zobraz_mon_temp();
	  }
    }
  }
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void nastav_casovac0(char tcra, char tcrb, char tnt, char s)
{
  TCCR0A=tcra;
  TCCR0B=tcrb;
  TCNT0=tnt;
  TIMSK0=s;
}
//----------------------------------------------------------------------
void nastav_casovac2(char tcra, char tcrb, char tnt, char s)
{
  TCCR2A=tcra;
  TCCR2B=tcrb;
  TCNT2=tnt;
  TIMSK2=s;  
}
//----------------------------------------------------------------------
void skenuj_alarm(void)
{
  if (alarm.al_den == 10)
    goto pokr_al;
  if ((alarm.al_den <= 7) && (alarm.al_den == udaje.mDen))		// jeden den
    goto pokr_al;
  if ((alarm.al_den == 8) && (udaje.mDen <= 5))				// Po - Pia
    goto pokr_al;
  if ((alarm.al_den == 9) && (udaje.mDen >= 6))				// So - Ne
    goto pokr_al;
  
  return;
  pokr_al:
  if ((alarm.clock == 1) && (alarm.temp == 0)) {
    if ((alarm.al_hod == udaje.hod) && (alarm.al_min == udaje.min) && (alarm.al_sek == udaje.sek)) {
	  PORTC |= 8;
	  al_res.pipa = 1;
	}
  }
  if ((alarm.clock == 0) && (alarm.temp == 1)) {
    if (alarm.al_temp == udaje.teplota) {
	  PORTC |= 8;
	  alarm.temp = 0;
	  al_res.pipa = 1;
	}
  }
  if ((alarm.clock == 1) && (alarm.temp == 1)) {
    if (((alarm.al_hod == udaje.hod) && (alarm.al_min == udaje.min) && (alarm.al_sek == udaje.sek)) && (alarm.al_temp == udaje.teplota)) {
	  PORTC |= 8;
	  al_res.pipa = 1;
	}
  }
}
//----------------------------------------------------------------------
void citaj_hodn(void)
{
  if ((stav & 1) != 1) {
    one_reset();
    send_convert();
    for (unsigned char i = 0; i < 7; i++)		//nacitam vsetky udaje do struktury - v priebehu cakania
      p_udaje[i] = read_ds1307(i);
    zisti_den();								//zistim den v tyzdni - v priebehu cakania (konvertovanie)
	stav |= 1;						// nastavim stav na 1 pre dokoncenie 1. casti
  }
  if (((stav & 1) == 1) && ((timer_ref % 50) == 0)) {
    one_reset();
    send_getTemp();
    udaje.teplota = one_read_byte();
    udaje.temp_flag = one_read_byte();			/// DOKONCIT NASTAV5, Fcia PRE DNI ALARMU
    one_reset();
    //if ((udaje.temp_flag = 0))
    udaje.teplota >>= 1;
    //else 
      //udaje.teplota = -(((~udaje.teplota)/2)+1);
    stav &= ~1;
  }
}
//----------------------------------------------------------------------
void delete_eeprom(void)
{
  int e;

  for (e = 0; e < 8; e++) {
    if (e != 4) {
	  EEPROM_CAKAJ;
	  eeprom_write_byte(e,255);
	}
	else {
	  EEPROM_CAKAJ;
	  eeprom_write_byte(4,0);
	}
  }

}
//----------------------------------------------------------------------
char *tst_monudaje(void)
{
  if (eeprom_read_byte(4) > 0x70) {
    EEPROM_CAKAJ;
	eeprom_write_byte(4,0);
	mon_udaje.max_temp = 0;
	return var(mon_udaje.max_temp);
  }
  return 0;
}
//----------------------------------------------------------------------
char *tst_alarm(void)
{
  if (alarm.temp > 1) {
    alarm.temp = 0;
  }
  if (alarm.clock > 1) {
    alarm.clock = 0;
  }
  if (alarm.al_flag > 1) {
    alarm.al_flag = 0;
  }
  if (alarm.al_den > 10) {
    alarm.al_den = 1;
  }
  return 0;
}
//----------------------------------------------------------------------
char *tst_alres(void)
{
  if (al_res.pipa > 1) {
    al_res.pipa = 0;
  }
  return 0;
}
//----------------------------------------------------------------------
char *tst_com(void)
{
  return 0;
}
//----------------------------------------------------------------------
