//*****************************************************************************
//									      *
// Meno súboru		: 'clock.h'					      *
// Popis		: Knižnica výhradne pre DIGITAL CLOCK SYSTEM			      *
// Autor		: Jerguš Lysý					      *
// Vytvorené		: 24.12.2009					      *
//									      *
//									      *
//*****************************************************************************


#ifndef Clock
#define Clock

#define EEPROM_CAKAJ while (!eeprom_is_ready()) 
#define STR 6;		// hodnota pre 21. storocie (pri zistovani dna v tyzdni)


void display1(void);					// Zobrazenie casu a teploty
void zisti_den(void);
void zobraz_mon_temp(void);
void skenuj_tep(void);					//funkcia v casovaci
void cakaj(char p);						// Caka kym je nastaveny bit p v PINC
char decToBcd(char d);
char bcdToDec(char b);
void CHbit_nastav(void);				// Stopni hodiny (DS1307)
void CHbit_nuluj(void);					// Spusti hodiny
void refresh_udaje(void);				// Obnovenie teploty a casu
void nastav(char [], char, char);
char nastav_cas(void);
char nastav_datum(void);
char nastav_extra(void);
char nastav_ukoncit(void);
void konfig_hodiny(void);
void nastav1(void);			// Nastavovanie ponuky snimaca teploty
void nastav2(void);			// Nastavenie alarmu - vseobecne
void nastav3(void);			// Nastavenie alarmu - cas
void nastav4(void);			// Nastavenie alarmu - teplota
void nastav5(void);			// V pripade nastavenia alarmu nastav den
void LCDstringXY(char X, char Y, char *str, char pct);		// vypis retazca str na poziciu X, Y o velkosti pct
void LCDcistiXY(char x, char y, char pct);					// vymaz z pozicie X, Y pct znakov
void USART_Transmit(char ch);		// Prenos bajtu USART-om
char USART_Receive(void);				// Prijem bajtu USART-om
void USART_Init(void);					// Inicializacia USART-u
void ansi_cl(void);
void ansi_cm(unsigned char row,unsigned char col);
//eeprom_write_byte(addr,data)
//eeprom_read_byte(addr)

struct {
	char teplota;
	char sek;
	char min;
	char hod;
	char mDen;
	char den;
	char mes;
	char rok;
	char temp_flag;	//  +-°C
	char mon_temp;	// Priznak ne/monitorovania teploty -- nastav1() --
} udaje;

struct {
	char min_temp;		// 0 adr v E2PROM
	char min_den;
	char min_mes;
	char min_rok;
	char max_temp;
	char max_den;
	char max_mes;
	char max_rok;		// 7 adr v E2PROM
} mon_udaje;

struct {
	char al_hod;
	char al_min;
	char al_sek;
	char al_temp;
	char temp;			// Priznak nastavenia teoplty -- nastav4() --
	char clock;			// Priznak nastavenia casu  -- nastav3() --
	char al_flag;		// Priznak - vseobecne nastavenie alarmu -- nastav2() --
	char al_den;
} alarm;

struct {
	unsigned pipa	: 1;
	unsigned pipalo	: 1;
	unsigned ref	: 6;
} al_res;

struct {
	unsigned char PC  :1;
	unsigned char dolava;
	unsigned char doprava;
	unsigned char hore;
	unsigned char enter;
} com;

// 2 - pri alarme, nesmimat pocas konfigu (pouzite bity 0,1,2)
unsigned char timer_ref = 0, timer2_ref = 0, zmena = 0, stav = 0, cursor, podsv_dis = 0, cak = 0;	
char *p_mon = &mon_udaje.min_temp;		// Prva hodnota struktury mon_udaje (pole)
char *p_alarm = &alarm.al_hod;			// Pointery pouzite pre ulahcenie prace (v poli)
char *p_udaje = &udaje.sek;	// Ukazatel na strukturu udaje
char *dni[] = {"Po ", "Ut ", "Str", "Stv", "Pia", "So ", "Ne "};
char lcd_dis[2][17], val, konf[16];
char *str_pis[] = {"Sek: %02x (0-%x)  ","Min: %02x (0-%x) >", "Hod: %02x (0-%x) >", "Den: %02x (1-%x)  ", "Mes: %02x (1-%x) >", "Rok: %02x (0-%x) >"};	// retazce pri zobrazene pri konfigu (dalej potom str_ud)
char pct_dni[] = {0x31, 0x28, 0x31, 0x30, 0x31, 0x30, 0x31, 0x31, 0x30, 0x31, 0x30, 0x31};
char mon[] = {0,3,3,6,1,4,6,2,5,0,3,5};
char (*p[4])() = {nastav_cas, nastav_datum, nastav_extra, nastav_ukoncit};
char *menu[4] = {"< Nastavit Cas >", "<Nastavit Datum>", "<   Dalsie     >", "<   Ukoncit    >"};

#include <avr/eeprom.h>
#include "ds1820\ds1820.h"
#include "ds1307\ds1307.h"
#include "lcd/lcd_lib.h"
#include <stdio.h>
#include <string.h>


#endif
