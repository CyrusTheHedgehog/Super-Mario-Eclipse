#pragma once

#include "AI.h"
#include "DVD.h"
#include "sms/JSystem/JSU/JSUStream.hxx"
#include "sms/sound/MSBGM.hxx"

using namespace SME::Util;

constexpr size_t AudioQueueSize = 4;
constexpr size_t AudioStackSize = 0x4000;

extern void *mainLoop(void *param);

namespace SME::Class {

class AudioStreamer : public OSThread {
public:
  enum class AudioCommand : u8 {
    NONE,
    PLAY,
    PAUSE,
    STOP,
    SKIP,
    NEXT,
    SEEK,
  };

  struct AudioPacket {
    union Identifier {
      u32 as_u32;
      char *as_string;
    };

    AudioPacket(u32 id)
        : mLoopStart(-1), mLoopEnd(-1) {
      mIdentifier.as_u32 = id;
    }

    AudioPacket(char *file)
        : mLoopStart(-1), mLoopEnd(-1) {
      mIdentifier.as_string = file;
    }

    bool exec(DVDFileInfo &handle);
    void setLoopPoint(s32 start, size_t length);
    void setLoopPoint(f64 start, f64 length);

    Identifier mIdentifier;
    s32 mLoopStart;
    s32 mLoopEnd;
  };

private:
  AudioStreamer() : mAudioIndex(0), mDelayedTime(0.0f) {
    mAudioStack = static_cast<u8 *>(Memory::malloc(AudioStackSize, 32));
    OSInitMessageQueue(&mMessageQueue, mMessageList, sizeof(mMessageList));
    OSCreateThread(this, mainLoop, this, mAudioStack + AudioStackSize,
                    AudioStackSize, 18, OS_THREAD_ATTR_DETACH);
    OSResumeThread(this);
  }

  AudioStreamer(AudioStreamer const &) = delete;
  void operator=(AudioStreamer const &) = delete;

public:
  static AudioStreamer &getInstance() {
    static AudioStreamer instance;
    return instance;
  }

  ~AudioStreamer() { OSCancelThread(this); }


  void queueAudio(AudioPacket &packet);
  void setLooping(bool loop);
  bool isPlaying() const { return AIGetStreamPlayState(); }
  AudioPacket *getCurrentAudio() const { return mAudioQueue[0]; }

  void play();
  void pause();
  void stop();
  void skip(f32 delay);
  void next(f32 delay);
  void seek(s32 where, JSUStreamSeekFrom whence);
  void seek(f64 seconds, JSUStreamSeekFrom whence);

  DVDFileInfo mAudioHandle;
  OSMessageQueue mMessageQueue;
  OSMessage mMessageList[16];

  bool play_();
  bool pause_();
  bool stop_();
  bool skip_();
  void next_();
  bool seek_();
  void update_();

private:

  AudioPacket *mAudioQueue[AudioQueueSize];
  u8 *mAudioStack;
  s32 mAudioIndex;
  f32 mDelayedTime;

  s32 _mWhere;
  JSUStreamSeekFrom _mWhence;

  static bool sInitialized;
  static AudioStreamer sInstance;
};

} // namespace SME::Class

namespace SME::Util::Music {

bool isValidBGM(u32 id);
bool isValidBGM(MSStageInfo id);

} // namespace SME::Util::Music