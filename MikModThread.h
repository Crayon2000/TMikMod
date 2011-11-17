//---------------------------------------------------------------------------
#ifndef MikModThreadH
#define MikModThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TMikModThread : public TThread
{
private:
protected:
    void __fastcall Execute();
public:
    __fastcall TMikModThread();
};
//---------------------------------------------------------------------------
#endif
