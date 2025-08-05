//---------------------------------------------------------------------------
#include <map>
#pragma hdrstop

#include "MikMod.h"
#include "MikModThread.h"
#include "MikModContnrs.h"
#include "include/mikmod.h"
#include "include/mikmod_internals.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#if defined(DRV_DS)
#pragma comment(lib, "dsound") // Needed for the DirectSound driver
#endif                      // DRV_DS

typedef struct _MOD_READER
{
    MREADER Core; /**< A MREADER struct. */
    System::Classes::TStream *Stream; /**< A stream. */
} MOD_READER;

static BOOL GST_READER_Eof(MREADER *reader);
static BOOL GST_READER_Read(MREADER *reader, void *ptr, size_t size);
static int GST_READER_Get(MREADER *reader);
static int GST_READER_Seek(MREADER *reader, long offset, int whence);
static long GST_READER_Tell(MREADER *reader);

/**
 * Constructor.
 */
__fastcall TVoice::TVoice(Int8 AVoiceNumber) :
    System::TObject(),
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
 * @return The current volume of the sample playing on the specified voice, or zero if no sample is currently playing on the voice.
 */
unsigned short __fastcall TVoice::GetVolume()
{
    return Voice_GetVolume(FVoiceNumber);
}

 /**
 * This function returns the frequency of the sample currently playing on the specified voice.
 * @return The current frequency of the sample playing on the specified voice, or zero if no sample is currently playing on the voice.
 */
unsigned long __fastcall TVoice::GetFrequency()
{
    return Voice_GetFrequency(FVoiceNumber);
}

/**
 * This function returns the actual playing volume of the specified voice.
 * @return The real volume of the voice when the function was called, in the range 0-65535.
 */
unsigned long __fastcall TVoice::GetRealVolume()
{
    return Voice_RealVolume(FVoiceNumber);
}

/**
 * This function returns the panning position of the sample currently playing on the specified voice.
 * @return The current panning position of the sample playing on the specified voice, or PAN_CENTER if no sample is currently playing on the voice.
 */
unsigned long __fastcall TVoice::GetPanning()
{
    return Voice_GetPanning(FVoiceNumber);
}

/**
 * Constructor.
 * @param ADriver The driver to use. If not specify, mdNoSound will be used.
 */
__fastcall TMikMod::TMikMod(TModuleDriver ADriver) :
    System::TObject(),
    FModule(NULL),
    FVolume(128),
    FWrap(true),
    FLoop(true),
    FFadeOut(false),
    FVoiceCount(0)
{
    std::map<TModuleDriver, MDRIVER*> DriverList;
    DriverList[TModuleDriver::NoSound] = &drv_nos;
#ifdef DRV_DS
    DriverList[TModuleDriver::DirectSound] = &drv_ds;
#endif /* DRV_DS */
#ifdef DRV_WIN
    DriverList[TModuleDriver::Windows] = &drv_win;
#endif /* DRV_WIN */
#ifdef DRV_OSX
    DriverList[TModuleDriver::MacOSX] = &drv_osx;
#endif /* DRV_OSX */
#ifdef DRV_OSLES
    DriverList[TModuleDriver::OpenSLES] = &drv_osles;
#endif /* DRV_OSLES */
#ifdef DRV_SDL
    DriverList[TModuleDriver::SDL] = &drv_sdl;
#endif /* DRV_SDL */
#ifdef DRV_OPENAL
    DriverList[TModuleDriver::OpenAL] = &drv_openal;
#endif /* DRV_OPENAL */
#ifdef DRV_RAW
    DriverList[TModuleDriver::Raw] = &drv_raw;
#endif /* DRV_RAW */
#ifdef DRV_WAV
    DriverList[TModuleDriver::WAV] = &drv_wav;
#endif /* DRV_WAV */
#ifdef DRV_AIFF
    DriverList[TModuleDriver::AIFF] = &drv_aiff;
#endif /* DRV_AIFF */
#ifdef DRV_STDOUT
    DriverList[TModuleDriver::StandardOutput] = &drv_stdout;
#endif /* DRV_STDOUT */

    if(DriverList[ADriver] == NULL)
    {
        throw Exception("Invalid driver.");
    }

    // Register a specific driver
    MikMod_RegisterDriver(DriverList[ADriver]);
    // Register all the module loaders
    MikMod_RegisterAllLoaders();
    // Only one device is used, this is needed to use command line
    md_device = 1;
    // Set compact disc quality (default value)
    md_mixfreq = 44100;

    std::string CommandLine; // Do not use AnsiString, there is a problem with OS X
    if(ADriver == TModuleDriver::DirectSound)
    {
        CommandLine = "globalfocus,"; // Play if window does not have the focus
        CommandLine += "buffer=15"; // Size of the buffer
    }

    // Initialize the library
    if(MikMod_Init(CommandLine.c_str()) != 0)
    {
        String LException = "An error occurred during initialization.";
        String LError = MikMod_strerror(MikMod_errno); // Get last error
        if(LError.IsEmpty() == false)
        {
            LException += " " + LError + ".";
        }
        throw Exception(LException, MikMod_errno);
    }

    // This should be at the end of the constructor in case an exception is thrown
    FMikModThread = new Mikmod::TMikModThread();
    FMikModThread->OnModuleCompleted = ModuleCompleted;
    FVoiceList = new Mikmod::TVoiceList();

    FIsThreadSafe = MikMod_InitThreads();
}

