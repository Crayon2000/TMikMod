//---------------------------------------------------------------------------
#ifndef MikModThreadH
#define MikModThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
namespace Mikmod
{

/**
 * The thread where TMikMod is updated.
 */
class TMikModThread : public Classes::TThread
{
    typedef Classes::TThread inherited;

private:
    TNotifyEvent FOnModuleCompleted;
    void __fastcall CallModuleCompleted(void);
protected:
    void __fastcall Execute();
    void __fastcall DoModuleCompleted(void);
public:
    __fastcall TMikModThread(bool ACreateSuspended = true);
    inline __fastcall virtual ~TMikModThread(void) { }

    __property TNotifyEvent OnModuleCompleted = {read=FOnModuleCompleted, write=FOnModuleCompleted};
};

}   /* namespace Mikmod */

using namespace Mikmod;

//---------------------------------------------------------------------------
#endif
