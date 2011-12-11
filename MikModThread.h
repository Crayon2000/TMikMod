//---------------------------------------------------------------------------
#ifndef MikModThreadH
#define MikModThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
namespace Mikmod
{

class TMikModThread : public TThread
{
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
