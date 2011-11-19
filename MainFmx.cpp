//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "MainFmx.h"
#include "MikMod.h"
//---------------------------------------------------------------------------
#pragma link "TMikModLib.lib"
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    FMikMod = new TMikMod(md_Windows);
    FMikMod->LoadFromFile(String("D:\\C++Builder\\TMikMod\\trunk\\music\\clair_de_lune.mod"), 32);
    FMikMod->Start();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
    delete FMikMod;
}
//---------------------------------------------------------------------------

