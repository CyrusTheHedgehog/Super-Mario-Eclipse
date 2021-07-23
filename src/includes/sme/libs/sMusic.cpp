#include "DVD.h"
#include "OS.h"
#include "printf.h"
#include "sms/sound/MSBGM.hxx"

#include "SME.hxx"
#include "sMusic.hxx"

#if 1

using namespace SME::Util;
using namespace SME::Class;

static AudioStreamer::AudioCommand sAudioCommand = AudioStreamer::AudioCommand::NONE;
AudioStreamer AudioStreamer::sInstance = AudioStreamer();

void *mainLoop(void *param) {
  AudioStreamer *streamer = reinterpret_cast<AudioStreamer *>(param);
  AudioStreamer::AudioCommand command = AudioStreamer::AudioCommand::NONE;

  while (true) {
    streamer->update_();
    OSReceiveMessage(&streamer->mMessageQueue, &command, OS_MESSAGE_BLOCK);
    switch (command) {
    case AudioStreamer::AudioCommand::PLAY:
      streamer->play_();
      break;
    case AudioStreamer::AudioCommand::PAUSE:
      streamer->pause_();
      break;
    case AudioStreamer::AudioCommand::STOP:
      streamer->stop_();
      break;
    case AudioStreamer::AudioCommand::SKIP:
      streamer->skip_();
      break;
    case AudioStreamer::AudioCommand::NEXT:
      streamer->next_();
      break;
    case AudioStreamer::AudioCommand::SEEK:
      streamer->seek_();
      break;
    default:
      break;
    }
  }
}

void AudioStreamer::play() {
  sAudioCommand = AudioCommand::PLAY;
  OSSendMessage(&mMessageQueue, &sAudioCommand, OS_MESSAGE_NOBLOCK);
}

void AudioStreamer::pause() {
  sAudioCommand = AudioCommand::PAUSE;
  OSSendMessage(&mMessageQueue, &sAudioCommand, OS_MESSAGE_NOBLOCK);
}

void AudioStreamer::stop() {
  sAudioCommand = AudioCommand::STOP;
  OSSendMessage(&mMessageQueue, &sAudioCommand, OS_MESSAGE_NOBLOCK);
}

void AudioStreamer::skip(f32 delay) {
  sAudioCommand = AudioCommand::SKIP;
  mDelayedTime = delay;
  OSSendMessage(&mMessageQueue, &sAudioCommand, OS_MESSAGE_NOBLOCK);
}

void AudioStreamer::next(f32 delay) {
  sAudioCommand = AudioCommand::NEXT;
  mDelayedTime = delay;
  OSSendMessage(&mMessageQueue, &sAudioCommand, OS_MESSAGE_NOBLOCK);
}

void AudioStreamer::seek(s32 where, JSUStreamSeekFrom whence) {
  sAudioCommand = AudioCommand::SEEK;
  _mWhere = where;
  _mWhence = whence;
  OSSendMessage(&mMessageQueue, &sAudioCommand, OS_MESSAGE_NOBLOCK);
}

void AudioStreamer::seek(f64 seconds, JSUStreamSeekFrom whence) {
  sAudioCommand = AudioCommand::SEEK;

  _mWhence = whence;
  OSSendMessage(&mMessageQueue, &sAudioCommand, OS_MESSAGE_NOBLOCK);
}

bool AudioStreamer::play_() {
  if (isPlaying()) {
    SME_LOG("%s: Already playing audio!\n", SME_FUNC_SIG);
    return false;
  }

  AudioPacket *packet = getCurrentAudio();
  if (packet)
    return packet->exec(mAudioHandle);

  SME_LOG("%s: No audio queued to play!\n", SME_FUNC_SIG);
  return false;
}

bool AudioStreamer::pause_() { 
  AISetStreamPlayState(0);
  return true;  
}

bool AudioStreamer::stop_() {
  DVDCancelStreamAsync(&mAudioHandle.mCmdBlock, 0);
  return true;
}

bool AudioStreamer::skip_() {
  next_();
  return play_();
}

