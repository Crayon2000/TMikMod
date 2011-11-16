//---------------------------------------------------------------------------
#ifndef MikModH
#define MikModH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
namespace Vcl
{
    namespace Extctrls
    {
        class TTimer;
    }
}
struct MODULE;

/**
 * Module driver.
 */
enum TModuleDriver
{
    md_DirectSound,     /**< Win32 DirectSound driver. */
    md_NoSound,         /**< no sound. */
    md_Raw,             /**< raw file disk writer [music.raw]. */
    md_StandardOutput,  /**< output to stdout. */
    md_WAV,             /**< RIFF WAVE file disk writer [music.wav]. */
    md_Windows          /**< Win32 multimedia API driver. */
};

/**
 * TMikMod class.
 */
class TMikMod : public TObject
{
    typedef System::TObject inherited;

private:
    MODULE* FModule;
    int FVolume;
    Vcl::Extctrls::TTimer* FTimer;
    void __fastcall CheckIfOpen();
protected:
    void __fastcall TimerUpdate(TObject *Sender);
    void __fastcall UnLoad();
    void __fastcall SetModule(MODULE* AModule);
    void __fastcall SetVolume(int AVolume);
    long __fastcall GetVersion();
    String __fastcall GetSongTitle();
    String __fastcall GetModType();
    String __fastcall GetComment();
public:
    __fastcall TMikMod(TModuleDriver ADriver = md_Windows);
    virtual __fastcall ~TMikMod();

    void __fastcall LoadFromFile(const UnicodeString AFilename, int Maxchan, bool Curious = 0);
    void __fastcall LoadFromFile(const AnsiString AFilename, int Maxchan, bool Curious = 0);
    void __fastcall LoadFromStream(System::Classes::TStream *AStream, int Maxchan, bool Curious = 0);
    void __fastcall LoadFromResourceName(unsigned Instance, const UnicodeString ResName, int Maxchan, bool Curious = 0);

    void __fastcall Start();
    void __fastcall Stop();
    void __fastcall Pause();

    __property long Version = {read=GetVersion}; /**< Library version. */
    __property String SongTitle = {read=GetSongTitle}; /**< Song title. */
    __property String ModuleType = {read=GetModType}; /**< Type of the module (which tracker format). */
    __property String Comment = {read=GetComment}; /**< Either the module comments, or NULL if the module doesn't have comments. */
__published:
    __property int Volume = {read=FVolume, write=SetVolume, default = 128}; /**< Volume. */
};
//---------------------------------------------------------------------------
#endif
