//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Main.h"
#include "MikMod.h"
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
    tbRepeat->Hint = "Repeat";
    tbMute->Hint = "Mute";
    TrackBar1->Hint = "Volume";

    OpenDialog1->Title = "Open Module File";
    const String LExtension = "*.669;*.amf;*.dsm;*.far;*.gdm;*.imf;*.it;*.med;"
        "*.mod;*.mtm;*.okta;*.s3m;*.stm;*.stx;*.ult;*.uni;*.xm;";
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
    ImageListAddRes(ImageList1, "PNG_REPEAT");

    FMikMod = new TMikMod(TModuleDriver::Windows);

    tbRepeat->Down = FMikMod->Wrap;

    // Play a module in the resource
    FMikMod->LoadFromResourceName(reinterpret_cast<NativeUInt>(HInstance), "MOD_MUSIC", 32);

    Start();
}
//---------------------------------------------------------------------------

__fastcall TForm1::~TForm1()
{
    for(std::vector<TProgressBar*>::iterator it = FBars.begin(); it != FBars.end(); ++it)
    {   // Free memory
        delete *it;
    }
    FBars.clear();
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
    for(int i = 0; i < FBars.size(); ++i)
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

    for(std::vector<TProgressBar*>::iterator it = FBars.begin(); it != FBars.end(); ++it)
    {   // Free memory
        delete *it;
    }
    FBars.clear();

    for(int i = FMikMod->VoiceCount - 1; i >= 0; --i)
    {
        TProgressBar* LBar = new TProgressBar(static_cast<System::Classes::TComponent*>(NULL));
        LBar->Parent = Panel1;
        LBar->Min = 0;
        LBar->Max = 80000;
        LBar->Orientation = TProgressBarOrientation::pbVertical;
        LBar->Align = TAlign::alLeft;
        LBar->Hint = "Voice " + String(i);
        LBar->Width = 35;
        FBars.push_back(LBar);
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

void __fastcall TForm1::tbRepeatClick(TObject *Sender)
{
    FMikMod->Wrap = tbRepeat->Down;
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
        tbMute->Hint = "Unmute";
    }
    else
    {
        SetVolumeImageIndex();
        TrackBar1->Enabled = true;
        FMikMod->Volume = TrackBar1->Position;
        tbMute->Hint = "Mute";
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

int __fastcall TForm1::ImageListAddRes(TImageList* AImageList, const String Identifier)
{
    if(AImageList == NULL)
    {
        return -1;
    }

    int Result;

    TWICImage* PngImage = NULL;
    Graphics::TBitmap* BitmapImage = NULL;
    try
    {
        PngImage = new TWICImage();
        PngImage->LoadFromResourceName(reinterpret_cast<NativeUInt>(HInstance), Identifier);

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

