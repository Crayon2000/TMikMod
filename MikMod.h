//---------------------------------------------------------------------------
#ifndef MikModH
#define MikModH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TMikModThread;
struct MODULE;

/**
 * Module driver.
 */
enum TModuleDriver
{
    mdDirectSound,     /**< Win32 DirectSound driver. */
    mdWindows,         /**< Win32 multimedia API driver. */
    mdMacOSX,          /**< MacOS X driver. */
    mdNoSound,         /**< no sound. */
    mdRaw,             /**< raw file disk writer [music.raw]. */
    mdStandardOutput,  /**< output to stdout. */
    mdWAV              /**< RIFF WAVE file disk writer [music.wav]. */
};

/**
 * TMikMod class.
 */
class TMikMod : public TObject
{
    typedef System::TObject inherited;

private:
    MODULE* FModule;
    bool FIsThreadSafe;
    int FVolume;
    TMikModThread* FMikModThread;
    void __fastcall CheckIfOpen();
protected:
    void __fastcall UnLoad();
    void __fastcall SetModule(MODULE* AModule);
    void __fastcall SetVolume(int AVolume);
    long __fastcall GetVersion();
    String __fastcall GetSongTitle();
    String __fastcall GetModType();
    String __fastcall GetComment();

    unsigned short __fastcall GetVoiceVolume(Int8 AVoice);
    unsigned long __fastcall GetVoiceFrequency(Int8 AVoice);
    unsigned long __fastcall GetVoiceRealVolume(Int8 AVoice);
    unsigned long __fastcall GetVoicePanning(Int8 AVoice);
public:
    __fastcall TMikMod(TModuleDriver ADriver = mdWindows);
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
    __property unsigned short VoiceVolume[Int8 AVoice] = {read=GetVoiceVolume};
    __property unsigned long VoiceFrequency[Int8 AVoice] = {read=GetVoiceFrequency};
    __property unsigned long VoiceRealVolume[Int8 AVoice] = {read=GetVoiceRealVolume};
    __property unsigned long VoicePanning[Int8 AVoice] = {read=GetVoicePanning};
__published:
    __property int Volume = {read=FVolume, write=SetVolume, default = 128}; /**< Volume. */
};
//---------------------------------------------------------------------------
#endif
