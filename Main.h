//---------------------------------------------------------------------------
#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TMikMod;

class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TEdit *txtModuleType;
    TTimer *Timer1;
    TToolBar *ToolBar1;
    TToolButton *tbPlay;
    TToolButton *tbPause;
    TToolButton *tbStop;
    TTrackBar *TrackBar1;
    TImageList *ImageList1;
    TToolButton *tbSeparator1;
    TOpenDialog *OpenDialog1;
    TToolButton *tbOpenFile;
    TToolButton *tbSeparator2;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *txtSongTitle;
    TLabel *Label3;
    TMemo *memoComment;
    TToolButton *tbMute;
    TPanel *Panel1;
    TToolButton *tbRepeat;
    void __fastcall TrackBar1Change(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall tbPlayClick(TObject *Sender);
    void __fastcall tbPauseClick(TObject *Sender);
    void __fastcall tbStopClick(TObject *Sender);
    void __fastcall tbOpenFileClick(TObject *Sender);
    void __fastcall tbMuteClick(TObject *Sender);
    void __fastcall tbRepeatClick(TObject *Sender);
private:	// User declarations
    TMikMod *FMikMod;
    DynamicArray<TProgressBar *> FBars;

    void __fastcall Start();
    void __fastcall SetVolumeImageIndex();
    int __fastcall ImageListAddRes(TImageList* AImageList, String Identifier);
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    __fastcall virtual ~TForm1(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
