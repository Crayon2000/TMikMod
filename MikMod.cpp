//---------------------------------------------------------------------------
#include <map>
#pragma hdrstop

#include "MikMod.h"
#include "MikModThread.h"
#include "MikModContnrs.h"
#include "mikmod_build.h"
#include "mikmod_internals.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "dsound.lib"   // Needed for the DirectSound driver

typedef struct _MOD_READER
{
    MREADER Core;
    System::Classes::TStream *Stream;
} MOD_READER;

static BOOL GST_READER_Eof(MREADER *reader);
static BOOL GST_READER_Read(MREADER *reader, void *ptr, size_t size);
static int GST_READER_Get(MREADER *reader);
static BOOL GST_READER_Seek(MREADER *reader, long offset, int whence);
static long GST_READER_Tell(MREADER *reader);

/**
 * Constructor.
 */
__fastcall TVoice::TVoice(Int8 AVoiceNumber) :
    FVoiceNumber(AVoiceNumber)
{
}

/**
 * Destructor.
 */
__fastcall TVoice::~TVoice()
{
}

/**
 * This function returns the volume of the sample currently playing on the specified voice.
 * @param AVoice The number of the voice to get volume.
 * @return The current volume of the sample playing on the specified voice, or zero if no sample is currently playing on the voice.
 */
unsigned short __fastcall TVoice::GetVolume()
{
    return Voice_GetVolume(FVoiceNumber);
}

 /**
 * This function returns the frequency of the sample currently playing on the specified voice.
 * @param AVoice The number of the voice to get frequency.
 * @return The current frequency of the sample playing on the specified voice, or zero if no sample is currently playing on the voice.
 */
unsigned long __fastcall TVoice::GetFrequency()
{
    return Voice_GetFrequency(FVoiceNumber);
}

/**
 * This function returns the actual playing volume of the specified voice.
 * @param AVoice The number of the voice to analyze (starting from zero).
 * @return The real volume of the voice when the function was called, in the range 0-65535.
 */
unsigned long __fastcall TVoice::GetRealVolume()
{
    return Voice_RealVolume(FVoiceNumber);
}

/**
 * This function returns the panning position of the sample currently playing on the specified voice.
 * @param AVoice The number of the voice to get panning position.
 * @return The current panning position of the sample playing on the specified voice, or PAN_CENTER if no sample is currently playing on the voice.
 */
unsigned long __fastcall TVoice::GetPanning()
{
    return Voice_GetPanning(FVoiceNumber);
}

/**
 * Constructor.
 * @param ADriver The driver to use. If not specify, mdWindows will be used.
 */
__fastcall TMikMod::TMikMod(TModuleDriver ADriver) :
    System::TObject(),
    FModule(NULL),
    FVolume(128),
    FVoiceCount(0)
{
    FMikModThread = new Mikmod::TMikModThread();
    FVoiceList = new Mikmod::TVoiceList();

    std::map<TModuleDriver, MDRIVER*> DriverList;
    DriverList[mdDirectSound] = &drv_ds;
    DriverList[mdWindows] = &drv_win;
    DriverList[mdMacOSX] = &drv_osx;
    DriverList[mdNoSound] = &drv_nos;
    DriverList[mdRaw] = &drv_raw;
    DriverList[mdStandardOutput] = &drv_stdout;
    DriverList[mdWAV] = &drv_wav;

    // Register a specific driver
    MikMod_RegisterDriver(DriverList[ADriver]);
    // Register all the module loaders
    MikMod_RegisterAllLoaders();
    // Only one device is used, this is needed to use command line
    md_device = 1;

    AnsiString CommandLine;
    if(ADriver == mdDirectSound)
    {
        CommandLine = "globalfocus"; // Play if window does not have the focus
    }

    // Initialize the library
    if(MikMod_Init(CommandLine.c_str()))
    {
        throw(Exception("L'initialisation de MikMod a �chou�."));
    }

    FIsThreadSafe = MikMod_InitThreads();
}

/**
 * Destructor.
 */
__fastcall TMikMod::~TMikMod()
{
    UnLoad();
    MikMod_Exit();
    delete FMikModThread;
    delete FVoiceList;
}

/**
 *
 */
void __fastcall TMikMod::SetModule(MODULE* AModule)
{
    if(AModule == NULL)
    {
        throw Exception("Module did not load properly.");
    }
    if(FModule)
    {
        UnLoad();
    }
    FModule = AModule;
    FModule->wrap = true;    // The module will restart when it's finished

    MikMod_Lock();
    FVoiceCount = md_numchn;
    MikMod_Unlock();

    for(char i = 0; i < FVoiceCount; ++i)
    {
        FVoiceList->Add(new TVoice(i));
    }
}

/**
 * This function loads a music module from a file.
 * @param AFilename The name of the module file.
 * @param Maxchan The maximum number of channels the song is allowed to request from the mixer.
 * @param Curious The curiosity level to use.
 */
void __fastcall TMikMod::LoadFromFile(const System::UnicodeString AFileName, int Maxchan, bool Curious)
{
    MODULE *Module = NULL;
    FILE *fp = _wfopen(AFileName.w_str(), L"rb");
    if(fp)
    {
        Module = Player_LoadFP(fp, Maxchan, Curious);
        fclose(fp);
    }
    SetModule(Module);
}

/**
 * This function loads a music module from a stream.
 * @param ASream The stream to load.
 * @param Maxchan The maximum number of channels the song is allowed to request from the mixer.
 * @param Curious The curiosity level to use.
 */
