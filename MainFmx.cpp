//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "MainFmx.h"
#include "MikMod.h"
//---------------------------------------------------------------------------
#if defined(_WINDOWS_)
#pragma comment(lib, "TMikModLib")
#define MODULEDRIVER TModuleDriver::mdWindows
#else
#pragma link "TMikModLib.a"
#define MODULEDRIVER TModuleDriver::mdMacOSX
#endif

#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner), FMikMod(NULL)
{
    try
    {
        FMikMod = new TMikMod(MODULEDRIVER);
        //FMikMod->LoadFromFile("E:\\Musique\\MOD\\A\\aaaahhh.mod", 32);
        FMikMod->LoadFromResourceName((unsigned)HInstance, "MOD_MUSIC", 32);
        FMikMod->Start();
    }
    catch(...)
    {
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
    delete FMikMod;
}
//---------------------------------------------------------------------------

