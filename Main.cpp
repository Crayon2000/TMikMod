//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Main.h"
#include "MikMod.h"
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
#pragma comment(lib, "TMikModLib") // Include the TMikMod library
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
    const String LExtension = "*.669;*.amf;*.dsm;*.far;*.gdm;*.imf;*.it;*.med;"
        "*.mod;*.mtm;*.okta;*.s3m;*.stm;*.ult;*.xm;";
    OpenDialog1->Filter = "Module File|" + LExtension + "|";
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
    for(int i = 0; i < FBars.Length; ++i)
    {   // Free memory
        delete FBars[i];
    }
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
    for(int i = 0; i < FBars.Length; ++i)
    {
        if(FMikMod->Voices[i]->RealVolume > 0)
        {
            FBars[i]->Position = FMikMod->Voices[i]->Frequency;
        }
        else
        {
            FBars[i]->Position = 0;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Start()
{
    FMikMod->Start();
    txtModuleType->Text = FMikMod->ModuleType;
    txtSongTitle->Text = FMikMod->SongTitle;
    memoComment->Lines->Text = FMikMod->Comment;

    for(int i = 0; i < FBars.Length; ++i)
    {   // Free memory
        delete FBars[i];
    }

    FBars.Length = FMikMod->VoiceCount;
    for(int i = FBars.Length - 1; i >= 0; --i)
    {
        TProgressBar* LBar = new TProgressBar((System::Classes::TComponent*)NULL);
        LBar->Parent = Panel1;
        LBar->Min = 0;
        LBar->Max = 80000;
        LBar->Orientation = TProgressBarOrientation::pbVertical;
        LBar->Align = TAlign::alLeft;
        LBar->Hint = "Voice " + String(i);
        LBar->Width = 35;
        FBars[i] = LBar;
    }
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