void __fastcall TMikMod::LoadFromStream(System::Classes::TStream *ASream, int Maxchan, bool Curious)
{
    MOD_READER Reader;
    Reader.Stream = ASream;
    Reader.Core.Eof = &GST_READER_Eof;
    Reader.Core.Read = &GST_READER_Read;
    Reader.Core.Get = &GST_READER_Get;
    Reader.Core.Seek = &GST_READER_Seek;
    Reader.Core.Tell = &GST_READER_Tell;

    MODULE *Module = Player_LoadGeneric((MREADER *)&Reader, Maxchan, Curious);
    SetModule(Module);
}

/**
 * This function loads a music module from resource.
 * @param Instance The Instance parameter is the instance handle associated with the executable or shared library that contains the resource.
 * @param ResName The ResName is the string associated with the resource in the .rc file that was compiled with the application.
 * @param Maxchan The maximum number of channels the song is allowed to request from the mixer.
 * @param Curious The curiosity level to use.
 */
void __fastcall TMikMod::LoadFromResourceName(unsigned Instance, const System::UnicodeString ResName, int Maxchan, bool Curious)
{
    TResourceStream *ResStream = new TResourceStream(Instance, ResName, (System::WideChar *)RT_RCDATA);
    LoadFromStream(ResStream, Maxchan, Curious);
    delete ResStream;
}

/**
 * Unload a MOD file.
 */
void __fastcall TMikMod::UnLoad()
{
    if(FModule)
    {
        Player_Stop();
        Player_Free(FModule);
        FModule = NULL;
        FVoiceCount = 0;
        FVoiceList->Clear();
    }
}

/**
 * This function sets the module volume.
 * @param AVolume The new overall module playback volume, in the range 0-128.
 */
void __fastcall TMikMod::SetVolume(int AVolume)
{
    FVolume = (AVolume<0) ? 0 : (AVolume>128) ? 128 : AVolume;
    Player_SetVolume(FVolume);
}

/**
 * This function stops the currently playing module.
 * @see Start
 */
void __fastcall TMikMod::Stop()
{
    Player_SetPosition(0);
    Player_Stop();
    FMikModThread->Suspended = true;
}

/**
 * This function starts the specified module playback.
 * @see Stop
 */
void __fastcall TMikMod::Start()
{
    CheckIfOpen();
    Player_Start(FModule);
    FMikModThread->Suspended = false;
}

/**
 * This function toggles the playing/paused status of the module.
 */
void __fastcall TMikMod::Pause()
{
    Player_TogglePause();
}

/**
 * This function returns the version number of the library.
 * @return The version number, encoded as follows: (maj<<16)|(min<<8)|(rev), where maj is the major version number, min is the minor version number, and rev is the revision number.
 */
long __fastcall TMikMod::GetVersion()
{
    return MikMod_GetVersion();
}

/**
 * Make sure a module is open, else raise exception.
 */
void __fastcall TMikMod::CheckIfOpen()
{
    if(FModule == NULL)
    {
        throw Exception("Load a module first.");
    }
}

/**
 * Returns the song title.
 */
String __fastcall TMikMod::GetSongTitle()
{
    CheckIfOpen();
    return FModule->songname;
}

/**
 * Returns the name of the tracker used to create the song.
 */
String __fastcall TMikMod::GetModType()
{
    CheckIfOpen();
    return FModule->modtype;
}

/**
 * Returns the song comment, if it has one.
 */
String __fastcall TMikMod::GetComment()
{
    CheckIfOpen();
    return FModule->comment;
}

/**
 * Get a voice at a certain position.
 * @param Index The number of the voice to get.
 * @return A pointer to the voice at Index.
 */
TVoice* __fastcall TMikMod::GetVoice(int Index)
{
    return FVoiceList->Items[Index];
}

/**
 * This function has the same behaviour as feof.
 */
static BOOL GST_READER_Eof(MREADER * reader)
{
    MOD_READER *pReader = (MOD_READER *) reader;

    return (pReader->Stream->Size == (pReader->Stream->Position)) ? true : false;
}

/**
 * This function copies length bytes of data into dest, and return zero if an error occured, and any nonzero value otherwise. Note that an end-of-file condition will not be considered as an error in this case.
 */
static BOOL GST_READER_Read(MREADER * reader, void *ptr, size_t size)
{
    MOD_READER *pReader = (MOD_READER *) reader;

    pReader->Stream->Read(ptr, size);

    return 1;
}

/**
 * This function has the same behaviour as fgetc.
 */
static int GST_READER_Get(MREADER * reader)
{
    MOD_READER *pReader = (MOD_READER *) reader;
    char buf;

    pReader->Stream->Read(&buf, 1);

    return((int)buf);
}

/**
 * This function has the same behaviour as fseek, with offset 0 meaning the start of the object (module, sample) being loaded.
 */
static BOOL GST_READER_Seek(MREADER * reader, long offset, int whence)
{
    MOD_READER *pReader = (MOD_READER *) reader;

    if(whence == SEEK_SET)
        pReader->Stream->Position = offset;
    else
        pReader->Stream->Position += offset;

    return 1;
}

/**
 * This function has the same behaviour as ftell, with offset 0 meaning the start of the object being loaded.
 */
static long GST_READER_Tell(MREADER * reader)
{
    MOD_READER *pReader = (MOD_READER *) reader;

    return pReader->Stream->Position;
}
