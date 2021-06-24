#pragma once

#include "types.h"
#include "sms/JSystem/JGeometry.hxx"
#include "sms/actor/SpineEnemy.hxx"
#include "sms/actor/HitActor.hxx"
#include "sms/JSystem/JDrama.hxx"



class TBossPakkun : public TSpineEnemy
{

private:
class TBPPolDrop: public THitActor
{

public:

    TBPPolDrop(TBossPakkun *parentPakkun,char *name);
    virtual void perform(u32, JDrama::TGraphics *) override;
    void move();

    TBossPakkun *parentPakkun;            //0x0068
    f32 _00[3];                           //0x006c
    u32 _01[4];                           //0x0078
    f32 _02;                              //0x0088

};

public:

    TBossPakkun(const char *);
    ~TBossPakkun();

    virtual void perform(u32, JDrama::TGraphics *) override;
    virtual bool receiveMessage(THitActor *, u32) override;
    virtual void init(TLiveManager *) override;
    virtual void setGroundCollision() override;
    virtual void kill() override;

    void changeBck(u32);
    void launchPolDrop();
    void getHipDropDamage() const;
    void showMessage(u32);
    void rumblePad(int, const JGeometry::TVec3<float> &);
    void checkMarioRiding();
    


    void* tWalker;              //0x0088
    f32 _00;                    //0x0154
    TBPPolDrop *mPollutionDrop; //0x0158
    void*  TBPVomit;            //0x015C
    void*  TBPTornado;          //0x0160
    void*  TNavel;              //0x0164
    void*  THeadHit;            //0x0168
    s8  _02;                    //0x016C
    u32 _03[0x18 / 3];          //0x0170
    s8  _04;                    //0x017C
    MActor* peteyMActorData;    //0x0180
    f32 _06;                    //0x0184
    u32 _07;                    //0x0188
    void* TWaterEmitInfo;       //0x018C
    s8  _08;                    //0x0190
    u32 _09;                    //0x01B8
    s8  _10;                    //0x01BC
    u32 _11;                    //0x01C0
    s8  _12;                    //0x01C4
    u32 _13;                    //0x01CC          
};


