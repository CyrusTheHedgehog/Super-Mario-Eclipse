#include "printf.h"
#include "sms/sound/MSBGM.hxx"

#include "SME.hxx"

using namespace SME;
using namespace SME::Class;

static bool startStreamedBGM(MSStageInfo musicID, bool loopMusic) {
  char buffer[0x20];
  DVDFileInfo *handle = (DVDFileInfo *)SME_PORT_REGION(0x803FDB7C, 0, 0, 0);

  sprintf(buffer, "/AudioRes/Streams/Music/%lu.adp", (u32)(musicID & 0x3FF));

  if (!DVDOpen(buffer, handle))
    return false;

  MSBgm::stopBGM(gStageBGM, 32);
  DVDPrepareStreamAsync(handle, 0, 0,
                        (DVDCallback)SME_PORT_REGION(0x803184E4, 0, 0, 0));

  if (!loopMusic)
    DVDStopStreamAtEndAsync(&handle->mCmdBlock, 0);
  return true;
}

static bool startStreamedSFX(u32 sfxID) {
  char buffer[0x20];
  DVDFileInfo *handle = (DVDFileInfo *)0x803FDB7C;

  sprintf(buffer, "/AudioRes/Streams/SFX/%lu.adp", (sfxID & 0x3FF));

  if (!DVDOpen(buffer, handle))
    return false;

  DVDPrepareStreamAsync(handle, 0, 0,
                        (DVDCallback)SME_PORT_REGION(0x803184E4, 0, 0, 0));
  DVDStopStreamAtEndAsync(&handle->mCmdBlock, 0);

  return true;
}

// 0x80016998
u32 SME::Patch::Music::setIsValid(MSStageInfo musicID) {
  TGlobals::sIsAudioStreamAllowed = SME::Util::Music::isValidBGM(static_cast<MSStageInfo>(musicID & 0x800103FF));
  return musicID & 0x3FF;
}

// 0x80016ABC
JAISound *SME::Patch::Music::initMusic(JAISound *jai) {
  DVDFileInfo *handle = (DVDFileInfo *)SME_PORT_REGION(0x803FDB7C, 0, 0, 0);

  if (!TGlobals::sIsAudioStreaming &&
      (TStageParams::sStageConfig->mMusicID.get() & 0x400))
    TGlobals::sIsAudioStreaming = startStreamedBGM(
        (MSStageInfo)TStageParams::sStageConfig->mMusicID.get(), true);

  if (TGlobals::sIsAudioStreaming && !TGlobals::sIsAudioStreamAllowed) {
    DVDCancelStreamAsync(&handle->mCmdBlock, 0);
    TGlobals::sIsAudioStreaming = false;
  }

  return jai;
}

// 0x80016948
void SME::Patch::Music::stopMusicOnStop() {
  DVDFileInfo *handle = (DVDFileInfo *)SME_PORT_REGION(0x803FDB7C, 0, 0, 0);

  if (TGlobals::sIsAudioStreaming) {
    DVDCancelStreamAsync(&handle->mCmdBlock, 0);
    TGlobals::sIsAudioStreaming = false;
  }
}

// 0x802A670C
void SME::Patch::Music::stopMusicOnStageExit(TMarioGamePad *gamepad) {
  DVDFileInfo *handle = (DVDFileInfo *)SME_PORT_REGION(0x803FDB7C, 0, 0, 0);

  if (TGlobals::sIsAudioStreaming) {
    DVDCancelStreamAsync(&handle->mCmdBlock, 0);
    TGlobals::sIsAudioStreaming = false;
  }
  reset__9RumbleMgrFv(gamepad);
}