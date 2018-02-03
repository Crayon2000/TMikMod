//---------------------------------------------------------------------------
#pragma hdrstop

#include "MikModThread.h"
#include "include/mikmod.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

/**
 * Constructor.
 */
__fastcall TMikModThread::TMikModThread() : Classes::TThread(true)
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
        if(Player_Active() == true)
        {
            MikMod_Update();
            Sleep(10);
        }
    }
}