/**
 * Destructor.
 */
__fastcall TMikMod::~TMikMod()
{
    // The thread must be deleted first in order to stop updating the sound
    delete FMikModThread;

    UnLoad();
    MikMod_Exit();
    delete FVoiceList;
}

/**
 * Set a module.
 * @param AModule The module to set.
 */
void __fastcall TMikMod::SetModule(MODULE* AModule)
{
    if(AModule == NULL)
    {
        throw Exception("Module did not load properly.");
    }
    if(FModule != NULL)
    {
        UnLoad();
    }
    FModule = AModule;
    FModule->wrap = FWrap; // The module will restart when it's finished
    FModule->loop = FLoop; // Allow module to loop
    FModule->fadeout = FFadeOut; // The module will fadeout while running lastpos if true

    MikMod_Lock();
    FVoiceCount = md_numchn;
    MikMod_Unlock();

    for(int i = 0; i < FVoiceCount; ++i)
    {
        FVoiceList->Add(new TVoice(i));
    }
}

/**
 * This function loads a music module from a file.
 * @param AFileName The name of the module file.
 * @param Maxchan The maximum number of channels the song is allowed to request from the mixer.
 * @param Curious The curiosity level to use.
 */
void __fastcall TMikMod::LoadFromFile(const System::UnicodeString AFileName, int Maxchan, bool Curious)
{
    TFileStream *FileStream = NULL;
    try
    {
        FileStream = new TFileStream(AFileName, fmOpenRead);
        LoadFromStream(FileStream, Maxchan, Curious);
    }
    __finally
    {
        delete FileStream;
    }
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
void __fastcall TMikMod::LoadFromResourceName(NativeUInt Instance, const System::UnicodeString ResName, int Maxchan, bool Curious)
{
    TResourceStream *ResStream = NULL;
    try
    {
        ResStream = new TResourceStream(Instance, ResName, (System::WideChar *)RT_RCDATA);
        LoadFromStream(ResStream, Maxchan, Curious);
    }
    __finally
    {
        delete ResStream;
    }
}

/**
 * Unload a MOD file.
 */
void __fastcall TMikMod::UnLoad()
{
    if(FModule != NULL)
    {
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
    FVolume = (AVolume < 0) ? 0 : (AVolume > 128) ? 128 : AVolume;
    Player_SetVolume(FVolume);
}

/**
 * This function sets the module wrap property.
 * @param AWrap The new wrap setup.
 */
void __fastcall TMikMod::SetWrap(bool AWrap)
{
    FWrap = AWrap;
    if(FModule != NULL)
    {   // If a module is loaded, set it to the current module
        FModule->wrap = FWrap;
    }
}

/**
 * This function sets the module loop property.
 * @param ALoop The new loop setup.
 */
void __fastcall TMikMod::SetLoop(bool ALoop)
{
    FLoop = ALoop;
    if(FModule != NULL)
    {   // If a module is loaded, set it to the current module
        FModule->loop = FLoop;
    }
}

/**
 * This function sets the module fadeout property.
 * @param AFadeOut The new fadeout setup.
 */
void __fastcall TMikMod::SetFadeOut(bool AFadeOut)
{
    FFadeOut = AFadeOut;
    if(FModule != NULL)
    {   // If a module is loaded, set it to the current module
        FModule->fadeout = FFadeOut;
    }
}

/**
 * This function stops the currently playing module.
 * @see Start
 */
void __fastcall TMikMod::Stop()
{
    Player_SetPosition(0); // Position must be changed before player stops
    FMikModThread->Suspended = true;
    Player_Stop();
}

/**
 * This function starts the specified module playback.
 * @see Stop
 */
void __fastcall TMikMod::Start()
{
    CheckIfOpen();
    Player_Start(FModule);
    Player_SetVolume(FVolume); // Volume must be changed after player is started
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
 * Event call when the module is completed.
 */
void __fastcall TMikMod::ModuleCompleted(System::TObject* Sender)
{
    Stop();
}

/**
 * Returns the song title.
 * @return The song title.
 */
String __fastcall TMikMod::GetSongTitle()
{
    CheckIfOpen();
    return FModule->songname;
}

/**
 * Returns the name of the tracker used to create the song.
 * @return The name of the tracker used to create the song.
 */
String __fastcall TMikMod::GetModType()
{
    CheckIfOpen();
    return FModule->modtype;
}

/**
 * Returns the song comment, if it has one.
 * @return The song comment.
 */
String __fastcall TMikMod::GetComment()
{
    CheckIfOpen();
    return FModule->comment;
}

/**
 * Returns the number of sound positions in the module.
 * @return The number of sound positions in the module.
 */
unsigned short __fastcall TMikMod::GetNumberPos()
{
    CheckIfOpen();
    return FModule->numpos;
}

/**
 * Returns the song position.
 * @return The song position.
 */
unsigned short __fastcall TMikMod::GetPosition()
{
    CheckIfOpen();
    return FModule->sngpos;
}

/**
 * Set the song position.
 * @param The song position.
 */
void __fastcall TMikMod::SetPosition(unsigned short APosition)
{
    Player_SetPosition(APosition);
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
 * This function returns whether sound output is enabled or not.
 * @return Returns true if sound output is enabled, false otherwise.
 */
bool __fastcall TMikMod::GetActive()
{
    return MikMod_Active();
}

/**
 * This function has the same behaviour as feof.
 * @return Returns true if the end-of-file has been reached, false otherwise.
 */
static BOOL GST_READER_Eof(MREADER * reader)
{
    MOD_READER *pReader = reinterpret_cast<MOD_READER *>(reader);

    return (pReader->Stream->Position == pReader->Stream->Size) ? true : false;
}

/**
 * This function copies length bytes of data into dest, and return zero if an error occured, and any nonzero value otherwise. Note that an end-of-file condition will not be considered as an error in this case.
 */
static BOOL GST_READER_Read(MREADER * reader, void *ptr, size_t size)
{
    MOD_READER *pReader = reinterpret_cast<MOD_READER *>(reader);

    pReader->Stream->Read(ptr, static_cast<System::LongInt>(size));

    return 1;
}

/**
 * This function has the same behaviour as fgetc.
 */
static int GST_READER_Get(MREADER * reader)
{
    MOD_READER *pReader = reinterpret_cast<MOD_READER *>(reader);
    char buf;

    pReader->Stream->Read(&buf, 1);

    return static_cast<int>(buf);
}

/**
 * This function has the same behaviour as fseek, with offset 0 meaning the start of the object (module, sample) being loaded.
 * @param offset Number of bytes to offset from whence.
 * @param whence Position used as reference for the offset.
 * @return If successful, the function returns zero. Otherwise, it returns non-zero value.
 */
static int GST_READER_Seek(MREADER * reader, long offset, int whence)
{
    int Result = 0;
    MOD_READER *pReader = reinterpret_cast<MOD_READER *>(reader);

    switch(whence)
    {
        case SEEK_SET:
            pReader->Stream->Position = offset;
            break;
        case SEEK_CUR:
            pReader->Stream->Position += offset;
            break;
        case SEEK_END:
            pReader->Stream->Position += pReader->Stream->Size + offset;
            break;
        default:
            Result = 1;
            break;
    }

    return Result;
}

/**
 * This function has the same behaviour as ftell, with offset 0 meaning the start of the object being loaded.
 */
static long GST_READER_Tell(MREADER * reader)
{
    MOD_READER *pReader = reinterpret_cast<MOD_READER *>(reader);

    return pReader->Stream->Position;
}

