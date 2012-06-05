//---------------------------------------------------------------------------
#ifndef MikModThreadH
#define MikModThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
namespace Mikmod
{

class TMikModThread : public Classes::TThread
{
    typedef Classes::TThread inherited;

private:
protected:
    void __fastcall Execute();
public:
    __fastcall TMikModThread();
};

}   /* namespace Mikmod */

using namespace Mikmod;

//---------------------------------------------------------------------------
#endif
