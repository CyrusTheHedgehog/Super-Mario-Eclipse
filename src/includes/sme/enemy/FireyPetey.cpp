#include "FireyPetey.hxx"

#include "SME.hxx"
//Should Be TNerveBase<TLiveActor>
extern int instance$3889;
extern bool init$3891;

//BPWait
extern int instance$3625;
extern bool init$3627;

//BPVomit
extern TNerveBase<TLiveActor> instance$3678;

//Globals
static TNerveFPSleep sleep;
static TNerveFPBreakSleep breakSleep;
static TNerveFPTakeOff takeoff;
static TNerveFPFly fly;
static TNerveFPWait wait;

TFireyPetey::TFireyPetey(const char *test) : TBossPakkun(test)
{
  numTornados = 3;
}

TFPTornado::TFPTornado(TBossPakkun *parent, const char *name) : TBPTornado::TBPTornado(parent, name)
{
  SME_LOG("Hi\n");
}

void TFPTornado::perform(u32 flags, JDrama::TGraphics *graphics)
{
  if (((*(int **)(this->mParent))[0x3c] & 1U) != 0)
  {
    return;
  }
  if (this->_98 == 0)
  {
    return;
  }
  TBPTornado::perform(flags, graphics);
  stamp__17TPollutionManagerFUsffff(gpPollution, 1, mPosition.x, mPosition.y,
                                    mPosition.z, 384.0f);
}

TNerveFPSleep::~TNerveFPSleep() {}

TNerveFPBreakSleep::~TNerveFPBreakSleep() {}

TNerveFPTakeOff::~TNerveFPTakeOff() {}

TNerveFPFly::~TNerveFPFly() {}

bool TNerveFPWait::execute(TSpineBase<TLiveActor> *spine) const
{
  // TBossPakkun *target = reinterpret_cast<TBossPakkun *>(spine->mTarget);
  // // if (target->mTornado->_98 == 0) {
  // //             {
  // //               *(int ***)(((int*)spine)[3] + spine->mVTableIndex++) = (int**)&fly;
  // //             }
  // //             if (init$3627 == '\0') {
  // //               instance$3625 = &TNerveBPWait::__vt;
  // //               Runtime.PPCEABI.H::__register_global_object
  // //                         (&instance$3625,::TNerveBPWait::~TNerveBPWait,(undefined4 *)&@3626);
  // //               init$3627 = '\x01';
  // //             }
  // //             if ((int)param_2[2] < (int)param_2[1]) {
  // //               param_2[3][(int)param_2[2]] = (int)&instance$3625;
  // //               param_2[2] = (int *)((int)param_2[2] + 1);
  // //             }
  // //             if (init$3709 == '\0') {
  // //               instance$3707 = &TNerveBPTornado::__vt;
  // //               Runtime.PPCEABI.H::__register_global_object
  // //                         (&instance$3707,::TNerveBPTornado::~TNerveBPTornado,(undefined4 *)&@3708);
  // //               init$3709 = '\x01';
  // //             }
  // //             if ((int)param_2[2] < (int)param_2[1]) {
  // //               param_2[3][(int)param_2[2]] = (int)&instance$3707;
  // //               param_2[2] = (int *)((int)param_2[2] + 1);
  // //             }
  // // }
  // if (target->mTornado->_98 == 0) {
  // SME_LOG("Frother\n");
  // if (spine->mVTableIndex < spine->_00) {
  // *(int ***)(((int*)spine)[3] + spine->mVTableIndex++) = (int**)&takeoff;
  // }
  // //   if (spine->mVTableIndex < spine->_00) {
  // // *(int ***)(((int*)spine)[3] + spine->mVTableIndex++) = (int**)NerveGetByIndex__Fi(0x1d);
  // //   }
  // return true;
  // }else{
  //   return false;
  // }
  // if (spine->mVTableIndex < spine->_00)
  // {
  //   *(int ***)(((int *)spine)[3] + spine->mVTableIndex++) = (int **)NerveGetByIndex__Fi(0x1d);
  // }
  // if (spine->mVTableIndex < spine->_00) {
  // *(int ***)(((int*)spine)[3] + spine->mVTableIndex++) = (int**)&takeoff;
  // }]
  TFireyPetey *target = reinterpret_cast<TFireyPetey *>(spine->mTarget);
  bool result = TNerveBPWait::execute(spine);
  SME_LOG("Tornados left 0x%08x\n", *(int ***)(((int *)spine)[3] + 4));
  if (*(int ***)(((int *)spine)[3] + 4) == (int **)&instance$3678)
  {
    SME_LOG("Tornados left 0x%08x\n", target->numTornados);
    if (target->numTornados > 0)
    {
      if (spine->mVTableIndex < spine->_00)
      {
        *(int ***)(((int *)spine)[3]) = (int **)&wait;
        *(int ***)(((int *)spine)[3] + 4) = (int **)NerveGetByIndex__Fi(0x1d);
      }
      target->numTornados--;
    }
    else
    {
      if (spine->mVTableIndex < spine->_00)
      {
        *(int ***)(((int *)spine)[3]) = (int **)&takeoff;
      }
      target->numTornados = 3;
    }
  }
  return result;
}

