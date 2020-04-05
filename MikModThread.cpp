//---------------------------------------------------------------------------
#pragma hdrstop

#include "MikModThread.h"
#include "include/mikmod.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

/**
 * Module states.
 */
enum class TModuleState : unsigned char
{
    Playing, /**< The module is playing. */
    Stopped, /**< The module is stopped. */
    Completed /**< The module is completed. */
};

/**
 * Constructor.
 * @param ACreateSuspended If set to true, Execute is not called until after the Start method is called.
 *                         If set to false, Execute is called immediately after the constructor.
 *                         The default value is true.
 */
__fastcall TMikModThread::TMikModThread(bool ACreateSuspended) :
    Classes::TThread(ACreateSuspended)
{
}

/**
 * This routine should be called on a regular basis to update the sound.
 * @note The sound output buffer is filled each time this function is called; if you use a large buffer, you don't need to call this routine as frequently as with a smaller buffer, but you get a bigger shift between the sound being played and the reported state of the player, since the player is always a buffer ahead of the playback.
 */
void __fastcall TMikModThread::Execute()
{
#if defined(_WINDOWS_)
    TThread::CurrentThread->NameThreadForDebugging(System::UnicodeString("TMikModThread"), TThread::CurrentThread->ThreadID);
#endif
    while(Terminated == false)
    {
        TModuleState LState;
        MikMod_Lock();
        const MODULE* pf = Player_GetModule();
        if(pf == NULL)
        {
            LState = TModuleState::Stopped;
        }
        else if(pf->sngpos >= pf->numpos)
        {
            LState = TModuleState::Completed;
        }
        else
        {
            LState = TModuleState::Playing;
        }
        MikMod_Unlock();

        if(LState == TModuleState::Playing)
        {
            MikMod_Update();
            Sleep(10);
        }
        else if(LState == TModuleState::Completed)
        {
            DoModuleCompleted();
        }
        else
        {
            Sleep(1);
        }
    }
}

/**
 * Generates an OnModuleCompleted event.
 */
void __fastcall TMikModThread::CallModuleCompleted(void)
{
    if(FOnModuleCompleted)
    {
        OnModuleCompleted(this);
    }
}

/**
 * Generates an OnModuleCompleted event within the main thread.
 */
void __fastcall TMikModThread::DoModuleCompleted(void)
{
    if(FOnModuleCompleted)
    {
        Synchronize(CallModuleCompleted);
    }
}

