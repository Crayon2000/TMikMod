//---------------------------------------------------------------------------
#ifndef MainFmxH
#define MainFmxH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
//---------------------------------------------------------------------------
class TMikMod;

class TForm1 : public TForm
{
__published:	// IDE-managed Components
    void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
    TMikMod* FMikMod;
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
