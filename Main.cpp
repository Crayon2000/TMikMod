//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Main.h"
#include "MikMod.h"
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
#pragma comment(lib, "TMikModLib")
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner), FMikMod(NULL)
{
    ShowHint = true;
    tbOpenFile->Hint = "Open";
    tbPlay->Hint = "Play";
    tbPause->Hint = "Pause";
    tbStop->Hint = "Stop";
    tbMute->Hint = "Mute";
    TrackBar1->Hint = "Volume";

    OpenDialog1->Title = "Open Module File";
    OpenDialog1->Filter = "Module File|*.669;*.it;*.med;*.mod;*.mtm;*.xm;|";
    OpenDialog1->Options << TOpenOption::ofFileMustExist;

    ImageListAddRes(ImageList1, "PNG_PLAY");
    ImageListAddRes(ImageList1, "PNG_PAUSE");
    ImageListAddRes(ImageList1, "PNG_STOP");
    ImageListAddRes(ImageList1, "PNG_OPEN");
    ImageListAddRes(ImageList1, "PNG_VOLUME0");
    ImageListAddRes(ImageList1, "PNG_VOLUME1");
    ImageListAddRes(ImageList1, "PNG_VOLUME2");
    ImageListAddRes(ImageList1, "PNG_VOLUME3");
    ImageListAddRes(ImageList1, "PNG_VOLUMEMUTE");

    const int LCount = ControlCount;
    for(int i = 0; i < LCount; ++i)
    {
        TProgressBar* LProgressBar = dynamic_cast<TProgressBar*>(Controls[i]);
        if(LProgressBar != NULL)
        {
            LProgressBar->Max = 100000;
        }
    }

    FMikMod = new TMikMod(TModuleDriver::Windows);

    TStream *Res = NULL;
    try
    {
        //Res = new TResourceStream((NativeUInt)HInstance, "MOD_MUSIC", (System::WideChar *)RT_RCDATA);
        //Res = new TFileStream("music.it", fmOpenRead);
        //FMikMod->LoadFromStream(Res, 32);
    }
    __finally
    {
        delete Res;
    }

    FMikMod->LoadFromResourceName((unsigned)HInstance, "MOD_MUSIC", 32);

    Start();
}
//---------------------------------------------------------------------------

__fastcall TForm1::~TForm1()
{
    delete FMikMod;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBar1Change(TObject *Sender)
{
    FMikMod->Volume = TrackBar1->Position;
    SetVolumeImageIndex();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    try
    {
        ProgressBar1->Position = FMikMod->Voices[0]->Frequency;
        ProgressBar2->Position = FMikMod->Voices[1]->Frequency;
        ProgressBar3->Position = FMikMod->Voices[2]->Frequency;
        ProgressBar4->Position = FMikMod->Voices[3]->Frequency;
        ProgressBar5->Position = FMikMod->Voices[4]->Frequency;
        ProgressBar6->Position = FMikMod->Voices[5]->Frequency;
    }
    catch(...)
    {
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Start()
{
    FMikMod->Start();
    txtModuleType->Text = FMikMod->ModuleType;
    txtSongTitle->Text = FMikMod->SongTitle;
    memoComment->Lines->Text = FMikMod->Comment;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::tbPlayClick(TObject *Sender)
{
    FMikMod->Start();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::tbPauseClick(TObject *Sender)
{
    FMikMod->Pause();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::tbStopClick(TObject *Sender)
{
    FMikMod->Stop();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::tbOpenFileClick(TObject *Sender)
{
    if(OpenDialog1->Execute() == true)
    {
        FMikMod->LoadFromFile(OpenDialog1->FileName, 32, 0);
        Start();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::tbMuteClick(TObject *Sender)
{
    if(tbMute->Down == true)
    {
        tbMute->ImageIndex = 8;
        TrackBar1->Enabled = false;
        FMikMod->Volume = 0;
    }
    else
    {
        SetVolumeImageIndex();
        TrackBar1->Enabled = true;
        FMikMod->Volume = TrackBar1->Position;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetVolumeImageIndex()
{
    if(TrackBar1->Position <= 0)
    {
        tbMute->ImageIndex = 4;
    }
    else if(TrackBar1->Position <= 42)
    {
        tbMute->ImageIndex = 5;
    }
    else if(TrackBar1->Position <= 85)
    {
        tbMute->ImageIndex = 6;
    }
    else
    {
        tbMute->ImageIndex = 7;
    }
}
//---------------------------------------------------------------------------

int __fastcall TForm1::ImageListAddRes(TImageList* AImageList, String Identifier)
{
    if(AImageList == NULL)
    {
        return -1;
    }

    int Result;

    Pngimage::TPngImage* PngImage = NULL;
    Graphics::TBitmap* BitmapImage = NULL;
    try
    {
        PngImage = new Pngimage::TPngImage();
        PngImage->LoadFromResourceName((NativeUInt)HInstance, Identifier);

        Graphics::TBitmap* BitmapImage = new Graphics::TBitmap();
        BitmapImage->Assign(PngImage);

        Result = AImageList->Add(BitmapImage, NULL);
    }
    __finally
    {
        delete PngImage;
        delete BitmapImage;
    }

    return Result;
}
//---------------------------------------------------------------------------
