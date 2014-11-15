//---------------------------------------------------------------------------
#ifndef MikModH
#define MikModH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
namespace Mikmod
{
    class TMikModThread;
    class TVoiceList;
}
struct MODULE;

/**
 * Module driver.
 */
enum class TModuleDriver : unsigned char
{
    mdDirectSound,     /**< Win32 DirectSound driver. */
    mdWindows,         /**< Win32 multimedia API driver. */
    mdMacOSX,          /**< MacOS X driver. */
    mdOpenSLES,        /**< OpenSL ES driver (for Android native code). */
    mdNoSound,         /**< no sound. */
    mdRaw,             /**< raw file disk writer [music.raw]. */
    mdStandardOutput,  /**< output to stdout. */
    mdWAV              /**< RIFF WAVE file disk writer [music.wav]. */
};

/**
 * TVoice class.
 */
class TVoice : public System::TObject
{
    typedef System::TObject inherited;

private:
    Int8 FVoiceNumber;

protected:
    unsigned short __fastcall GetVolume();
    unsigned long __fastcall GetFrequency();
    unsigned long __fastcall GetRealVolume();
    unsigned long __fastcall GetPanning();

public:
    __fastcall TVoice(Int8 AVoiceNumber = 0);
    virtual __fastcall ~TVoice();

    __property unsigned short Volume = {read=GetVolume};
    __property unsigned long Frequency = {read=GetFrequency};
    __property unsigned long RealVolume = {read=GetRealVolume};
    __property unsigned long Panning = {read=GetPanning};
};

/**
 * TMikMod class.
 */
class TMikMod : public System::TObject
{
    typedef System::TObject inherited;

private:
    MODULE* FModule;
    bool FIsThreadSafe;
    int FVolume;
    int FVoiceCount;
    Mikmod::TMikModThread* FMikModThread;
    Mikmod::TVoiceList *FVoiceList;
    void __fastcall CheckIfOpen();
protected:
    void __fastcall UnLoad();
    void __fastcall SetModule(MODULE* AModule);
    void __fastcall SetVolume(int AVolume);
    long __fastcall GetVersion();
    String __fastcall GetSongTitle();
    String __fastcall GetModType();
    String __fastcall GetComment();
    unsigned short __fastcall GetNumberPos();
    unsigned short __fastcall GetPosition();
    void __fastcall SetPosition(unsigned short APosition);
    TVoice* __fastcall GetVoice(int Index);
    bool __fastcall GetActive();
public:
    __fastcall TMikMod(TModuleDriver ADriver = TModuleDriver::mdNoSound);
    virtual __fastcall ~TMikMod();

    void __fastcall LoadFromFile(const System::UnicodeString AFileName, int Maxchan, bool Curious = 0);
    void __fastcall LoadFromStream(System::Classes::TStream *AStream, int Maxchan, bool Curious = 0);
    void __fastcall LoadFromResourceName(unsigned Instance, const System::UnicodeString ResName, int Maxchan, bool Curious = 0);

    void __fastcall Start();
    void __fastcall Stop();
    void __fastcall Pause();

    __property long Version = {read=GetVersion}; /**< Library version. */
    __property String SongTitle = {read=GetSongTitle}; /**< Song title. */
    __property String ModuleType = {read=GetModType}; /**< Type of the module (which tracker format). */
    __property String Comment = {read=GetComment}; /**< Either the module comments, or NULL if the module doesn't have comments. */
    __property unsigned short NumberPos = {read=GetNumberPos}; /**< Number of sound positions. */
    __property unsigned short SongPosition = {read=GetPosition, write=SetPosition}; /**< Song position. */
    __property int VoiceCount = {read=FVoiceCount}; /**< Number of voice. */
    __property TVoice* Voices[int Index] = {read=GetVoice}; /**< Use Voices to obtain a pointer to a specific TVoice object in the array. */
    __property bool Active = {read=GetActive}; /**< Returns whether sound output is enabled or not. */
__published:
    __property int Volume = {read=FVolume, write=SetVolume, default = 128}; /**< Volume. */
};
//---------------------------------------------------------------------------
#endif
