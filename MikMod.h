// ---------------------------------------------------------------------------
#ifndef MikModH
#define MikModH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>

// ---------------------------------------------------------------------------
namespace Mikmod {
    class TMikModThread;
    class TVoiceList;
}
struct MODULE;

/**
 * Module driver.
 */
enum class TModuleDriver : unsigned char {
    /** < No sound. */
    NoSound,
    /** < Win32 DirectSound driver. */
    DirectSound,
    /** < Windows multimedia API driver. */
    Windows,
    /** < MacOS X driver. */
    MacOSX,
    /** < OpenSL ES driver (for Android native code). */
    OpenSLES,
    /** < Raw file disk writer (music.raw). */
    Raw,
    /** < RIFF WAVE file disk writer (music.wav). */
    WAV,
    /** < AIFF file disk writer (music.aiff). */
    AIFF,
    /** < Output to stdout. */
    StandardOutput,
    /** < Simple DirectMedia Layer driver. */
    SDL,
    /** < OpenAL driver. */
    OpenAL
};

/**
 * TVoice class.
 */
class TVoice : public System::TObject {
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

    /** < The volume of the sample currently playing. */
    __property unsigned short Volume = {read = GetVolume};
    /** < The frequency of the sample currently playing. */
    __property unsigned long Frequency = {read = GetFrequency};
    /** < The actual playing volume. */
    __property unsigned long RealVolume = {read = GetRealVolume};
    /** < The panning position of the sample currently playing. */
    __property unsigned long Panning = {read = GetPanning};
};

/**
 * TMikMod class.
 */
class TMikMod : public System::TObject {
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
    void __fastcall SetModule(MODULE* AModule, bool Wrap = 1, bool FadeOut = 0,
        bool Loop = 1);
    void __fastcall SetVolume(int AVolume);
    long __fastcall GetVersion();
    int __fastcall GetWrap();
    int __fastcall GetFadeOut();
    int __fastcall GetLoop();
    void __fastcall SetWrap(bool AWrap);
    void __fastcall SetFadeOut(bool AFadeOut);
    void __fastcall SetLoop(bool ALoop);
    String __fastcall GetSongTitle();
    String __fastcall GetModType();
    String __fastcall GetComment();
    unsigned short __fastcall GetNumberPos();
    unsigned short __fastcall GetPosition();
    void __fastcall SetPosition(unsigned short APosition);
    TVoice* __fastcall GetVoice(int Index);
    bool __fastcall GetActive();

public:
    __fastcall TMikMod(TModuleDriver ADriver = TModuleDriver::NoSound,
        std::string CommandLine = "");
    virtual __fastcall ~TMikMod();

    void __fastcall LoadFromFile(const System::UnicodeString AFileName,
        int Maxchan, bool Curious = 0, bool Wrap = 1, bool FadeOut = 0,
        bool Loop = 1);
    void __fastcall LoadFromStream(System::Classes::TStream *AStream,
        int Maxchan, bool Curious = 0, bool Wrap = 1, bool FadeOut = 0,
        bool Loop = 1);
    void __fastcall LoadFromResourceName(NativeUInt Instance,
        const System::UnicodeString ResName, int Maxchan, bool Curious = 0,
        bool Wrap = 1, bool FadeOut = 0, bool Loop = 1);

    void __fastcall Start();
    void __fastcall Stop();
    void __fastcall Pause();

    /** < Library version. */
    __property long Version = {read = GetVersion};
    /** < Song title. */
    __property String SongTitle = {read = GetSongTitle};
    /** < Type of the module (which tracker format). */
    __property String ModuleType = {read = GetModType};
    /** < Either the module comments, or NULL if the module doesn't have comments. */
    __property String Comment = {read = GetComment};
    /** < Number of sound positions. */
    __property unsigned short NumberPos = {read = GetNumberPos};
    /** < Song position. */
    __property unsigned short SongPosition = {
        read = GetPosition, write = SetPosition};
    /** < Number of voice. */
    __property int VoiceCount = {read = FVoiceCount};
    /** < Use Voices to obtain a pointer to a specific TVoice object in the array. */
    __property TVoice* Voices[int Index] = {read = GetVoice};
    /** < Returns whether sound output is enabled or not. */
    __property bool Active = {read = GetActive};

    __property int Wrap = {read = GetWrap, write = SetWrap}; // *< Wrap flag"
    __property int FadeOut = {read = GetFadeOut, write = SetFadeOut}; // *< FadeOut flag"
    __property int Loop = {read = GetLoop, write = SetLoop}; // *< FadeOut flag"

__published:
    /** < Volume. */
    __property int Volume = {read = FVolume, write = SetVolume, default = 128};
};
// ---------------------------------------------------------------------------
#endif
