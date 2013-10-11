//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#include "MikModThread.h"
#include "mikmod_build.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

/**
 * Constructor.
 */
__fastcall TMikModThread::TMikModThread() : Classes::TThread(true)
{
    TThread::CurrentThread->NameThreadForDebugging(System::AnsiString("TMikModThread"), TThread::CurrentThread->ThreadID);
}

/**
 * This routine should be called on a regular basis to update the sound.
 * @note The sound output buffer is filled each time this function is called; if you use a large buffer, you don't need to call this routine as frequently as with a smaller buffer, but you get a bigger shift between the sound being played and the reported state of the player, since the player is always a buffer ahead of the playback.
 */
void __fastcall TMikModThread::Execute()
{
    while(!Terminated)
    {
        if(Player_Active())
        {
            MikMod_Update();
            Sleep(10);
        }
    }
}