bool TNerveFPFly::execute(TSpineBase<TLiveActor> *spine) const
{
  if (spine->mStateTimer % 100 == 99)
  {
    TFireyPetey *target = reinterpret_cast<TFireyPetey *>(spine->mTarget);
    // // target->launchPolDrop();
    // TWaterBalloon *sWaterBalloon = target->sWaterBalloon;

    // if (!sWaterBalloon)
    // {
    //   sWaterBalloon = new TWaterBalloon("waterballoon");
    //   sWaterBalloon->initAndRegister("waterballoon");
    // }
    // else
    // {
    //   sWaterBalloon->kill();
    // }

    // sWaterBalloon->mPosition.set(target->mPosition);
    // sWaterBalloon->appear();
  }
  SME_LOG("0x%08x\n", spine->mStateTimer);
  return TNerveBPFly::execute(spine);
}

bool TNerveFPTakeOff::execute(TSpineBase<TLiveActor> *spine) const
{
  TFireyPetey *target = reinterpret_cast<TFireyPetey *>(spine->mTarget);
  JGeometry::TVec3<f32> mPosition;
  if (spine->mStateTimer % 100 == 99)
  {
    // target->launchPolDrop();
    // target->launchPolDrop();



    TKukkuBall *mKukkuBall = target->mKukkuBall;
    JGeometry::TVec3<f32> step(0.000f, 1.000f, 0.000f);
    mKukkuBall->mPosition.set(target->mPosition);
    mKukkuBall->mVelocity.set(step);
    mKukkuBall->mObjectType = mKukkuBall->mObjectType & 0xfffffffe;
    mKukkuBall->unk1 = mKukkuBall->unk1 & 0xfffffffe;
  }

  // if (checkCurBckFromIndex__6MActorFi(target->mActorData,0xb))
  // {
  //   target->mPosition.y+= 5.0f;
  //   SME_LOG("0x%08x\n", offsetof(TSpineEnemy,_104))
  //   if (target->_104 == 0)
  //   {
  //     mPosition = target->_108;
  //   }
  //   else
  //   {
  //     mPosition = *reinterpret_cast<JGeometry::TVec3<f32> *>(*(int *)(this + 0x104) + 0x10);
  //   }
  //   if (mPosition.y < target->mPosition.y)
  //   {
  //     SME_LOG("0x%08x\n", spine->mStateTimer);
  //     if (target->mGraphTracer->mGraph != 0)
  //     {
  //       if (spine->mVTableIndex < spine->_00)
  //       {
  //         *(int ***)(((int*)spine)[3] + spine->mVTableIndex++) = (int**)&fly;
  //       }
  //     }
  //     return true;
  //   }
  // }
  return TNerveBPTakeOff::execute(spine);
}

bool TNerveFPBreakSleep::execute(TSpineBase<TLiveActor> *spine) const
{
  TBossPakkun *target = reinterpret_cast<TBossPakkun *>(spine->mTarget);
  if (spine->mStateTimer == 0)
  {
    target->changeBck(0x0E);
    MSBgm::stopTrackBGMs('\a', 10);
  }
  if (!target->mActorData->curAnmEndsNext(0, 0))
  {
    return false;
  }
  if (spine->mVTableIndex < spine->_00)
  {

    *(int ***)(((int *)spine)[3] + spine->mVTableIndex++) = (int **)&takeoff;
    // spine->mVTableIndex++;
    // spine->_01[spine->mVTableIndex] = takeoff;
    // spine->mVTableIndex++;
    // spine->mPrevVTable = spine->mCurVTable;
    // spine->mStateTimer = -1;
    // spine->mCurVTable = &takeoff;
  }
  return true;
}

bool TNerveFPSleep::execute(TSpineBase<TLiveActor> *param1) const
{
  if (param1->mStateTimer == 0)
  {
    reinterpret_cast<TBossPakkun *>(param1->mTarget)->changeBck(0x17);
  }
  return 0;
}

void TFireyPetey::init(TLiveManager *param1)
{
  TBossPakkun::init(param1);
  TBossPakkun *target = reinterpret_cast<TBossPakkun *>(this);
  target->mTornado = new TFPTornado(this, "<TFPTornado>\n");
  SME_LOG("flydrop\n");
  mSpineBase->mVTableIndex = 0;
  mSpineBase->mStateTimer = 0;
  mSpineBase->mCurVTable = &sleep;
  mSpineBase->mPrevVTable = 0;

  SME_LOG("Redirecting Nerves\n");
  // Should be instance$3889 = fly;
  instance$3889 = *(int *)&fly;
  init$3891 = true;

  instance$3625 = *(int *)&wait;
  init$3627 = true;


  mKukkuBall = new TKukkuBall("lavaDrop\n");
  mKukkuBall->mActorData = mMActorKeeper->createMActor("pollut_ball.bmd", 3);
  mKukkuBall->init();
  SME_LOG("initiallizing LavaDrops\n");
  SME_LOG("Object at 0x%08x\n", &mKukkuBall);
}

void TFireyPetey::perform(u32 flags, JDrama::TGraphics *graphics)
{
  TBossPakkun::perform(flags, graphics);
  if (mKukkuBall != nullptr)
  {
    mKukkuBall->perform(flags, graphics);
  }
}

bool TFireyPetey::receiveMessage(THitActor *reciever, u32 param2)
{
  if (reciever->mObjectID == 0x1000001 && mSpineBase->mCurVTable == &sleep)
  {
    mSpineBase->mPrevVTable = mSpineBase->mCurVTable;
    mSpineBase->mStateTimer = 0;
    mSpineBase->mCurVTable = &breakSleep;
    return false;
  }
  // SME_LOG("0x%08x\n",reciever->mObjectID);
  return TBossPakkun::receiveMessage(reciever, param2);
}

// Because Screw hardcoded behaviours
SME_WRITE_32(SME_PORT_REGION(0x80095c54, 0x8008f2f4, 0x0, 0x0), 0x60000000);

TFireyPetey::~TFireyPetey()
{
}
