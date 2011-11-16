//---------------------------------------------------------------------------
#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TMikMod;

class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TTrackBar *TrackBar1;
    TButton *Button1;
    TButton *Button2;
    TEdit *Edit1;
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall TrackBar1Change(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
    TMikMod *FMikMod;
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
