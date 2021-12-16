#pragma once

#include "types.h"
#include "sms/enemy/BossPakkun.hxx"
#include "sms/actor/SpineBase.hxx"

class TNerveFPSleep : public TNerveBase<TLiveActor>
{
public:
    TNerveFPSleep(){};
    virtual ~TNerveFPSleep();
    virtual bool execute(TSpineBase<TLiveActor> *mSpineBase) const;
};

class TNerveFPBreakSleep : public TNerveBase<TLiveActor>
{
public:
    TNerveFPBreakSleep(){};
    virtual ~TNerveFPBreakSleep();
    virtual bool execute(TSpineBase<TLiveActor> *mSpineBase) const;
};


class TNerveFPWait : public TNerveBPWait
{

public:
    TNerveFPWait(){};
    virtual ~TNerveFPWait(){};
    virtual bool execute(TSpineBase<TLiveActor> *mSpineBase) const;
};
class TNerveFPTakeOff : public TNerveBPTakeOff
{
public:
    TNerveFPTakeOff(){};
    virtual ~TNerveFPTakeOff();
    virtual bool execute(TSpineBase<TLiveActor> *mSpineBase) const;
};

class TNerveFPFly : public TNerveBPFly
{
public:
    TNerveFPFly(){};
    virtual ~TNerveFPFly();
    virtual bool execute(TSpineBase<TLiveActor> *mSpineBase) const;
};

class TFPTornado : public TBPTornado
{
public:
    TFPTornado(TBossPakkun *parent, const char *);
    virtual ~TFPTornado(){};
    virtual void perform(u32, JDrama::TGraphics *) override;
};

class TFireyPetey : public TBossPakkun
{
public:
    TFireyPetey(const char *);
    virtual ~TFireyPetey();
    virtual void init(TLiveManager *) override;
    virtual bool receiveMessage(THitActor *, u32) override;

    short numTornados;
};