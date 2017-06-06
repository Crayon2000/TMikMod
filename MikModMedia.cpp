//---------------------------------------------------------------------------
#pragma hdrstop

#include "MikModMedia.h"
#include "MikMod.h"
//---------------------------------------------------------------------------
#if defined(_WINDOWS_)
#pragma comment(lib, "TMikModLib")
#define MODULEDRIVER TModuleDriver::Windows
#elif defined(__ANDROID__)
#define MODULEDRIVER TModuleDriver::OpenSLES
#else
#pragma link "TMikModLib"
#define MODULEDRIVER TModuleDriver::MacOSX
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)

/**
 * Register Media Codec Class.
 */
void AutoRegMediaCodecClass()
{
    TMediaCodecManager::RegisterMediaCodecClass(".669", "Composer 669 File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".amf", "DSMI Advanced Module Format File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".dsm", "DSIK Internal Format File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".far", "Farandole Composer File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".gdm", "General DigiMusic File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".imf", "Imago Orpheus File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".it", "Impulse Tracker File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".med", "OctaMED File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".mod", "MOD File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".mtm", "MultiTracker Module File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".okta", "Amiga Oktalyzer File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".s3m", "Scream Tracker 3 File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".stm", "Scream Tracker File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".ult", "UltraTracker File", TMediaType::Audio, __classid(TMikModMediaCodec));
    TMediaCodecManager::RegisterMediaCodecClass(".xm", "FastTracker 2 File", TMediaType::Audio, __classid(TMikModMediaCodec));
}
#pragma startup AutoRegMediaCodecClass

/**
 * Creates a TMikModMedia from a given file.
 * @param AFileName The parameter specifies the name of the file from which to create the TMikModMedia.
 * @return A TMedia created from a specified file.
 */
TMedia* __fastcall TMikModMediaCodec::CreateFromFile(const System::UnicodeString AFileName)
{
    return new TMikModMedia(AFileName);
}

/**
 * Constructor.
 * @param AFileName The media file.
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

/**
 * Getter function for the Duration property.
 * @return The total play time of the current media file.
 */
__int64 __fastcall TMikModMedia::GetDuration(void)
{
    return FMikMod->NumberPos;
}

/**
 * Getter function for the CurrentTime property.
 * @return The current playback position.
 */
__int64 __fastcall TMikModMedia::GetCurrent(void)
{
    return FMikMod->SongPosition;
}

/**
 * Setter function for the CurrentTime property.
 * @param Value Specifies the new value for CurrentTime.
 */
void __fastcall TMikModMedia::SetCurrent(const __int64 Value)
{
    FMikMod->SongPosition = Value;
}

/**
 * Getter function for the VideoSize property.
 * @return The media file is audio only and does not have a window, so the VideoSize is (0,0).
 */
System::Types::TPointF __fastcall TMikModMedia::GetVideoSize(void)
{
    return TPointF(0, 0);
}

/**
 * Getter function for the State property.
 * @return If the current media file is playing, then State is set to Playing, otherwise it is set to Stopped.
 */
TMediaState __fastcall TMikModMedia::GetMediaState(void)
{
    return FMikMod->Active ? TMediaState::Playing : TMediaState::Stopped;
}

/**
 * Getter function for the Volume property.
 * @return The audio volume of the current media file.
 */
float __fastcall TMikModMedia::GetVolume(void)
{
    return FMikMod->Volume / 128.0f;
}

/**
 * Setter function for the Volume property.
 * @param Value Specifies the new value of Volume.
 */
void __fastcall TMikModMedia::SetVolume(const float Value)
{
    FMikMod->Volume = Value * 128;
}

/**
 * Updates the current Media, depending on the TMediaPlayerControl associated with it.
 * The TMediaPlayerControl associated with the current media is specified through the Control property.
 */
void __fastcall TMikModMedia::UpdateMediaFromControl()
{
    inherited::UpdateMediaFromControl();
}

/**
 * Plays the current media file.
 */
void __fastcall TMikModMedia::DoPlay(void)
{
    FMikMod->Start();
}

/**
 * Stops the current media from being played.
 */
void __fastcall TMikModMedia::DoStop(void)
{
    FMikMod->Stop();
}
