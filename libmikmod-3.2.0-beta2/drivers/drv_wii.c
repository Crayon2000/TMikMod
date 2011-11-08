/*	MikMod sound library
*/

/*==============================================================================

  Driver for Wii Homebrew

==============================================================================*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <string.h>
#include <malloc.h>
#include "mikmod_internals.h"

static int sLen = 0;
static SBYTE *buf=NULL;

static BOOL Wii_IsThere(void)
{
	return 1;
}

static void Wii_Update(void)
{
	if(buf)
	{
		// Render temporarily, then expand
		SBYTE *tbuf= (SBYTE *)malloc(sLen / 2);
		
		// Grab it
		VC_WriteBytes(tbuf,sLen / 2);
		
		// Format it
		int i;
		
		for(i = 0; i < sLen / 4; i++)
		{
			memcpy(buf + (i << 2), tbuf + (i << 1), 2);
			memcpy(buf + (i << 2) + 2, tbuf + (i << 1), 2);
		}
		
		// Free it
		free(tbuf);
	}
}

void setBuffer(int16_t *buffer, int renderSamples)
{
	buf = (SBYTE *)buffer;
	sLen = renderSamples * 4;
}

MIKMODAPI MDRIVER drv_wii={
	NULL,
	"Wii",
	"Wii Driver v1.0",
	0,255,
	"nosound",
	NULL,
	NULL,
	Wii_IsThere,
	VC_SampleLoad,
	VC_SampleUnload,
	VC_SampleSpace,
	VC_SampleLength,
	VC_Init,
	VC_Exit,
	NULL,
	VC_SetNumVoices,
	VC_PlayStart,
	VC_PlayStop,
	Wii_Update,
	NULL,
	VC_VoiceSetVolume,
	VC_VoiceGetVolume,
	VC_VoiceSetFrequency,
	VC_VoiceGetFrequency,
	VC_VoiceSetPanning,
	VC_VoiceGetPanning,
	VC_VoicePlay,
	VC_VoiceStop,
	VC_VoiceStopped,
	VC_VoiceGetPosition,
	VC_VoiceRealVolume
};
