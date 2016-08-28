//---------------------------------------------------------------------------
#ifndef MainFmxH
#define MainFmxH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
class TMikMod;

class TForm1 : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
    TMikMod* FMikMod;
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    __fastcall virtual ~TForm1(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
