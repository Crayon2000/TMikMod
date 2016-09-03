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
    TProgressBar *ProgressBar1;
    TTimer *Timer1;
    TProgressBar *ProgressBar2;
    TProgressBar *ProgressBar3;
    TProgressBar *ProgressBar4;
    TProgressBar *ProgressBar5;
    TProgressBar *ProgressBar6;
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
    void __fastcall TrackBar1Change(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall tbPlayClick(TObject *Sender);
    void __fastcall tbPauseClick(TObject *Sender);
    void __fastcall tbStopClick(TObject *Sender);
    void __fastcall tbOpenFileClick(TObject *Sender);
    void __fastcall tbMuteClick(TObject *Sender);
private:	// User declarations
    TMikMod *FMikMod;

    void __fastcall Start();
    void __fastcall SetVolumeImageIndex();
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    __fastcall virtual ~TForm1(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
