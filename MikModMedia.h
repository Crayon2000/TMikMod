//---------------------------------------------------------------------------
#ifndef MikModMediaH
#define MikModMediaH
//---------------------------------------------------------------------------
#include <FMX.Media.hpp>
//---------------------------------------------------------------------------
class TMikMod;

/**
 * Media class for TMikMod.
 */
class TMikModMedia : public Media::TMedia
{
    typedef Media::TMedia inherited;

private:
    TMikMod* FMikMod;

protected:
    virtual __int64 __fastcall GetDuration(void);
    virtual __int64 __fastcall GetCurrent(void);
    virtual void __fastcall SetCurrent(const __int64 Value);
    virtual System::Types::TPointF __fastcall GetVideoSize(void);
    virtual TMediaState __fastcall GetMediaState(void);
    virtual float __fastcall GetVolume(void);
    virtual void __fastcall SetVolume(const float Value);
    virtual void __fastcall UpdateMediaFromControl(void) {inherited::UpdateMediaFromControl();};
    virtual void __fastcall DoPlay(void);
    virtual void __fastcall DoStop(void);

public:
    __fastcall virtual TMikModMedia(const System::UnicodeString AFileName);
    __fastcall virtual ~TMikModMedia(void);
};

/**
 * Media codec for TMikMod.
 */
class TMikModMediaCodec : public Media::TCustomMediaCodec
{
public:
    inline __fastcall TMikModMediaCodec(void) : TCustomMediaCodec() { }
    inline __fastcall virtual ~TMikModMediaCodec(void) { }

    virtual TMedia* __fastcall CreateFromFile(const System::UnicodeString AFileName);
};
//---------------------------------------------------------------------------
#endif