void AudioStreamer::next_() {
  const f32 waitTime = mDelayedTime;
  OSTime startTime = OSGetTime();
  while (OSTicksToSeconds(OSGetTime() - startTime) < waitTime) {
    asm volatile("");
  }
  stop();
  SME_ATOMIC_CODE(mAudioQueue[mAudioIndex] = nullptr;
                  mAudioIndex = (mAudioIndex + 1) % AudioQueueSize;)
}

bool AudioStreamer::seek_() {
  return false;
}

void AudioStreamer::update_() {
  if (!isPlaying())
    return;

  return;
}

bool AudioStreamer::AudioPacket::exec(DVDFileInfo &handle) {
  char buffer[0x40];

  if ((mIdentifier.as_u32 & 0x80000000) && mIdentifier.as_u32 != 0xFFFFFFFF)
    sprintf(buffer, "/AudioRes/Streams/Music/%s.adp", mIdentifier.as_string);
  else
    sprintf(buffer, "/AudioRes/Streams/Music/%lu.adp", mIdentifier.as_u32);

  if (!DVDOpen(buffer, &handle))
    return false;

  DVDPrepareStreamAsync(&handle, 0, 0,
                        (DVDCallback)SME_PORT_REGION(0x803184E4, 0, 0, 0));

  return true;
}

void AudioStreamer::AudioPacket::setLoopPoint(s32 start, size_t length) {
  mLoopStart = start;
  mLoopEnd = start + length;
}

void AudioStreamer::AudioPacket::setLoopPoint(f64 start, f64 length) {
  mLoopStart = start;
  mLoopEnd = start + length;
}

bool Music::isValidBGM(MSStageInfo id) {
  switch (id) {
  case BGM_AIRPORT:
  case BGM_BIANCO:
  case BGM_CASINO:
  case BGM_CORONA:
  case BGM_DELFINO:
  case BGM_DOLPIC:
  case BGM_EVENT:
  case BGM_EXTRA:
  case BGM_MAMMA:
  case BGM_MARE_SEA:
  case BGM_MAREVILLAGE:
  case BGM_MERRY_GO_ROUND:
  case BGM_MONTE_NIGHT:
  case BGM_MONTE_ONSEN:
  case BGM_MONTE_RESCUE:
  case BGM_MONTEVILLAGE:
  case BGM_PINNAPACO:
  case BGM_PINNAPACO_SEA:
  case BGM_RICCO:
  case BGM_SHILENA:
  case BGM_SKY_AND_SEA:
    return true;
  default:
    return false;
  }
}

bool Music::isValidBGM(u32 id) {
  switch (id & 0x3FF) {
#if 0
  case 0x1A:
  case 0x02:
  case 0x1E:
  case 0x15:
  case 0x13:
  case 0x01:
  case 0x1F:
  case 0x12:
  case 0x03:
  case 0x06:
  case 0x14:
  case 0x23:
  case 0x1D:
  case 0x18:
  case 0x22:
  case 0x07:
  case 0x05:
  case 0x04:
  case 0x09:
  case 0x08:
  case 0x21:
#else
  case BGM_AIRPORT & 0xFF:
  case BGM_BIANCO & 0xFF:
  case BGM_CASINO & 0xFF:
  case BGM_CORONA & 0xFF:
  case BGM_DELFINO & 0xFF:
  case BGM_DOLPIC & 0xFF:
  case BGM_EVENT & 0xFF:
  case BGM_EXTRA & 0xFF:
  case BGM_MAMMA & 0xFF:
  case BGM_MARE_SEA & 0xFF:
  case BGM_MAREVILLAGE & 0xFF:
  case BGM_MERRY_GO_ROUND & 0xFF:
  case BGM_MONTE_NIGHT & 0xFF:
  case BGM_MONTE_ONSEN & 0xFF:
  case BGM_MONTE_RESCUE & 0xFF:
  case BGM_MONTEVILLAGE & 0xFF:
  case BGM_PINNAPACO & 0xFF:
  case BGM_PINNAPACO_SEA & 0xFF:
  case BGM_RICCO & 0xFF:
  case BGM_SHILENA & 0xFF:
  case BGM_SKY_AND_SEA & 0xFF:
#endif
    return true;
  default:
    return false;
  }
}

#endif