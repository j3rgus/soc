//*****************************************************************************
//									      *
// Meno súboru		: 'clock.c'					      *
// Úèel			: Funkcie výhradne pre DIGITAL CLOCK SYSTEM			      *
// Autor		: Jerguš Lysý					      *
// Vytvorené		: 24.12.2009					      *
//									      *
//									      *
//*****************************************************************************

#ifndef Clock_F
#define Clock_F

//----------------------------------------------------------------------
void display1(void)					//samotne vypisanie (stav po spusteni)
{
  LCDhome();
  sprintf(lcd_dis[0], "%2x:%02x:%02x%+6d", udaje.hod, udaje.min, udaje.sek, ((udaje.teplota == 85) || (udaje.teplota == 84) || (udaje.teplota == 127)) ? 0 : udaje.teplota);				//prichystane na vypis
  sprintf(lcd_dis[1], "%3s  %2x.%2x. 20%02x", dni[udaje.mDen - 1], udaje.den, udaje.mes, udaje.rok);	//taktiez vyplnim lcd_dis pole ktore je
  LCDstring(lcd_dis[0], strlen(lcd_dis[0]));
  LCDsendChar(0xdf);LCDsendChar('C');
  LCDGotoXY(0,1);
  LCDstring(lcd_dis[1], strlen(lcd_dis[1]));
}
//----------------------------------------------------------------------
void nastav(char str_ud[], char val, char addr)		// (hodnota zo struktury, prislusny retazec, max zobrazena hodnota, adresa (na zapis))
{
  char p_udaj;
  p_udaj = (addr != 0) ? p_udaje[addr] : p_udaje[addr]&0x7f;
  if (addr == 4)													// Ak nastavujem den 
    if (p_udaj > pct_dni[bcdToDec(udaje.mes)-1]) {				// zisti ci je pocet dni vacsi ako celkovy pocet v danom mesiaci
	  p_udaj = pct_dni[bcdToDec(udaje.mes)-1];						// pokial je vacsi nastav max. pocet dni a
	  if (((bcdToDec(udaje.rok) % 4) == 0) && (udaje.mes == 2)) 	// pokial je februar a priestupny rok tak este zvys den
	    p_udaj++;												// jedna sa o strukturu (nie o zobrazovacie hodnoty)
	}
  while (1) {
  	LCDGotoXY(0,1);
    sprintf(lcd_dis[1], str_ud, p_udaj, (addr == 0) ? val&0x7f : val);	// vyplnenie str_pis (o max hodnoty val-pom)
	LCDstring(lcd_dis[1], 16);											//16 znakov kvoli >
	if (bit_is_set(PINC, 1)) {										// zvacsovanie hodnoty pri stlacani tlacitka 1
	  if (p_udaj < val) {									
	    p_udaj++;
	    if ((bcdToDec(p_udaj) % 10) == 0)							//osetrenie hex cislic na BCD (vynechanie pismen pricitanim 6)
		  p_udaj += 6;	
	  } else p_udaj = ((addr == 5)||(addr == 4)) ? 1 : 0;		//pokial je Den a Mesiac nastav min na 1 (inak zacinaj od nuly)
	  _delay_ms(100);
	}
	if (bit_is_set(PINC, 2)) {
	  if (p_udaj > ((addr == 4) || (addr == 5)) ? 1 : 0) {
	    if ((bcdToDec(p_udaj--) % 10) == 0)
		  p_udaj -= 6;
	  } else p_udaj = val;
	  _delay_ms(100);
	}
	if (bit_is_set(PINC, 0)) {										//pri stlaceni tlacitka 0 uloz hodnoty do DS1307
	  write_ds1307(addr, p_udaj);
	  if (addr == 5)
	    udaje.mes = read_ds1307(addr);
	  cakaj(0);
	  break;
	}
	if (bit_is_set(PINC, 4)) {
	  cakaj(4);
	  return;
	}
  }
}
//----------------------------------------------------------------------
void zobraz_mon_temp(void)
{
  unsigned char i;  

  LCDclr();
  cakaj(1);
  
  for (;;) {
    LCDhome();
    sprintf(lcd_dis[0], "MIN:%+2d\xdf", mon_udaje.min_temp);	// pre zobrazenie monitorovanej teploty pouzit lcd_dis
    sprintf(lcd_dis[1], "MAX:%+2d\xdf", mon_udaje.max_temp);
    LCDstring(lcd_dis[0], strlen(lcd_dis[0]));LCDsendChar('C');
    LCDGotoXY(0,1);
    LCDstring(lcd_dis[1], strlen(lcd_dis[1]));LCDsendChar('C');
	if (bit_is_set(PINC, 1)) {									// pri stlaceni tlacitka 1 vrat sa spat na default zobrazovanie
	  LCDclr();
	  cakaj(1);
	  break;
	}
	if (bit_is_set(PINC, 0)) {									// pri stlaceni tlacitka 0 uloz zaznam do EEPROM
	  for (i = 0; i < 8; i++) {
	    EEPROM_CAKAJ;
		eeprom_write_byte(i,p_mon[i]);
	  }
	  LCDclr();
	  strcpy(lcd_dis[0], "Hodnoty ulozene ");
	  strcpy(lcd_dis[1], "v pamati        ");
	  LCDstringXY(0, 0, lcd_dis[0], 16);
	  LCDstringXY(0, 1, lcd_dis[1], 16);
	  while (1) {
	    if (bit_is_set(PINC, 0)) {
		  cakaj(0);
		  break;
		}  
	  }
	  LCDclr();
	}
  }
}
//----------------------------------------------------------------------
void CHbit_nastav(void)
{
  udaje.sek = read_ds1307(0);
  write_ds1307(0, (udaje.sek|0x80));
}
//----------------------------------------------------------------------
void CHbit_nuluj(void)
{
  udaje.sek = read_ds1307(0);
  write_ds1307(0, (udaje.sek&0x7f));
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void cakaj(char p)
{
  while (bit_is_set(PINC, p)) ;
}
//----------------------------------------------------------------------
char decToBcd(char d)
{
  return (((d/10)<<4) + (d%10));
}
//----------------------------------------------------------------------
char bcdToDec(char b)
{
  return (((b>>4)*10) + ((b&0x0f)));
}
//----------------------------------------------------------------------
void zisti_den(void)				//zisti den 
{
  char den, mesiac, rok, sum = 0;
									// POSTUP
  den = bcdToDec(udaje.den);		// vsetky hodnoty na dekadicke
  mesiac = bcdToDec(udaje.mes);
  rok = bcdToDec(udaje.rok);
  sum += (den % 7);
  sum += mon[mesiac-1];
  sum += ((rok + (rok / 4)) % 7);
  sum += STR;
  if ((sum %= 7) == 0)
    sum = 7;
  if ((rok % 4) == 0) {
    if ((mesiac == 1) || (mesiac == 2))
      if ((sum += 6) > 7)
	    sum -= 7;
  }
  udaje.mDen = sum;
}
//----------------------------------------------------------------------
void konfig_hodiny(void)
{ 
  char i;

  LCDclr();
  cakaj(0);
  LCDhome();
  strcpy(lcd_dis[0], "  Hlavne menu   ");	//doplnene
  LCDstring(lcd_dis[0], 16);				// prvy riadok vypis Hlavne menu
  strcpy(lcd_dis[1], menu[0]);			//doplnene
  LCDstringXY(0, 1, lcd_dis[1], 16);					// na druhy riadok vypis prvu polozku
  for (i = 0; i < 4;) {
	if (bit_is_set(PINC, 1)) {						//tlacitko 1 zvys i (i = aktualna polozka)
	  cakaj(1);
	  if (++i > 3)
	    i = 0;
	  LCDcistiXY(1, 1, 16);
	  _delay_ms(50);
	  strcpy(lcd_dis[1], menu[i]);			//doplnene
  	  LCDstringXY(1, 1, &menu[i][1], 16);
	}
	if (bit_is_set(PINC, 2)) {
	  cakaj(2);
	  if (--i > 3)
	    i = 3;
	  LCDcistiXY(0, 1, 15);
	  _delay_ms(50);
	  strcpy(lcd_dis[1], menu[i]);			//doplnene
  	  LCDstringXY(0, 1, lcd_dis[1], 15);
	}
	if (bit_is_set(PINC, 0)) {
	  cakaj(0);
	  LCDclr();
	  if (i < 2) { 
	    strcpy(lcd_dis[0], menu[i]);				//doplnene
	    LCDstringXY(0, 0, &menu[i][1], 14); 
	  }
	  if (p[i]() != 0) break;
	  strcpy(lcd_dis[0], "  Hlavne menu   ");		//doplnene
  	  LCDstringXY(0, 0, lcd_dis[0], 16);
	  strcpy(lcd_dis[1], menu[i]);
	  LCDstringXY(0, 1, lcd_dis[1], 16);
	}
	if (bit_is_set(PINC, 4)) {
	  cakaj(4);
	  return;
	}
  }
}
//----------------------------------------------------------------------
void nastav1(void)			
{
  char mon_temp;
  
  cakaj(0);
  mon_temp = udaje.mon_temp;
  LCDclr();
  LCDhome();
  strcpy(lcd_dis[0], "Snimat teplotu >");	//doplnene
  LCDstring(lcd_dis[0], 16);				// zacinam nastavenim sniania teploty
  while (1) {									// co je funkcia nastav1
    LCDGotoXY(0,1);								// tato funkcia je spustena funkciou nastav_extra()
    if (mon_temp == 0) {					// a ta je spustena ukazatelom podla prislusnej volby
	  strcpy(lcd_dis[1], "Vypnute");		//doplnene
      LCDstring(lcd_dis[1], 7);
	}
    else {
	  strcpy(lcd_dis[1],"Zapnute"); 		//doplnene
	  LCDstring(lcd_dis[1], 7);
	}
	if (bit_is_set(PINC, 1)) {	
	  cakaj(1);
	  mon_temp ^= 1;
	  LCDcistiXY(0, 1, 7);
	  _delay_ms(100);
	}
	if (bit_is_set(PINC, 2)) {
	  cakaj(2);
	  mon_temp ^= 1;
	  LCDcistiXY(0, 1, 7);
	  _delay_ms(100);
	}
	if (bit_is_set(PINC, 0)) {
	  cakaj(0);
	  udaje.mon_temp = mon_temp;
	  break;
	}
	if (bit_is_set(PINC, 4)) {
	  cakaj(4);
	  return;
	}
  }
}
//----------------------------------------------------------------------
void nastav2(void)		// nastavenie ALARMU (ako celku)
{
  char al_flag;

  cakaj(0);
  al_flag = alarm.al_flag;
  LCDclr();
  LCDhome();
  strcpy(lcd_dis[0], "Nastavit Alarm  ");	//doplnene
  LCDstring(lcd_dis[0], 16);				// funkcia nastav2() je spustena funkciou nastav1()
  while (1) {								// v tejto funkcii nastavujem ON/OFF alarm
    LCDGotoXY(0,1);							// po nastaveni ON funkcia automaticky skoci na nastav3()
	if (al_flag == 0) {					// inak neskoci nikam a vrati sa do nastav1() ktora nasledne skonci
	  strcpy(lcd_dis[1], "Vypnute");	//doplnene
	  LCDstring(lcd_dis[1], 7);
	}
	else {
	  strcpy(lcd_dis[1], "Zapnute");	//doplnene
	  LCDstring(lcd_dis[1], 7);
	}
	if (bit_is_set(PINC, 1)) {
	  cakaj(1);
	  al_flag ^= 1;
	  LCDcistiXY(0, 1, 7);
	  _delay_ms(80);
	}
	if (bit_is_set(PINC, 2)) {
	  cakaj(2);
	  al_flag ^= 1;
	  LCDcistiXY(0, 1, 7);
	  _delay_ms(80);
	}
	if (bit_is_set(PINC, 0)) {
	  alarm.al_flag = al_flag;
	  break;
	}
	if (bit_is_set(PINC, 4)) {
	  cakaj(4);
	  return;
	}
  }
}
//----------------------------------------------------------------------
void nastav3(void)
{
  unsigned char i, _clock;
  char _alarm[3];

  cakaj(0);
  _clock = alarm.clock;
  LCDclr();
  strcpy(lcd_dis[0], "Cas: ");		//doplnene
  strcpy(lcd_dis[1], "               ");
  LCDstringXY(0,0,lcd_dis[0], 5);		// tato funkcia je volana z funkcie nastav2(), sluzi pre nastavenie alarmu - casu
  while (1) {
    if (_clock == 0) {
	  strcpy(lcd_dis[0], "Cas: Vypnute");	//doplnene
      LCDstringXY(0,0,lcd_dis[0], 12);
	}
    else { 
	  strcpy(lcd_dis[0], "Cas: Zapnute");	//doplnene
	  LCDstringXY(0,0,lcd_dis[0], 12); 
	}
	if (bit_is_set(PINC, 1)) {
	  cakaj(1);
	  _clock ^= 1;					// zakazat/povolit alram pre cas
	  LCDcistiXY(5, 0, 7);
	  _delay_ms(80);
	}
	if (bit_is_set(PINC, 2)) {
	  cakaj(2);
	  _clock ^= 1;	
	  LCDcistiXY(5, 0, 7);
	  _delay_ms(80);
	}
	if (bit_is_set(PINC, 0)) {
	  cakaj(0);
	  break;
	}
	if (bit_is_set(PINC, 4)) {
	  cakaj(4);
	  return;
	}
  }
  if (_clock != 0) {
    for (i = 0; i < 3; i++)
	  _alarm[i] = p_alarm[i];
    sprintf(lcd_dis[1], "%02x:%02x:%02x", _alarm[0], _alarm[1], _alarm[2]);		//doplnene
	LCDstringXY(0, 1, lcd_dis[1], 8);
	for (i = 0; i < 3;) {
	  if (bit_is_set(PINC, 1)) {
	    if ((_alarm[i]) < ((i == 0) ? 0x23 : 0x59)) {
		  (_alarm[i])++;
		  if ((bcdToDec(_alarm[i]) % 10) == 0)
		    (_alarm[i]) += 6;
		} else _alarm[i] = 0;
		LCDcursorOFF(); cursor = 0;
		sprintf(lcd_dis[1], "%02x:%02x:%02x", _alarm[0], _alarm[1], _alarm[2]);	//doplnene
		LCDstringXY(0, 1, lcd_dis[1], 8);
		_delay_ms(100);
	  }
	  if (bit_is_set(PINC, 2)) {
	    if (_alarm[i] > 0) {
		  if ((bcdToDec(_alarm[i]--) % 10) == 0)
		    (_alarm[i]) -= 6;
		} else { _alarm[i] = (i == 0) ? 0x23 : 0x59;}
	    LCDcursorOFF(); cursor = 0;
		sprintf(lcd_dis[1], "%02x:%02x:%02x", _alarm[0], _alarm[1], _alarm[2]);	//doplnene
		LCDstringXY(0, 1, lcd_dis[1], 8);
		_delay_ms(100);
	  }
	  LCDcursorOnBlink(); cursor = 1;
	  LCDGotoXY(1+(i*3),1);

	  if (bit_is_set(PINC, 0)) {
	    cakaj(0);
		i++;
	  }
	  if (bit_is_set(PINC, 4)) {
  	    cakaj(4);
		LCDcursorOFF(); cursor = 0;
		return;
	  }
	}
	for (i = 0; i < 3; i++)
	  p_alarm[i] = _alarm[i];
  }
  alarm.clock = _clock;
  LCDcursorOFF();cursor = 0;
}
//----------------------------------------------------------------------
void nastav4(void)
{
  char _temp, _altemp;

  _temp = alarm.temp;
  LCDclr();
  strcpy(lcd_dis[0], "Teplota: ");			//doplnene
  strcpy(lcd_dis[1], "                ");
  LCDstringXY(0, 0, lcd_dis[0], 9);
  while (1) {
    if (_temp == 0) {
	  strcpy(lcd_dis[0], "Teplota: Vypnute");	//doplnene
	  LCDstringXY(0, 0, lcd_dis[0], 16);
	}
    else {
	  strcpy(lcd_dis[0], "Teplota: Zapnute");	//doplnene
	  LCDstringXY(0, 0, lcd_dis[0], 16); 
	}
    if (bit_is_set(PINC, 1)) {
      cakaj(1);
	  _temp ^= 1;				// zakazat/povolit alram pre teplotu
	  LCDcistiXY(9, 0, 7);
	  _delay_ms(80);
    }
	if (bit_is_set(PINC, 2)) {
      cakaj(2);
	  _temp ^= 1;				// zakazat/povolit alram pre teplotu
	  LCDcistiXY(9, 0, 7);
	  _delay_ms(80);
    }
    if (bit_is_set(PINC, 0)) {
      cakaj(0);
	  break;
    }
	if (bit_is_set(PINC, 4)) {
	  cakaj(4);
	  return;
	}
  }
  if (_temp != 0) {
    _altemp = p_alarm[3];
    sprintf(lcd_dis[1], "%02d\xdf", _altemp);			//doplnene
    LCDstringXY(0, 1, lcd_dis[1], 3); LCDsendChar('C');
    while (1) {
	  if (bit_is_set(PINC, 1)) {
	    if (_altemp < 70)
	      _altemp++;
	    else _altemp = 0;
	    sprintf(lcd_dis[1], "%02d\xdf", _altemp);		//doplnene
  	    LCDstringXY(0, 1, lcd_dis[1], 3); LCDsendChar('C');
	    _delay_ms(100);
	  }
	  if (bit_is_set(PINC, 2)) {
	    if (_altemp > 0)
	      _altemp--;
	    else _altemp = 70;
	    sprintf(lcd_dis[1], "%02d\xdf", _altemp);		//doplnene
  	    LCDstringXY(0, 1, lcd_dis[1], 3); LCDsendChar('C');
	    _delay_ms(100);
	  }
	  if (bit_is_set(PINC, 0)) {
	    cakaj(0);
	    break;
	  }
	  if (bit_is_set(PINC, 4)) {
	    cakaj(4);
		return;
	  }
    }
	p_alarm[3] = _altemp;
  }
  alarm.temp = _temp;
}
//----------------------------------------------------------------------
void nastav5(void)	
{
  char _aden, j;
  char *al_dni[] = {"Pondelok", "Utorok  ", "Streda  ", "Stvrtok ", "Piatok  ", "Sobota  ", "Nedela  ", "Po - Pia", "So - Ne ", "Po - Ne "};

  cakaj(0);
  _aden = alarm.al_den;					
  LCDclr();
  strcpy(lcd_dis[0], "  Nastavit den");
  strcpy(lcd_dis[1], al_dni[_aden-1]);
  LCDstringXY(0, 0, lcd_dis[0], 14);
  LCDstringXY(0, 1, lcd_dis[1], 8);
  for (;;) {
    strcpy(lcd_dis[1], al_dni[_aden-1]);
    LCDstringXY(0, 1, lcd_dis[1], 8);	
    if (bit_is_set(PINC, 1)) {
	  cakaj(1);
	  if (_aden < 10)
	    _aden++;
	  else _aden = 1;
	  LCDcistiXY(0, 1, 8);
	}
	if (bit_is_set(PINC, 2)) {
	  cakaj(2);
	  if (_aden > 1)
	    _aden--;
	  else _aden = 10;
	  LCDcistiXY(0, 1, 8);
	}
	if (bit_is_set(PINC, 0)) {
	  cakaj(0);
	  alarm.al_den = _aden;
	  break;
	}
	if (bit_is_set(PINC, 4)) {
	  cakaj(4);
	  return;
	}
  }
}
//----------------------------------------------------------------------
char nastav_cas(void)
{
  CHbit_nastav();							// pri nastaveni casu stopni cas
  nastav(str_pis[2], 0x23, 2);		// nastavenie kazdeho udaju
  nastav(str_pis[1], 0x59, 1);
  nastav(str_pis[0], 0x59, 0);
  CHbit_nuluj();
  return 0;
}
//----------------------------------------------------------------------
char nastav_datum(void)
{
  unsigned char pom;

  nastav(str_pis[5], 0x99, 6);			// nastavenie roku
  nastav(str_pis[4], 0x12, 5);			//nastavenie mesiaca
  pom = pct_dni[(unsigned char)bcdToDec(udaje.mes)-1];
  if ((udaje.mes == 2) && ((bcdToDec(udaje.rok) % 4) == 0))			//ak je februar a rok delitelny 4 potom
    pom++;															//zvacsi zobrazene dni (pom) o 1 (29)
  nastav(str_pis[3], pom, 4);		// nastavenie dna	// pom - max zobrazena hodnota
  return 0;
}
//----------------------------------------------------------------------
char nastav_extra(void)
{
  nastav1();
  stav |= 2;			// nesnimaj pocas nastavovania - alarm
  nastav2();
  if (alarm.al_flag == 1) {
    nastav3();
    nastav4();
  }
  if ((alarm.al_flag == 1) && ((alarm.clock == 1) || (alarm.temp == 1)) )
    nastav5();
  stav &= ~2;
  return 0;
}
//----------------------------------------------------------------------
char nastav_ukoncit(void)
{
  return 1;
}
//----------------------------------------------------------------------
void LCDcistiXY(char x, char y, char pct)
{
  LCDGotoXY(x, y);
  for (unsigned char i = 0; i < pct; i++)
    LCDsendChar(' ');
}
//----------------------------------------------------------------------
void LCDstringXY(char X, char Y, char *str, char pct)
{
  LCDGotoXY(X, Y);
  LCDstring(str, pct);
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
void skenuj_tep(void)		// funkcia pouzita len v casovaci
{
  if (udaje.teplota < 80) {
    if (udaje.teplota <= mon_udaje.min_temp) {
	  mon_udaje.min_temp = udaje.teplota;
	  mon_udaje.min_den = udaje.den;
	  mon_udaje.min_mes = udaje.mes;
	  mon_udaje.min_rok = udaje.rok;
	}
    if (udaje.teplota >= mon_udaje.max_temp) {
	  mon_udaje.max_temp = udaje.teplota;
      mon_udaje.max_den = udaje.den;
	  mon_udaje.max_mes = udaje.mes;
	  mon_udaje.max_rok = udaje.rok;
    }
  }
}
//----------------------------------------------------------------------
void USART_Transmit(char ch)
{
   while (!(UCSR0A & (1<<UDRE0)));
   UDR0=ch;

   return 0;
}
//----------------------------------------------------------------------
char USART_Receive(void)
{
   unsigned char ch;

   while (!(UCSR0A & (1<<RXC0)));
   ch=UDR0;  

   return ch;
}
//----------------------------------------------------------------------
/* 19200kbps 8N1 */
void USART_Init(void)
{
  UCSR0A = 0x00;
  UCSR0B = 0x98;
  UCSR0C = 0x06;
  UBRR0H = 0x00;
  UBRR0L = 0x19;
}
//----------------------------------------------------------------------
void ansi_cl(void)
{
  // ANSI clear screen: cl=\E[H\E[J
  putchar(27);
  putchar('[');
  putchar('H');
  putchar(27);
  putchar('[');
  putchar('J');
}
//----------------------------------------------------------------------

void ansi_cm(unsigned char row,unsigned char col)
{
  // ANSI cursor movement: cl=\E%row;%colH
  putchar(27);
  putchar('[');
  printf("%d",row);
  putchar(';');
  printf("%d",col);
  putchar('H');
}
//----------------------------------------------------------------------

#endif
