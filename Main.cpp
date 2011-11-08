//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Main.h"
#include "MikMod.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner), FMikMod(NULL)
{
    FMikMod = new TMikMod(md_Windows);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
    delete FMikMod;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    TStream *Res = NULL;
    //Res = new TResourceStream((unsigned)HInstance, "MOD_MUSIC", (System::WideChar *)RT_RCDATA);
    //Res = new TFileStream("music.it", fmOpenRead);
    String FileName = "music.xm";

    //FMikMod->LoadFromFile(FileName, 32, 0);
    //FMikMod->LoadFromStream(Res, 32);
    FMikMod->LoadFromResourceName((unsigned)HInstance, "MOD_MUSIC", 32);
    FMikMod->Start();

    Edit1->Text = FMikMod->ModuleType;

    delete Res;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrackBar1Change(TObject *Sender)
{
    FMikMod->Volume = TrackBar1->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    FMikMod->Stop();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    FMikMod->Start();
}
//---------------------------------------------------------------------------

