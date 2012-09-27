//---------------------------------------------------------------------------

#pragma hdrstop

#include "MikModMedia.h"
#include "MikMod.h"
//---------------------------------------------------------------------------
#if defined(_WINDOWS_)
#pragma link "TMikModLib.lib"
#define MODULEDRIVER mdWindows
#else
#pragma link "TMikModLib.a"
#define MODULEDRIVER mdMacOSX
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)

/**
 * Register Media Codec Class.
 */
void AutoRegMediaCodecClass()
{
    TMediaCodecManager::RegisterMediaCodecClass(".mod", "MOD File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".s3m", "Scream Tracker 3 File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".it", "Impulse Tracker File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".xm", "FastTracker 2 File", TMediaType::Audio, __classid(TMikModMediaCodec));
}
#pragma startup AutoRegMediaCodecClass


TMedia* __fastcall TMikModMediaCodec::CreateFromFile(const System::UnicodeString AFileName)
{
    return new TMikModMedia(AFileName);
}

/**
 * Constructor.
 */
__fastcall TMikModMedia::TMikModMedia(const System::UnicodeString AFileName) :
    Media::TMedia(AFileName)
{
    FMikMod = new TMikMod(MODULEDRIVER);
    FMikMod->LoadFromFile(AFileName, 32);
}

/**
 * Destructor.
 */
__fastcall TMikModMedia::~TMikModMedia(void)
{
    delete FMikMod;
}

__int64 __fastcall TMikModMedia::GetDuration(void)
{
    return 0;
}

__int64 __fastcall TMikModMedia::GetCurrent(void)
{
    return 0;
}

void __fastcall TMikModMedia::SetCurrent(const __int64 Value)
{
}

System::Types::TPointF __fastcall TMikModMedia::GetVideoSize(void)
{
    return TPointF(0, 0);
}

TMediaState __fastcall TMikModMedia::GetMediaState(void)
{
    return TMediaState::Unavailable;
}

float __fastcall TMikModMedia::GetVolume(void)
{
    return FMikMod->Volume / 128.0f;
}

void __fastcall TMikModMedia::SetVolume(const float Value)
{
    FMikMod->Volume = Value * 128;
}

void __fastcall TMikModMedia::DoPlay(void)
{
    FMikMod->Start();
}

void __fastcall TMikModMedia::DoStop(void)
{
    FMikMod->Stop();
}
