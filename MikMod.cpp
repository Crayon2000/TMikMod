//---------------------------------------------------------------------------
#include <map>
#pragma hdrstop

#include "MikMod.h"
#include "MikModThread.h"
#include "mikmod_build.h"
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
 * @param ADriver The driver to use. If not specify, md_Windows will be used.
 */
__fastcall TMikMod::TMikMod(TModuleDriver ADriver) :
    FModule(NULL),
    FVolume(128)
{
    FMikModThread = new TMikModThread();

    std::map<TModuleDriver, MDRIVER*> DriverList;
    DriverList[md_DirectSound] = &drv_ds;
    DriverList[md_Windows] = &drv_win;
    DriverList[md_MacOSX] = &drv_osx;
    DriverList[md_NoSound] = &drv_nos;
    DriverList[md_Raw] = &drv_raw;
    DriverList[md_StandardOutput] = &drv_stdout;
    DriverList[md_WAV] = &drv_wav;

    // Register a specific driver
    MikMod_RegisterDriver(DriverList[ADriver]);
    // Register all the module loaders
    MikMod_RegisterAllLoaders();
    // Only one device is used, this is needed to use command line
    md_device = 1;

    AnsiString CommandLine;
    if(ADriver == md_DirectSound)
    {
        CommandLine = "globalfocus"; // Play if window does not have the focus
    }

    // Initialize the library
    if(MikMod_Init(CommandLine.c_str()))
    {
        throw(Exception("L'initialisation de MikMod a échoué."));
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
