//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "MainFmx.h"
#include "MikMod.h"
//---------------------------------------------------------------------------
#if defined(_WINDOWS_)
#pragma comment(lib, "TMikModLib")
#define MODULEDRIVER TModuleDriver::Windows
#elif defined(__ANDROID__)
#define MODULEDRIVER TModuleDriver::OpenSLES
#else
#pragma comment(lib, "TMikModLib")
#define MODULEDRIVER TModuleDriver::MacOSX
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
__fastcall TForm1::~TForm1()
{
    delete FMikMod;
}
//---------------------------------------------------------------------------

