//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *Ladenie1;
    TMenuItem *Relnyrem1;
    TMenuItem *Pripojenie1;
    TLabel *Label1;
    TButton *Button1;
    TComboBox *ComboBox1;
    TTimer *Timer1;
    TGroupBox *GroupBox1;
    TEdit *Edit1;
    TEdit *Edit2;
    TEdit *Edit3;
    TLabel *Label4;
    TLabel *Label5;
    TButton *Button3;
    TButton *Button4;
    TGroupBox *GroupBox2;
    TEdit *Edit4;
    TEdit *Edit5;
    TEdit *Edit6;
    TLabel *Label6;
    TLabel *Label7;
    TButton *Button5;
    TButton *Button6;
    TGroupBox *GroupBox3;
    TCheckBox *CheckBox1;
    TCheckBox *CheckBox2;
    TCheckBox *CheckBox3;
    TCheckBox *CheckBox4;
    TEdit *Edit7;
    TEdit *Edit8;
    TEdit *Edit9;
    TLabel *Label8;
    TLabel *Label9;
    TEdit *Edit10;
    TComboBox *ComboBox2;
    TButton *Button7;
    TButton *Button8;
    TButton *Button9;
    TGroupBox *GroupBox4;
    TGroupBox *GroupBox5;
    TGroupBox *GroupBox6;
    TGroupBox *GroupBox7;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label14;
    TLabel *Label21;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *Label30;
    TLabel *Label31;
    TLabel *Label32;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label37;
    TLabel *Label38;
    TImage *Image1;
    TImage *Image2;
    TImage *Image3;
    TImage *Image4;
    TImage *Image5;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label39;
    TLabel *Label40;
    TLabel *Label41;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *Label44;
    TLabel *Label45;
    TLabel *Label46;
    TLabel *Label47;
    TLabel *Label48;
    TLabel *Label49;
    TLabel *Label50;
    TLabel *Label51;
    TLabel *Label52;
    TLabel *Label53;
    TLabel *Label54;
    TLabel *Label55;
    TLabel *Label56;
    TLabel *Label57;
    TLabel *Label58;
    TLabel *Label59;
    TLabel *Label60;
    TLabel *Label61;
    TLabel *Label62;
    TLabel *Label63;
    TLabel *Label64;
    TLabel *Label65;
    TTimer *Timer2;
    TTimer *Timer3;
    TTimer *Timer4;
    TTimer *Timer5;
    TGroupBox *GroupBox8;
    TLabel *Label67;
    TLabel *Label68;
    TLabel *Label69;
    TLabel *Label70;
    TLabel *Label71;
    TLabel *Label72;
    TLabel *Label73;
    TLabel *Label74;
    TTimer *Timer6;
    TLabel *Label75;
    TLabel *Label76;
    TLabel *Label77;
    TLabel *Label78;
    TTimer *Timer7;
    TGroupBox *GroupBox9;
    TLabel *Label80;
    TLabel *Label81;
    TLabel *Label82;
    TLabel *Label83;
    TLabel *Label84;
    TLabel *Label85;
    TLabel *Label86;
    TLabel *Label87;
    TTimer *Timer8;
    TLabel *Label88;
    TLabel *Label91;
    TLabel *Label94;
    TMenuItem *Extranastavenia1;
    TButton *Button10;
    TCheckBox *CheckBox5;
    TGroupBox *GroupBox10;
    TCheckBox *CheckBox6;
    TComboBox *ComboBox3;
    TLabel *Label89;
    TLabel *Label90;
    TLabel *Label66;
    TLabel *Label79;
    TLabel *Label92;
    TLabel *Label93;
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall Pripojenie1Click(TObject *Sender);
    void __fastcall N1Click(TObject *Sender);
    void __fastcall Ladenie1Click(TObject *Sender);
    void __fastcall Relnyrem1Click(TObject *Sender);
    void __fastcall Edit1Change(TObject *Sender);
    void __fastcall Edit2Change(TObject *Sender);
    void __fastcall Edit3Change(TObject *Sender);
    void __fastcall Edit4Change(TObject *Sender);
    void __fastcall Edit5Change(TObject *Sender);
    void __fastcall Edit6Change(TObject *Sender);
    void __fastcall Edit7Change(TObject *Sender);
    void __fastcall Edit8Change(TObject *Sender);
    void __fastcall Edit9Change(TObject *Sender);
    void __fastcall Edit10Change(TObject *Sender);
    void __fastcall Button9Click(TObject *Sender);
    void __fastcall Button7Click(TObject *Sender);
    void __fastcall Button8Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Timer2Timer(TObject *Sender);
    void __fastcall Timer3Timer(TObject *Sender);
    void __fastcall Timer4Timer(TObject *Sender);
    void __fastcall Timer5Timer(TObject *Sender);
    void __fastcall Timer6Timer(TObject *Sender);
    void __fastcall Timer7Timer(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall Timer8Timer(TObject *Sender);
    void __fastcall Button10Click(TObject *Sender);
    void __fastcall Extranastavenia1Click(TObject *Sender);
    void __fastcall CheckBox5Click(TObject *Sender);
    void __fastcall CheckBox6Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
