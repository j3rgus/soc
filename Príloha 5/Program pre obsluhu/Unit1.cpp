//---------------------------------------------------------------------------

#include <vcl.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#pragma hdrstop

#include "Unit1.h"
#include "Tserial.h"
#include "Tserial.cpp"
#include "Tserial_event.h"
#include "Tserial_event.cpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

struct {
    unsigned char teplota;
    unsigned char sek;
    unsigned char min;
    unsigned char hod;
    unsigned char mDen;
    unsigned char den;
    unsigned char mes;
    unsigned char rok;
    unsigned char t_flag;
    unsigned char mon_temp;
} udaje;

struct {
    unsigned char al_hod;
    unsigned char al_min;
    unsigned char al_sek;
    unsigned char al_temp;
    unsigned char temp;
    unsigned char clock;
    unsigned char al_flag;
    unsigned char al_den;
} alarm;

struct {
    unsigned char min_temp;
    unsigned char min_den;
    unsigned char min_mes;
    unsigned char min_rok;
    unsigned char max_temp;
    unsigned char max_den;
    unsigned char max_mes;
    unsigned char max_rok;
} mon_udaje;

struct {
    unsigned char pipa   : 1;
    unsigned char pipalo : 1;
    unsigned char ref    : 6;
} al_res;

TForm1 *Form1;
Tserial_event *serial = new Tserial_event();
Tserial *con_com = new Tserial();
bool dcs, pripojenie, realny, ladenie, nastav, extra, prijem;
bool casB, alarmB, monB;
const int dni[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char *text_c_b[10] = {"Pondelok", "Utorok", "Streda", "Štvrtok", "Piatok", "Sobota", "Nedela", "Po - Pia", "So - Ne", "Po - Ne"};
char *frek_pole[9] = {"jednosmerne","122,549 Hz","61,274 Hz","40,849 Hz","30,637 Hz","24,509 Hz","20,424 Hz","6,127 Hz","1,00368 Hz"};
unsigned char timer_ref, timer2_ref, port_dis, stav, cursor, prepni, freq, freq_nastavena;
unsigned char *p_udaje = &udaje.sek;
unsigned char *p_mon = &mon_udaje.min_temp;
unsigned char *p_alarm = &alarm.al_hod;
unsigned char eeprom[8];
unsigned int i = 0, j;
char lcd_dis[2][17];
char *buf;
int   size;


//----------------------------------------------------------------------
int decToBcd(char d)
{
  return (((d/10)<<4) + (d%10));
}
//----------------------------------------------------------------------
int bcdToDec(unsigned char b)
{
  return (((b>>4)*10) + ((b&0x0f)));
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  unsigned char s = 's', *b, h='h';

  prijem = true;
  if (dcs == true) {
    serial->sendData(&s, 1);
    serial->setRxSize(75);
    Sleep(20);
    buf = serial->getDataInBuffer();
    serial->dataHasBeenRead();
    Sleep(2);
    for (i = 0; i < 9; i++) {
      p_udaje[i] = buf[i];
    }
    for (i = 9; i < 17; i++) {
      p_mon[i-9] = buf[i];
    }
    for (i = 17; i < 25; i++) {
      p_alarm[i-17] = buf[i];
    }
    al_res.pipa = buf[25] & 1;
    al_res.pipalo = buf[25] & 2;
    al_res.ref = buf[25] & ~3;
    timer_ref = buf[26];
    stav = buf[27];
    cursor = buf[28];

    for (i = 29; i < 45; i++) {
      lcd_dis[0][i-29] = buf[i];
    }
    for (i = 45; i < 61; i++) {
      lcd_dis[1][i-45] = buf[i];
    }
    udaje.teplota = buf[61];
    timer2_ref = buf[62];
    port_dis = buf[63] >> 2;
    for (i = 64; i < 72; i++)
      eeprom[i-64] = buf[i];
    prepni = buf[72];
    freq = buf[73];
    freq_nastavena = buf[74];
  }
  prijem = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Pripojenie1Click(TObject *Sender)
{
  if (nastav == true) {
    GroupBox1->Visible = false;
    GroupBox2->Visible = false;
    GroupBox3->Visible = false;
    Button9->Visible = false;
    nastav = false;
  }
  if (ladenie == true) {
    GroupBox4->Visible = false;
    GroupBox5->Visible = false;
    GroupBox6->Visible = false;
    GroupBox7->Visible = false;
    GroupBox8->Visible = false;
    GroupBox9->Visible = false;
    ladenie = false;
  }
  if (realny == true) {
    Image1->Visible = false;
    Image2->Visible = false;
    Image3->Visible = false;
    Image4->Visible = false;
    Image5->Visible = false;
    realny = false;
    Label73->Visible = false;
    Label74->Visible = false;
  }
  if (extra == true) {
    GroupBox10->Visible = false;
    CheckBox5->Visible = false;
    Button10->Visible = false;
  }
  Label1->Visible = true;
  Button1->Visible = true;
  ComboBox1->Visible = true;
  pripojenie = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N1Click(TObject *Sender)
{
  if (pripojenie == true) {
    Label1->Visible = false;
    Button1->Visible = false;
    ComboBox1->Visible = false;
    pripojenie = false;
  }
  if (ladenie == true) {
    GroupBox4->Visible = false;
    GroupBox5->Visible = false;
    GroupBox6->Visible = false;
    GroupBox7->Visible = false;
    GroupBox8->Visible = false;
    GroupBox9->Visible = false;
    ladenie = false;
  }
  if (realny == true) {
    Image1->Visible = false;
    Image2->Visible = false;
    Image3->Visible = false;
    Image4->Visible = false;
    Image5->Visible = false;
    realny = false;
    Label73->Visible = false;
    Label74->Visible = false;
  }
  if (extra == true) {
    GroupBox10->Visible = false;
    CheckBox5->Visible = false;
    Button10->Visible = false;
  }
  GroupBox1->Visible = true;
  GroupBox2->Visible = true;
  GroupBox3->Visible = true;
  Button9->Visible = true;
  nastav = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Ladenie1Click(TObject *Sender)
{
  if (pripojenie == true) {
    Label1->Visible = false;
    Button1->Visible = false;
    ComboBox1->Visible = false;
    pripojenie = false;
  }
  if (nastav == true) {
    GroupBox1->Visible = false;
    GroupBox2->Visible = false;
    GroupBox3->Visible = false;
    Button9->Visible = false;
    nastav = false;
  }
  if (realny == true) {
    Image1->Visible = false;
    Image2->Visible = false;
    Image3->Visible = false;
    Image4->Visible = false;
    Image5->Visible = false;
    Label73->Visible = false;
    Label74->Visible = false;
    realny = false;
  }
  if (extra == true) {
    GroupBox10->Visible = false;
    CheckBox5->Visible = false;
    Button10->Visible = false;
  }
  GroupBox4->Visible = true;
  GroupBox5->Visible = true;
  GroupBox6->Visible = true;
  GroupBox7->Visible = true;
  GroupBox8->Visible = true;
  GroupBox9->Visible = true;
  ladenie = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Relnyrem1Click(TObject *Sender)
{
  if (pripojenie == true) {
    Label1->Visible = false;
    Button1->Visible = false;
    ComboBox1->Visible = false;
    pripojenie = false;
  }
  if (nastav == true) {
    GroupBox1->Visible = false;
    GroupBox2->Visible = false;
    GroupBox3->Visible = false;
    Button9->Visible = false;
    nastav = false;
  }
  if (ladenie == true) {
    GroupBox4->Visible = false;
    GroupBox5->Visible = false;
    GroupBox6->Visible = false;
    GroupBox7->Visible = false;
    GroupBox8->Visible = false;
    GroupBox9->Visible = false;
    ladenie = false;
  }
  if (extra == true) {
    CheckBox5->Visible = false;
    GroupBox10->Visible = false;
    Button10->Visible = false;
  }
  Image1->Visible = true;
  Image2->Visible = true;
  Image3->Visible = true;
  Image4->Visible = true;
  Image5->Visible = true;
  Label73->Visible = true;
  Label74->Visible = true;
  realny = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1Change(TObject *Sender)
{
  if (Edit1->Text.IsEmpty())
    Edit1->Text = "0";
  try {
  if (Edit1->Text.ToInt() > 23)
    Edit1->Text = "23";
  } catch (Exception &e) {
    Edit1->Text = "0";
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit2Change(TObject *Sender)
{
  if (Edit2->Text.IsEmpty())
    Edit2->Text = "0";
  try {
  if (Edit2->Text.ToInt() > 59)
    Edit2->Text = "59";
  } catch (Exception &e) {
    Edit2->Text = "0";
  }    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit3Change(TObject *Sender)
{
  if (Edit3->Text.IsEmpty())
    Edit3->Text = "0";
  try {
  if (Edit3->Text.ToInt() > 59)
    Edit3->Text = "59";
  } catch (Exception &e) {
    Edit3->Text = "0";
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit4Change(TObject *Sender)
{
  try {
  if (Edit4->Text.IsEmpty() || (Edit4->Text.ToInt() == 0))
    Edit4->Text = "1";
  if (Edit4->Text.ToInt() > dni[Edit5->Text.ToInt()-1]) {
    if ((Edit5->Text.ToInt() == 2) && ((Edit6->Text.ToInt() % 4) == 0))
      Edit4->Text = dni[Edit5->Text.ToInt()-1] + 1;
    else  Edit4->Text = dni[Edit5->Text.ToInt()-1];
  }
  } catch (Exception &e) {
    Edit4->Text = "1";
  }    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit5Change(TObject *Sender)
{
  try {
  if (Edit5->Text.ToInt() > 12) {
    Edit5->Text = 1;
    Edit4->Text = 1;
  }
  if (Edit4->Text.ToInt() > dni[Edit5->Text.ToInt()-1]) {
    if ((Edit5->Text.ToInt() == 2) && ((Edit6->Text.ToInt() % 4) == 0))
      Edit4->Text = dni[Edit5->Text.ToInt()-1] + 1;
    else  Edit4->Text = dni[Edit5->Text.ToInt()-1];
  }
  if (Edit5->Text.IsEmpty() || (Edit5->Text.ToInt() == 0))
    Edit5->Text = "1";
  if (Edit5->Text.ToInt() > 12)
    Edit5->Text = "1";
  } catch (Exception &e) {
    Edit5->Text = "1";
  }    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit6Change(TObject *Sender)
{
  try {
  if (Edit6->Text.IsEmpty())
    Edit6->Text = "0";
  if (Edit6->Text.ToInt() > 99)
    Edit6->Text = "99";
  } catch (Exception &e) {
    Edit6->Text = "0";
  }    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit7Change(TObject *Sender)
{
  if (Edit7->Text.IsEmpty())
    Edit7->Text = "0";
  try {
  if (Edit7->Text.ToInt() > 23)
    Edit7->Text = "23";
  } catch (Exception &e) {
    Edit7->Text = "0";
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit8Change(TObject *Sender)
{
  if (Edit8->Text.IsEmpty())
    Edit8->Text = "0";
  try {
  if (Edit8->Text.ToInt() > 59)
    Edit8->Text = "59";
  } catch (Exception &e) {
    Edit8->Text = "0";
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit9Change(TObject *Sender)
{
  if (Edit9->Text.IsEmpty())
    Edit9->Text = "0";
  try {
  if (Edit9->Text.ToInt() > 59)
    Edit9->Text = "59";
  } catch (Exception &e) {
    Edit9->Text = "0";
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit10Change(TObject *Sender)
{
  if (Edit10->Text.IsEmpty())
    Edit10->Text = "0";
  try {
  if (Edit10->Text.ToInt() > 70)
    Edit10->Text = "70";
  } catch (Exception &e) {
    Edit10->Text = "0";
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{
  struct tm cas;
  time_t s_cas;

  time(&s_cas);
  cas = *localtime(&s_cas);
  Edit1->Text = cas.tm_hour;
  Edit2->Text = cas.tm_min;
  Edit3->Text = cas.tm_sec;
  Edit4->Text = cas.tm_mday;
  Edit5->Text = cas.tm_mon+1;
  Edit6->Text = cas.tm_year - 100;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
  if ((udaje.mon_temp & 1) == 1)
    CheckBox1->Checked = 1;
  else CheckBox1->Checked = 0;
  if ((alarm.al_flag & 1) == 1)
    CheckBox2->Checked = 1;
  else CheckBox2->Checked = 0;
  if ((alarm.clock & 1) == 1)
    CheckBox3->Checked = 1;
  else CheckBox3->Checked = 0;
  if ((alarm.temp & 1) == 1)
    CheckBox4->Checked = 1;
  else CheckBox4->Checked = 0;
  Edit7->Text = Edit7->Text.IntToHex(alarm.al_hod, 2);
  Edit8->Text = Edit8->Text.IntToHex(alarm.al_min, 2);
  Edit9->Text = Edit9->Text.IntToHex(alarm.al_sek, 2);
  Edit10->Text = alarm.al_temp;
  ComboBox2->ItemIndex = (int)alarm.al_den-1;    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
  char al_den, b, v = 'v';
  for (al_den = 0; al_den < 10; al_den++) {
    if (ComboBox2->Text.IsEmpty() == true) {
      al_den = 0;
      break;
    }
    if (ComboBox2->Text.AnsiCompare(text_c_b[al_den]) == 0)
      break;
  }

  while (prijem != false)
    ;
  Timer1->Enabled = false;
  serial->sendData(&v, 1);
  Sleep(30);
  b = decToBcd(Edit7->Text.ToInt());
  serial->sendData(&b, 1);
  Sleep(20);
  b = decToBcd(Edit8->Text.ToInt());
  serial->sendData(&b, 1);
  Sleep(20);
  b = decToBcd(Edit9->Text.ToInt());
  serial->sendData(&b, 1);
  Sleep(20);
  b = Edit10->Text.ToInt();
  serial->sendData(&b, 1);
  Sleep(20);
  b = (CheckBox4->Checked == 1) ? 1 : 0;
  serial->sendData(&b, 1);
  Sleep(20);
  b = (CheckBox3->Checked == 1) ? 1 : 0;
  serial->sendData(&b, 1);
  Sleep(20);
  b = (CheckBox2->Checked == 1) ? 1 : 0;
  serial->sendData(&b, 1);
  Sleep(20);
  b = (CheckBox1->Checked == 1) ? 1 : 0;
  serial->sendData(&b, 1);
  Sleep(20);
  b = al_den+1;
  serial->sendData(&b, 1);
  Sleep(20);
  Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void OnDataArrival(int size, char *buffer)
{
  if ((size>0) && (buffer!=0)) {
    buffer[size] = 0;
    //printf("OnDataArrival: %s ",buffer);
  }
}
//---------------------------------------------------------------------------
void SerialEventManager(uint32 object, uint32 event)
{
  int   size;

  if (serial != 0)
  {
    switch(event)
    {
      case  SERIAL_CONNECTED  :
                                        Form1->Button1->Caption = "Odpoji";
                                        break;
      case  SERIAL_DISCONNECTED  :
                                        Form1->Button1->Caption = "Pripoji";
                                        break;
      case  SERIAL_DATA_SENT  :
                                        //printf("Data sent ! \n");
                                        break;
      case  SERIAL_RING       :
                                        //printf("DRING ! \n");
                                        break;
      case  SERIAL_CD_ON      :
                                        //printf("Carrier Detected ! \n");
                                        break;
      case  SERIAL_CD_OFF     :
                                        //printf("No more carrier ! \n");
                                        break;
      case  SERIAL_DATA_ARRIVAL  :
                                        //size   = serial->getDataInSize();
                                        //buffer = serial->getDataInBuffer();
                                        //OnDataArrival(size, buffer);
                                        //serial->dataHasBeenRead();
                                        break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormActivate(TObject *Sender)
{
  //serial->setManager(SerialEventManager);
  dcs = false;
  pripojenie = true;
  realny = false;
  ladenie = false;
  nastav = false;
  lcd_dis[0][16] = 0;
  lcd_dis[1][16] = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  int err;
  char h = 'h', *b;

  if (dcs == false) {
    err = serial->connect(ComboBox1->Text.c_str(), 19200, SERIAL_PARITY_NONE, 8, true);
    if (!err) {
      serial->sendData(&h, 1);
      b = serial->getDataInBuffer();
      serial->dataHasBeenRead();
      dcs = true;
    }
    else
      Application->MessageBoxA("Nemozno sa pripojit na port", "Varovanie", MB_OK);
  }
  else {
    serial->disconnect();
    dcs = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  char y='y', freq = 0, kontr = 0, pod='_';
  if (dcs == true) {
    Timer1->Enabled = false;
    if (prepni) {
      serial->sendData(&y,1);
      Sleep(20);
    }
    if (freq_nastavena) {
      serial->sendData(&pod,1);
      Sleep(20);
      serial->sendData(&freq,1);
      Sleep(20);
      serial->sendData(&kontr,1);
      Sleep(20);
    }
    serial->disconnect();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
  if (pripojenie == true) {
    if (dcs == true) {
      Button1->Caption = "Odpoji";
      Extranastavenia1->Enabled = true;
      N1->Enabled = true;
      Relnyrem1->Enabled = true;
      Ladenie1->Enabled = true;
    }
    else {
      Button1->Caption = "Pripoji";
      Extranastavenia1->Enabled = false;
      N1->Enabled = false;
      Relnyrem1->Enabled = false;
      Ladenie1->Enabled = false;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer3Timer(TObject *Sender)
{
  if ((dcs == true) && (ladenie == true)) {
    Label2->Caption = udaje.teplota;
    Label3->Caption = Label3->Caption.IntToHex(p_udaje[0], 2);  //udaje
    Label39->Caption = Label39->Caption.IntToHex(p_udaje[1], 2);
    Label40->Caption = Label40->Caption.IntToHex(p_udaje[2], 2);
    Label41->Caption = Label41->Caption.IntToHex(p_udaje[3], 2);
    Label42->Caption = Label42->Caption.IntToHex(p_udaje[4], 2);
    Label43->Caption = Label43->Caption.IntToHex(p_udaje[5], 2);
    Label44->Caption = Label44->Caption.IntToHex(p_udaje[6], 2);
    Label45->Caption = Label45->Caption.IntToHex(p_udaje[7], 2);
    Label46->Caption = Label46->Caption.IntToHex(p_udaje[8], 2);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer4Timer(TObject *Sender)
{
  if ((dcs == true) && (ladenie == true)) {
    Label47->Caption = (int)p_mon[0];    //mon_udaje
    Label48->Caption = Label48->Caption.IntToHex(p_mon[1], 2);
    Label49->Caption = Label49->Caption.IntToHex(p_mon[2], 2);
    Label50->Caption = Label50->Caption.IntToHex(p_mon[3], 2);
    Label51->Caption = (int)p_mon[4];
    Label52->Caption = Label52->Caption.IntToHex(p_mon[5], 2);
    Label53->Caption = Label53->Caption.IntToHex(p_mon[6], 2);
    Label54->Caption = Label54->Caption.IntToHex(p_mon[7], 2);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer5Timer(TObject *Sender)
{
  if ((dcs == true) && (ladenie == true)) {
    Label55->Caption = Label55->Caption.IntToHex(p_alarm[0], 2);  //alarm
    Label56->Caption = Label56->Caption.IntToHex(p_alarm[1], 2);
    Label57->Caption = Label57->Caption.IntToHex(p_alarm[2], 2);
    Label58->Caption = p_alarm[3];
    Label59->Caption = Label59->Caption.IntToHex(p_alarm[4], 2);
    Label60->Caption = Label60->Caption.IntToHex(p_alarm[5], 2);
    Label61->Caption = Label61->Caption.IntToHex(p_alarm[6], 2);
    Label62->Caption = (int)p_alarm[7];
    Label70->Caption = timer_ref;
    Label77->Caption = timer2_ref;
    Label78->Caption = port_dis;
    Label71->Caption = prepni;
    Label72->Caption = cursor;
    Label63->Caption = al_res.pipa;
    Label64->Caption = al_res.pipalo;
    Label65->Caption = al_res.ref;
    Label90->Caption = stav;
    Label79->Caption = freq;
    Label93->Caption = freq_nastavena;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer6Timer(TObject *Sender)
{
  if ((dcs == true) && (realny == true)) {
    if (lcd_dis[0][11] == '+') {
      lcd_dis[0][14] = '°';
      lcd_dis[0][15] = 'C';
    }
    if ((!strcmp(lcd_dis[0], "Teplota: Zapnute")) || (!strcmp(lcd_dis[0], "Teplota: Vypnute"))) {
      lcd_dis[1][2] = '°';
      lcd_dis[1][3] = 'C';
    }
    if ((lcd_dis[0][0] == 'M' && lcd_dis[0][1] == 'I') && (lcd_dis[1][0] == 'M' && lcd_dis[1][1] == 'A')) {
      lcd_dis[0][7] = '°';
      lcd_dis[0][8] = 'C';
      lcd_dis[0][9] = '\0';
      lcd_dis[1][7] = '°';
      lcd_dis[1][8] = 'C';
      lcd_dis[1][9] = '\0';
    }
    Label73->Caption = lcd_dis[0];
    Label74->Caption = lcd_dis[1];
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer7Timer(TObject *Sender)
{
  if (nastav == true) {
    if (CheckBox2->Checked == true) {
      CheckBox3->Visible = true;
      CheckBox4->Visible = true;
    } else {
      CheckBox3->Checked = false;
      CheckBox4->Checked = false;
      CheckBox3->Visible = false;
      CheckBox4->Visible = false;

    }
    if ((CheckBox3->Checked == true) && (CheckBox2->Checked == true)) {
      Edit7->Visible = true;
      Edit8->Visible = true;
      Edit9->Visible = true;
      Label8->Visible = true;
      Label9->Visible = true;
    } else {
      Edit7->Visible = false;
      Edit8->Visible = false;
      Edit9->Visible = false;
      Label8->Visible = false;
      Label9->Visible = false;
    }
    if ((CheckBox4->Checked == true) && (CheckBox2->Checked == true)) {
      Edit10->Visible = true;
    }
    else {
      Edit10->Visible = false;
    }
    if ((CheckBox4->Checked == true) ||(CheckBox3->Checked == true)) {
      ComboBox2->Visible = true;
    }
    else
      ComboBox2->Visible = false;
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button3Click(TObject *Sender)
{
  Edit3->Text = bcdToDec(p_udaje[0]);
  Edit2->Text = bcdToDec(p_udaje[1]);
  Edit1->Text = bcdToDec(p_udaje[2]);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
  Edit4->Text = bcdToDec(udaje.den);
  Edit5->Text = bcdToDec(udaje.mes);
  Edit6->Text = bcdToDec(udaje.rok);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
  char t = 't', b;  //poziadavka pre zapis casu

  while (prijem != false)
    ;
  Timer1->Enabled = false;
  serial->sendData(&t,1);
  Sleep(20);
  b = decToBcd(Edit3->Text.ToInt());
  serial->sendData(&b,1);
  Sleep(20);
  b = decToBcd(Edit2->Text.ToInt());
  serial->sendData(&b,1);
  Sleep(20);
  b = decToBcd(Edit1->Text.ToInt());
  serial->sendData(&b,1);
  Sleep(20);
  Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
  char u = 'u', b;  //poziadavka pre zapis casu

  while (prijem != false)
    ;
  Timer1->Enabled = false;
  serial->sendData(&u,1);
  Sleep(20);
  b = decToBcd(Edit4->Text.ToInt());
  serial->sendData(&b,1);
  Sleep(20);
  b = decToBcd(Edit5->Text.ToInt());
  serial->sendData(&b,1);
  Sleep(20);
  b = decToBcd(Edit6->Text.ToInt());
  serial->sendData(&b,1);
  Sleep(20);
  Timer1->Enabled = true;
}
//---------------------------------------------------------------------------





void __fastcall TForm1::Timer8Timer(TObject *Sender)
{
  if ((dcs == true) && (ladenie == true)) {      //EEPROM addr 0x00 - 0x07
    Label80->Caption = Label80->Caption.IntToHex(eeprom[0], 2);
    Label81->Caption = Label81->Caption.IntToHex(eeprom[1], 2);
    Label82->Caption = Label82->Caption.IntToHex(eeprom[2], 2);
    Label83->Caption = Label83->Caption.IntToHex(eeprom[3], 2);
    Label84->Caption = Label84->Caption.IntToHex(eeprom[4], 2);
    Label85->Caption = Label85->Caption.IntToHex(eeprom[5], 2);
    Label86->Caption = Label86->Caption.IntToHex(eeprom[6], 2);
    Label87->Caption = Label87->Caption.IntToHex(eeprom[7], 2);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button10Click(TObject *Sender)
{
  char x = 'x';

  Timer1->Enabled = false;
  Sleep(20);
  serial->sendData(&x,1);
  Sleep(20);
  Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Extranastavenia1Click(TObject *Sender)
{
  if (pripojenie == true) {
    Label1->Visible = false;
    Button1->Visible = false;
    ComboBox1->Visible = false;
    pripojenie = false;
  }
  if (nastav == true) {
    GroupBox1->Visible = false;
    GroupBox2->Visible = false;
    GroupBox3->Visible = false;
    Button9->Visible = false;
    nastav = false;
  }
  if (ladenie == true) {
    GroupBox4->Visible = false;
    GroupBox5->Visible = false;
    GroupBox6->Visible = false;
    GroupBox7->Visible = false;
    GroupBox8->Visible = false;
    GroupBox9->Visible = false;
    ladenie = false;
  }
  if (realny == true) {
    Image1->Visible = false;
    Image2->Visible = false;
    Image3->Visible = false;
    Image4->Visible = false;
    Image5->Visible = false;
    realny = false;
    Label73->Visible = false;
    Label74->Visible = false;
  }
  CheckBox5->Visible = true;
  Button10->Visible = true;
  if (CheckBox5->Checked == true)
    GroupBox10->Visible = true;
  extra = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox5Click(TObject *Sender)
{
  char y='y';
  Timer1->Enabled = false;
  if (CheckBox5->Checked == true) {
    serial->sendData(&y,1);
    //Sleep(20);
    N1->Enabled = false;
    GroupBox10->Visible = true;
  }
  else {
    serial->sendData(&y,1);
    //Sleep(20);
    N1->Enabled = true;
    GroupBox10->Visible = false;
    CheckBox6->Checked = false;
  }
  Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox6Click(TObject *Sender)
{
  char pod='_', freq, kontr;
  int i;

  Timer1->Enabled = false;
  serial->sendData(&pod,1);
  Sleep(20);
  if (CheckBox6->Checked == true) {
    kontr = 1;
    if ((ComboBox3->Text.AnsiCompare("jednosmerne") == 0) || (ComboBox3->Text.IsEmpty()))
      freq = 1;
    else freq = 2;
    serial->sendData(&freq,1);
    Sleep(20);
    serial->sendData(&kontr,1);
    Sleep(20);
  }
  else {
    freq = 0;
    kontr = 0;
    serial->sendData(&freq,1);
    Sleep(20);
    serial->sendData(&kontr,1);
    Sleep(20);
  }
  Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

