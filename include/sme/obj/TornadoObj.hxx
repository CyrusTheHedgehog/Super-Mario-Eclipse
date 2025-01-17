#pragma once

#include "JDrama/JDRGraphics.hxx"
#include "JSU/JSUMemoryStream.hxx"
#include "sms/actor/LiveActor.hxx"
#include "sms/actor/Mario.hxx"
#include "sms/mapobj/MapObjRail.hxx"
#include "types.h"

class TTornadoMapObj : public TRailMapObj {
  struct BlownEntry {
    TLiveActor *mActor;
    f32 mBlowProgress;

    bool isActor(TLiveActor *actor) const { return mActor == actor; }
  };

public:
  TTornadoMapObj(const char *name);
  virtual ~TTornadoMapObj();

  virtual void load(JSUMemoryInputStream &) override;
  virtual void perform(u32, JDrama::TGraphics *) override;
  virtual void control() override;

  f32 getStrength() const { return mBlowStrength; }
  void setStrength(f32 strength) { mBlowStrength = strength; }

  bool isBlowing(TLiveActor *actor) {
    for (int i = 0; i < mNumObjs; ++i) {
      TLiveActor *blowObj = reinterpret_cast<TLiveActor *>(mCollidingObjs[i]);
      if (blowObj == actor) {
        return true;
      }
    }
    return false;
  }

  bool blowUp(TLiveActor *actor);
  bool blowUp(TMario *actor);

private:
  f32 mBlowStrength;
  bool mHasFinalPush;
  TVec3f mTrueRotation;
};

extern ObjData tornadoData;