#include "types.h"
#include "MTX.h"

#include "sms/actor/Mario.hxx"
#include "sms/enemy/EnemyMario.hxx"
#include "sms/npc/BaseNPC.hxx"

#include "SME.hxx"

//0x8024D2C4
/*
TMario* updateMario(TMario* player) {
    if (SME::TGlobals::sGlobals.PlayerData.mCurPlayerID[i] != prevPlayerID) {
        u32* marioVolumeData = (u32*)getVolume__13JKRFileLoaderFPCc("mario");
        u32* marioDataArray = (u32*)*(u32*)ARCBufferMario;

        if (marioDataArray[SME::TGlobals::sGlobals.PlayerData.mCurPlayerID[i]]) {
            __dt__13JKRMemArchiveFv(marioVolumeData);
            __ct__13JKRMemArchiveFPvUl15JKRMemBreakFlag(marioVolumeData, marioDataArray[i], 0, 0);

            initModel__6TMarioFv(player);
            initMirrorModel__6TMarioFv(player);
        }
    }
    asm("lhz 0, 0x114 (3)");
    return player;
}
*/


/* MASTER MARIO.CPP UPDATER */

u32 SME::Patch::Mario::updateContexts(TMario *player)
{
    SME::Class::TPlayerParams *playerParams = SME::TGlobals::sGlobals.getPlayerParams(player);

    if (!playerParams->isMario() || !playerParams->isInitialized())
    {
        playerParams->mIsClimbTired = false;
        return 1;
    }

    bool checkClimbContext = false;

    if ((player->mState & static_cast<u32>(TMario::State::AIRBORN)) == 0 && (player->mState & 0x1C0) != 320)
        player->mSubStateTimer = 0;
    else if ((player->mState & 0x1C0) == 320)
    {
        if ((player->mState & 0x200000) != 0 && player->mRoofTriangle &&
            player->mRoofTriangle->mCollisionType != 266)
            checkClimbContext = player->mState != static_cast<u32>(TMario::State::HANG);
        else if ((player->mState & 0x200000) == 0 && 
                 player->mWallTriangle &&
                 player->mWallTriangle->mCollisionType != 266)
            checkClimbContext = player->mState != static_cast<u32>(TMario::State::HANG);

        if (checkClimbContext)
        {
            if (player->mSubStateTimer == player->mDeParams.mNoFreezeTime.get() / 5)
            {
                player->mActionState |= 0x8000;
                player->mSubStateTimer = 0;
                playerParams->mIsClimbTired = false;
            }
            else
            {
                if (SME::Util::Math::lerp<f32>(0.0f, 1.0f, static_cast<f32>(player->mSubStateTimer) /
                                              player->mDeParams.mNoFreezeTime.get()) > 0.9f)
                {
                    if (!playerParams->mIsClimbTired)
                        startVoice__6TMarioFUl(player, static_cast<u32>(TMario::Voice::FALL_LEDGE_GRAB));
                    
                    playerParams->mIsClimbTired = true;
                }
                else
                    playerParams->mIsClimbTired = false;

                player->mSubStateTimer += 1;
            }
        }

        if (player->mCeilingAbove >= 9999990.0f && (player->mState & 0x200000) != 0)
            player->mActionState |= 0x8000; //patch upwarps
    }
    return 1;
}

/* NPC CARRY CODE */

u32 SME::Patch::Mario::carryOrTalkNPC(TBaseNPC *npc)
{
    const SME::Class::TPlayerParams *playerParams = SME::TGlobals::sGlobals.getPlayerParams(gpMarioAddress);

    if ((*(u32 *)(&npc->mStateFlags) & 0x840007) != 0)
        return 0;
    
    if (gpMarioAddress->mState == static_cast<u32>(TMario::State::IDLE))
        return 0;

    if (!playerParams->isMario() ||
        !playerParams->isInitialized())
        return 0;

    bool oldTake = npc->mStateFlags.mCanBeTaken;
    npc->mStateFlags.mCanBeTaken = playerParams->getParams()->mCanHoldNPCs.get();

    u32 ret = isNowCanTaken__8TBaseNPCCFv(npc);

    npc->mStateFlags.mCanBeTaken = oldTake;
    return ret;
}

bool SME::Patch::Mario::canGrabAtNPC()
{
    TBaseNPC *npc;
    SME_FROM_GPR(30, npc);

    const SME::Class::TPlayerParams *playerParams = SME::TGlobals::sGlobals.getPlayerParams(gpMarioAddress);
    
    if (npc->mStateFlags.mCanBeTaken)
        return true;
    
    if (gpMarioAddress->mState == static_cast<u32>(TMario::State::IDLE))
        return false;

    if (!playerParams->isMario() || !playerParams->isInitialized())
        return false;

    return playerParams->getParams()->mCanHoldNPCs.get();
}

bool SME::Patch::Mario::canCarryNPC()
{
    TBaseNPC *npc;
    SME_FROM_GPR(30, npc);

    const SME::Class::TPlayerParams *playerParams = SME::TGlobals::sGlobals.getPlayerParams(gpMarioAddress);
    
    if (npc->mStateFlags.mCanBeTaken)
        return true;
    
    if (gpMarioAddress->mState == static_cast<u32>(TMario::State::IDLE))
        return false;

    if (!playerParams->isMario() || !playerParams->isInitialized())
        return false;

    return playerParams->getParams()->mCanHoldNPCs.get();
}

#ifdef MARIO_MODS
static TMario *scaleNPCThrowLength(TMario *player, float *params)
{
    f32 _f11;
    __asm { fmr _f11, f11 };

    _f11 = params[0x1D0 / 4];
    if (playerParams->isInitialized() && playerParams->isMario())
    {
        _f11 *= playerParams->mParams->Attributes.mThrowPowerMultiplier * ((playerParams->mParams->Attributes.mSizeMultiplier * 0.5f) + (1.0f - 0.5f));
    }
    
    if (player->mState == static_cast<u32>(TMario::State::NPC_THROW) || player->mState == static_cast<u32>(TMario::State::NPC_JUMPTHROW))
    {
        _f11 *= 4.0f;
    }

    __asm volatile { fmr f11, _f11 };
    return player;
}
kmCall(0x802145F0, &scaleNPCThrowLength);
kmWrite32(0x802145F4, 0xC002E5E0);
kmWrite32(0x802145F8, 0xC0230034);
kmWrite32(0x8021462C, 0xEC0B0032);
kmWrite32(0x80214634, 0xEC2B0072);

static u32 scaleNPCThrowHeight(u32 _r3, f32 z, f32 y)
{
    TBaseNPC *npc;
    SME_FROM_GPR(31, npc);

    TMario *player = (TMario *)npc->mPrevHolder;
    if (playerParams->isInitialized() && playerParams->isMario())
        y *= playerParams->mParams->Attributes.mThrowPowerMultiplier *
             ((playerParams->mParams->Attributes.mSizeMultiplier * 0.5f) + (1.0f - 0.5f));
    
    if (player->mState == static_cast<u32>(TMario::State::NPC_THROW) ||
        player->mState == static_cast<u32>(TMario::State::NPC_JUMPTHROW))
        y *= 4.0f;
    
    npc->mSpeed.y = y;
    npc->mSpeed.z = z;

    return _r3;
}
kmCall(0x8021463C, &scaleNPCThrowHeight);


/* TREE CLIMB CODE */

static f32 getTreeClimbMinFall()
{
    TMario *player;
    SME_FROM_GPR(31, player);

    return 100.0f * player->mSize.y;
}
kmCall(0x80261C3C, &getTreeClimbMinFall);
kmWrite32(0x80261C40, 0xC05F038C);
kmWrite32(0x80261C44, 0xFC020040);

static void getTreeClimbMaxFall(TMapObjBase *tree, f32 speed)
{
    TMario *player;
    f32 ret;
    SME_FROM_GPR(31, player);
    SME_FROM_FPR(3, ret);
    
    ret = tree->mReceiveHeight / ((player->mSize.y*0.2f) + (1.0f - 0.2f));

    SME_TO_FPR(1, speed);
    SME_TO_FPR(3, ret);
}
kmCall(0x802619CC, &getTreeClimbMaxFall);
kmWrite32(0x802619D0, 0xC05F0014);

// 0x80261C3C
bool scaleTreeSlideSpeed(register f32 _f1, f32 _f2)
{
    TMario *player;
    SME_FROM_GPR(31, player);

    _f1 = 0.00195313f * player->mGravity;

    __asm volatile { fmr f1, _f1
                     fmr f2, _f2 };

    if (_f2 < -16.0f)
    {
        return true;
    }
    else
    {
        player->mSpeed.y = 0.0f;
        return false;
    }
}
kmCall(0x80261CF4, &scaleTreeSlideSpeed);
kmWrite32(0x80261CF8, 0x2C030000);
kmWrite32(0x80261CFC, 0x41820070);


/* GLOBAL CLIMB CODE */

static void getClimbingAnimSpd(TMario *player, TMario::Animation anim, f32 speed)
{
    if (playerParams->mIsClimbTired)
        speed = 6.0f;
    
    setAnimation__6TMarioFif(player, anim, speed);
}
kmCall(0x8025D588, &getClimbingAnimSpd);
kmCall(0x8025D63C, &getClimbingAnimSpd);
kmCall(0x8025D650, &getClimbingAnimSpd);
kmCall(0x8025DBC4, &getClimbingAnimSpd);
kmCall(0x8025E38C, &getClimbingAnimSpd);


/* ROOF HANG CODE */

static void scaleRoofClimbHeight(f32 yCoord, f32 speed)
{
    TMario *player;
    __asm { mr player, r31 };

    player->mPosition.y = yCoord - (160.0f * player->mSize.y);

    __asm volatile { fmr f2, speed };
}
kmCall(0x8025D66C, &scaleRoofClimbHeight);
kmWrite32(0x8025D670, 0x60000000);
kmWrite32(0x8025D674, 0x60000000);

static asm void scaleRoofSquashedHeight()
{
    nofralloc
    lfs	f3, -0xDE0 (rtoc)
    lfs f5, 0x28 (r30)
    fmuls f3, f5, f3
    blr
}
kmCall(0x802617EC, &scaleRoofSquashedHeight);

static void scaleRoofMoveDiff()
{
    TMario *player;
    f32 _f0;

    __asm { mr player, r30
            fmr _f0, f0 };

    _f0 = 30.0f * player->mSize.y;

    __asm volatile { fmr f0, _f0 };
}
kmCall(0x80261824, &scaleRoofMoveDiff);

void scaleHangSpeed(TMario *player)
{
    player->mForwardSpeed += 1.0f;

    if (playerParams->isInitialized() && playerParams->isMario())
        player->mForwardSpeed = Min(player->mForwardSpeed, 4.0f * playerParams->mParams->Attributes.mSpeedMultiplier);
    else
        player->mForwardSpeed = Min(player->mForwardSpeed, 4.0f);
}
kmCall(0x802615AC, &scaleHangSpeed);
kmWrite32(0x802615B0, 0x60000000);

static TBGCheckData *canHangOnRoof(TBGCheckData *roof)
{
    bool canCling;
    __asm { mr canCling, r4 };

    TMario *player;
    __asm { mr player, r30 };

    if (playerParams->isMario() && playerParams->isInitialized())
        canCling = (roof->mCollisionType == 266 || playerParams->mParams->Attributes.mCanClimbWalls);
    else
        canCling = roof->mCollisionType == 266;

    __asm volatile { mr r4, canCling };

    return roof;
}
kmCall(0x802617C0, &canHangOnRoof);
kmWrite32(0x802617C4, 0x2C040000);
kmWrite32(0x802617C8, 0x4182000C);


/* WALL CLIMB CODE */

//8025e560 <- possibly resize float by char size mul for height
//8025e5bc <- gate sound?

//80415DCC <- this float controls how far into the wall mario is placed on first grab
//80415DD4 <- this float controls how high mario is placed from the floor on first grab

//80415DEC <- this float controls the climbing speed, scale accordingly

static f32 scaleClimbSpeed(f32 speed)
{
    TMario *player;
    __asm { mr player, r30 };

    f32 _f0;
    f32 _f3;
    f32 _f7;

    __asm { fmr _f0, f0
            fmr _f3, f3
            fmr _f7, f7 };

    f32 scale = 0.015625f;

    if (playerParams->isInitialized() && playerParams->isMario())
        scale *= playerParams->mParams->Attributes.mSpeedMultiplier;

    __asm volatile { fmr f0, _f0
                     fmr f3, _f3
                     fmr f7, _f7 };

    return scale;
}
kmCall(0x8025E19C, &scaleClimbSpeed);
kmWrite32(0x8025E1A0, 0x807E0010);
kmWrite32(0x8025E1A4, 0x801E0014);
kmWrite32(0x8025E1C4, 0xC0440014);
kmCall(0x8025E218, &scaleClimbSpeed);
kmWrite32(0x8025E21C, 0x7FC3F378);

static TBGCheckData *checkClimbingWallPlane(TMario *player, JGeometry::TVec3<float> pos, f32 w, f32 h)
{
    return (TBGCheckData *)checkWallPlane__6TMarioFP3Vecff(player, pos, w * player->mSize.z, h * player->mSize.y);
}
kmCall(0x8025DD84, &checkClimbingWallPlane);
kmCall(0x8025DEB8, &checkClimbingWallPlane);
kmCall(0x8025E184, &checkClimbingWallPlane);
kmCall(0x8025E2D0, &checkClimbingWallPlane);
kmCall(0x8025E2E8, &checkClimbingWallPlane);

static TBGCheckData *canJumpClingWall(TBGCheckData *wall)
{
    bool canCling;
    __asm { mr canCling, r4 };

    TMario *player;
    __asm { mr player, r28 };

    if (playerParams->isMario() && playerParams->isInitialized())
        canCling =
            wall->mCollisionType == 266 ||
                (playerParams->mParams->Attributes.mCanClimbWalls &&
                 player->mController->mButtons.mInput & TMarioGamePad::Buttons::Z);
    else
        canCling = wall->mCollisionType == 266;

    __asm volatile { mr r4, canCling };

    return wall;
}
kmCall(0x8024C888, &canJumpClingWall);
kmWrite32(0x8024C88C, 0x2C040000);
kmWrite32(0x8024C890, 0x4182000C);

static TBGCheckData *canUnkActionWall(TBGCheckData *wall)
{
    bool canCling;
    __asm { mr canCling, r4 };

    TMario *player;
    __asm { mr player, r22 };

    if (playerParams->isMario() && playerParams->isInitialized())
        canCling =
            wall->mCollisionType == 266 ||
                (playerParams->mParams->Attributes.mCanClimbWalls &&
                 player->mController->mButtons.mInput & TMarioGamePad::Buttons::Z && 
                 wall->mCollisionType != 5);
    else
        canCling = wall->mCollisionType == 266;

    __asm volatile { mr r4, canCling };

    return wall;
}
kmCall(0x80256A3C, &canUnkActionWall);
kmWrite32(0x80256A40, 0x2C040000);
kmWrite32(0x80256A44, 0x4182000C);

static TBGCheckData *canRunClingWall(TBGCheckData *wall)
{
    bool canCling;
    __asm { mr canCling, r4 };

    TMario *player;
    __asm { mr player, r31 };

    if (playerParams->isMario() && playerParams->isInitialized())
        canCling =
            wall->mCollisionType == 266 ||
                (playerParams->mParams->Attributes.mCanClimbWalls &&
                 player->mController->mButtons.mInput & TMarioGamePad::Buttons::Z &&
                 wall->mCollisionType != 5);
    else
        canCling = wall->mCollisionType == 266;

    __asm volatile { mr r4, canCling };

    return wall;
}
kmCall(0x8025B200, &canRunClingWall);
kmWrite32(0x8025B204, 0x2C040000);
kmWrite32(0x8025B208, 0x4182000C);

static TBGCheckData *canMoveOnWall1()
{
    bool canCling;
    __asm { mr canCling, r4 };

    TMario *player;
    __asm { mr player, r30 };

    TBGCheckData *wall;
    __asm { mr wall, r29 };

    if (playerParams->isMario() && playerParams->isInitialized())
        canCling = (wall->mCollisionType == 266 || playerParams->mParams->Attributes.mCanClimbWalls);
    else
        canCling = wall->mCollisionType == 266;

    __asm volatile { mr r4, canCling };

    return wall;
}
kmCall(0x8025E2F4, &canMoveOnWall1);
kmWrite32(0x8025E2F8, 0x2C040000);
kmWrite32(0x8025E2FC, 0x4182000C);

static TBGCheckData *canMoveOnWall2(TBGCheckData *wall)
{
    bool canCling;
    __asm { mr canCling, r4 };

    TMario *player;
    __asm { mr player, r30 };

    if (playerParams->isMario() && playerParams->isInitialized())
        canCling = (wall->mCollisionType == 266 || playerParams->mParams->Attributes.mCanClimbWalls);
    else
        canCling = wall->mCollisionType == 266;

    __asm volatile { mr r4, canCling };

    return wall;
}
kmCall(0x8025E31C, &canMoveOnWall2);
kmWrite32(0x8025E320, 0x2C040000);
kmWrite32(0x8025E324, 0x4182000C);

static TBGCheckData *canClimbUnderwater(TBGCheckData *wall)
{
    bool canCling;
    __asm { mr canCling, r4 };

    TMario *player;
    __asm { mr player, r31 };

    if (playerParams->isMario() && playerParams->isInitialized())
        canCling =
            wall->mCollisionType == 266 ||
                (playerParams->mParams->Attributes.mCanClimbWalls &&
                 player->mController->mButtons.mInput & TMarioGamePad::Buttons::Z &&
                 wall->mCollisionType != 5);
    else
        canCling = wall->mCollisionType == 266;

    __asm volatile { mr r4, canCling };

    return wall;
}
kmCall(0x80272660, &canClimbUnderwater);
kmWrite32(0x80272664, 0x2C040000);
kmWrite32(0x80272668, 0x4182000C);


/* GOOP WALKING CODE */

static void checkGraffitiAffected(TMario *player)
{
    if (!playerParams->isMario() || !playerParams->isInitialized())
    {
        checkGraffito__6TMarioFv(player);
    }
    else if (playerParams->mParams->Attributes.mGoopAffected)
    {
        checkGraffito__6TMarioFv(player);
    }
}
kmCall(0x8024E288, &checkGraffitiAffected);


//0x801E4118
static void rescaleHeldObj(Mtx holderMatrix, Mtx destMatrix)
{
    TMapObjBase *mHeldObj;
    __asm { mr mHeldObj, r31 };

    PSMTXCopy(holderMatrix, destMatrix);
    PSMTXScaleApply(destMatrix, destMatrix, 1 / mHeldObj->mHolder->mSize.x, 1 / mHeldObj->mHolder->mSize.y, 1 / mHeldObj->mHolder->mSize.z);
}
//kmCall(0x801E4118, &rescaleHeldObj);

//0x80213314
static asm void scaleNPCTalkRadius()
{
    nofralloc
    lis	r3, 0x8041
    lwz	r3, -0x1EF8 (r3)
    lfs	f0, 0x002C (r3)
    fmuls f30, f30, f0
    lwz r3, -0x6220 (r13)
    blr
}
kmCall(0x80213314, &scaleNPCTalkRadius);

static u32 patchYStorage()
{
    TMario *player;
    __asm { mr player, r31 };

    if (player->mState != static_cast<u32>(TMario::State::IDLE))
        player->mSpeed.y = 0.0f;

    return 0;
}
kmCall(0x802571F0, &patchYStorage);

//0x8024E02C
static void manageExtraJumps(TMario *player)
{
    if (!playerParams->isMario() || !playerParams->isInitialized())
    {
        stateMachine__6TMarioFv(player);
        return;
    }

    if ((player->mState & static_cast<u32>(TMario::State::AIRBORN)) == false ||
        (player->mState & 0x800000) ||
        player->mYoshi->mState == TYoshi::MOUNTED)
        playerParams->mCurJump = 1;
    else
    {
        if (player->mController->mButtons.mFrameInput & TMarioGamePad::Buttons::A &&
            playerParams->mCurJump < playerParams->mMaxJumps &&
            player->mState != static_cast<u32>(TMario::State::WALLSLIDE))
        {
            if ((playerParams->mMaxJumps - playerParams->mCurJump) == 1)
            {
                if (player->mState != static_cast<u32>(TMario::State::TRIPLE_J))
                    changePlayerJumping__6TMarioFUlUl(player, static_cast<u32>(TMario::State::TRIPLE_J), 0);
                else
                    setStatusToJumping__6TMarioFUlUl(player, static_cast<u32>(TMario::State::TRIPLE_J), 0);
            }
            else if ((player->mState - static_cast<u32>(TMario::State::JUMP)) > 1)
                changePlayerJumping__6TMarioFUlUl(player, static_cast<u32>(TMario::State::JUMP), 0);
            else
                changePlayerJumping__6TMarioFUlUl(player, player->mState ^ 1, 0);
            playerParams->mCurJump += 1;
        }
    }
    stateMachine__6TMarioFv(player);
}
kmCall(0x8024E02C, &manageExtraJumps);

static f32 calcJumpPower(TMario *player, f32 factor, f32 curYVelocity, f32 jumpPower)
{
    if (playerParams->mParams)
    {
        jumpPower *= playerParams->mParams->Attributes.mBaseJumpHeightMulti;
        if (player->mState & static_cast<u32>(TMario::State::AIRBORN))
        {
            jumpPower *= powf(playerParams->mParams->Attributes.mMultiJumpMultiplier, (f32)playerParams->mCurJump);
            player->mForwardSpeed *= playerParams->mParams->Attributes.mMultiJumpFSpeedMulti;
        }
    }
    return (curYVelocity * factor) + jumpPower;
}

static asm void jumpPowerWrapper()
{
    mflr r0
    stw r0, 0x4 (sp)
    stwu sp, -0x10 (sp)
    stw r31, 0x8 (sp)
    mr r31, r4
    lwz r3, -0x60B8 (r13)
    cmpw r30, r3
    fmadds f0, f2, f1, f0
    bne notMario
    mr r3, r30
    fmr f3, f0
    bl calcJumpPower
    fmr f0, f1
    notMario:
    mr r3, r30
    mr r4, r31
    lwz r31, 0x8 (sp)
    addi sp, sp, 0x10
    lwz r0, 0x4 (sp)
    mtlr r0
    blr
}
kmCall(0x80254540, &jumpPowerWrapper);
kmCall(0x802546B0, &jumpPowerWrapper);
kmCall(0x802546E4, &jumpPowerWrapper);
kmCall(0x8025474C, &jumpPowerWrapper);
kmCall(0x80254884, &jumpPowerWrapper);

static f32 checkGroundSpeedLimit()
{
    TMario *player;
    __asm { mr player, r31 };

    float multiplier = 1;
    if (onYoshi__6TMarioCFv(player))
    {
        multiplier *= player->mFSpeedYoshiMul;
    }
    else if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
    {
        multiplier *= playerParams->mParams->Attributes.mSpeedMultiplier;
    }
    return multiplier;
}
kmWrite32(0x8025B8BC, 0x60000000);
kmCall(0x8025B8C0, &checkGroundSpeedLimit);
kmWrite32(0x8025B8C4, 0xEFFF0072);

static f32 checkGroundSpeedMulti()
{
    TMario *player;
    __asm { mr player, r31 };

    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
        return player->mRunSlowdownFactor2 / playerParams->mParams->Attributes.mSpeedMultiplier;
    else
        return player->mRunSlowdownFactor2;
}
kmCall(0x8025B8F8, &checkGroundSpeedMulti);

static void checkJumpSpeedLimit(f32 speed)
{
    TMario *player;
    __asm { mr player, r31 };

    f32 speedCap = 32;
    f32 speedReducer = 0.2;

    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
    {
        speedCap *= playerParams->mParams->Attributes.mSpeedMultiplier;
        speedReducer *= playerParams->mParams->Attributes.mSpeedMultiplier;
    }
    
    if (speed > speedCap)
    {
        player->mForwardSpeed = (speed - speedReducer);
    }
}
kmWrite32(0x8024CC60, 0x60000000);
kmWrite32(0x8024CC64, 0x60000000);
kmWrite32(0x8024CC68, 0x60000000);
kmCall(0x8024CC6C, &checkJumpSpeedLimit);

static TMario *checkJumpSpeedMulti(TMario *player, f32 factor, f32 max)
{
    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
    {
        player->mForwardSpeed = ((factor * playerParams->mParams->Attributes.mSpeedMultiplier) * max) + player->mForwardSpeed;
        return player;
    }
    else
    {
        player->mForwardSpeed = (factor * max) + player->mForwardSpeed;
        return player;
    }
}
kmCall(0x8024CC2C, &checkJumpSpeedMulti);
kmWrite32(0x8024CC30, 0x60000000);

//0x80272FF0 - fSpeed Multplier - swim
static void checkSwimSpeedMulti(f32 max, f32 factor)
{
    TMario *player;
    __asm { mr player, r31 };

    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
    {
        player->mForwardSpeed = ((factor * playerParams->mParams->Attributes.mSpeedMultiplier) * max) + player->mForwardSpeed;
    }
    else
    {
        player->mForwardSpeed = (factor * max) + player->mForwardSpeed;
    }
}
kmCall(0x80272FF0, &checkSwimSpeedMulti);
kmWrite32(0x80272FF4, 0x60000000);

static f64 checkSlideSpeedMulti(f32 max, f32 factor)
{
    TMario *player;
    __asm { mr player, r30 };

    f64 slowFactor = 0.5;

    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
    {
        slowFactor /= playerParams->mParams->Attributes.mSpeedMultiplier;
    }
    
    if (player->mFludd && isEmitting__9TWaterGunFv(player->mFludd))
    {
        if (player->mFludd->mCurrentNozzle == TWaterGun::Hover)
            slowFactor /= 1.3;
        else if (player->mFludd->mCurrentNozzle == TWaterGun::Rocket)
        {
            slowFactor /= 2;
            addVelocity__6TMarioFf(player, 20.0f);
        }
    }
    return slowFactor;
}
kmCall(0x8025C3E0, &checkSlideSpeedMulti);
kmWrite32(0x8025C3E8, 0xFC4100F2);
kmWrite32(0x8025C3EC, 0xFC2300F2);

static f32 checkGroundSpeedCap()
{
    TMario *player;
    __asm { mr player, r31 };

    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
        return player->mMaxGroundSpeed * playerParams->mParams->Attributes.mSpeedMultiplier;
    else
        return player->mMaxGroundSpeed;
}
kmCall(0x8025BC44, &checkGroundSpeedCap);

static void checkHoverSpeedMulti(f32 factor, f32 max)
{
    TMario *player;
    __asm { mr player, r30 };

    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
    {
        player->mForwardSpeed = ((factor * playerParams->mParams->Attributes.mSpeedMultiplier) * max) + player->mForwardSpeed;
    }
    else
    {
        player->mForwardSpeed = (factor * max) + player->mForwardSpeed;
    }
}
kmCall(0x8024AE80, &checkHoverSpeedMulti);
kmWrite32(0x8024AE84, 0x60000000);

static void mario_addVelocity(TMario *player, f32 add)
{
    player->mForwardSpeed += add;
    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
    {
        if (player->mForwardSpeed > (99.0f * playerParams->mParams->Attributes.mSpeedMultiplier))
            player->mForwardSpeed = (99.0f * playerParams->mParams->Attributes.mSpeedMultiplier);
    }
    else
    {
        if (player->mForwardSpeed > 99.0f)
            player->mForwardSpeed = 99.0f;
    }
}
kmBranch(0x802558A4, &mario_addVelocity);

static TMario *checkOilSlipSpeedMulti(f32 factor, f32 max)
{
    TMario *player;
    __asm { mr player, r31 };

    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
    {
        player->mForwardSpeed = ((factor * playerParams->mParams->Attributes.mSpeedMultiplier) * max) + player->mForwardSpeed;
    }
    else
    {
        player->mForwardSpeed = (factor * max) + player->mForwardSpeed;
    }
    return player;
}
kmCall(0x80259368, &checkOilSlipSpeedMulti);
kmWrite32(0x8025936C, 0x60000000);

static f32 checkJumpingWallBonk(f32 f1, f32 speed)
{
    TMario *player;
    __asm { mr player, r28 };

    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
        return player->mWallBonkThreshold * playerParams->mParams->Attributes.mSpeedMultiplier;
    else
        return player->mWallBonkThreshold;
}
kmWrite32(0x8024C7C0, 0xC05C00B0);
kmCall(0x8024C7C4, &checkJumpingWallBonk);
kmWrite32(0x8024C7C8, 0xFC020840);

static f32 checkRunningWallBonk(f32 f1, f32 speed)
{
    TMario *player;
    __asm { mr player, r31 };

    if (playerParams->isInitialized() && playerParams->isMario() && !onYoshi__6TMarioCFv(player))
        return player->mWallBonkThreshold * playerParams->mParams->Attributes.mSpeedMultiplier;
    else
        return player->mWallBonkThreshold;
}
kmWrite32(0x8025B148, 0xC05F00B0);
kmCall(0x8025B14C, &checkRunningWallBonk);
kmWrite32(0x8025B150, 0xFC020840);

static void checkYSpdForTerminalVelocity()
{
    TMario *player;
    SME_FROM_GPR(31, player);

    float terminalVelocity; = -75.0f * player->mJumpParams.mGravity.get();
    if (SME::TGlobals::sGlobals.getPlayerParams(player)->mCollisionFlags.mIsGlideBounce)
        terminalVelocity = -20.0f * player->mJumpParams.mGravity.get();
    else
        terminalVelocity = -75.0f * player->mJumpParams.mGravity.get();
    player->mSpeed.y = Max(player->mSpeed.y, terminalVelocity);
}
kmCall(0x80256678, &checkYSpdForTerminalVelocity);
kmWrite32(0x8025667C, 0x60000000);

static void scaleDiveSpeed()
{
    TMario *player;
    __asm { mr player, r30 };

    f32 _f0;
    __asm { fmr _f0, f0 };

    if (playerParams->isMario() && playerParams->isInitialized())
        _f0 = 48.0f * playerParams->mParams->Attributes.mSpeedMultiplier;
    else
        _f0 = 48.0f;

    __asm volatile { fmr f0, _f0 };
}
kmCall(0x80254904, &scaleDiveSpeed);
kmWrite32(0x80254908, 0xC042F0DC);
kmWrite32(0x8025490C, 0xC03E00B0);

//8024afe0 <- hover air Y spd

static asm void scaleHoverInitYSpd()
{
    nofralloc
    lfs	f0, -0x0FE0 (rtoc)
    lfs f4, 0x28 (r30)
    fmuls f0, f0, f4
    blr
}
kmCall(0x80254A2C, &scaleHoverInitYSpd);

static f32 setBounceYSpeed()
{
    TMario *player;
    __asm { mr player, r30 };

    return 130.0f * player->mSize.y;
}
kmCall(0x80254720, &setBounceYSpeed);
kmWrite32(0x80254724, 0xD01E00A8);

static asm void checkGrabHeight()
{
    nofralloc
    lfs f0, -0xEBC (rtoc)
    lfs f4, 0x28 (r29)
    fcmpo cr0, f4, f0
    lfs	f0, -0x0ED4 (rtoc)
    bgt skipmul

    fmuls f0, f0, f4

    skipmul:
    blr
}
kmCall(0x80256D34, &checkGrabHeight);

static asm void setCollisionHeight1()
{
    nofralloc
    lfs f1, -0xEDC (rtoc)
    lfs f0, 0x28 (r22)
    fmuls f1, f0, f1
    blr
}
kmCall(0x8025696C, &setCollisionHeight1);

static asm void setCollisionHeight2()
{
    nofralloc
    lfs f2, -0xEDC (rtoc)
    lfs f0, 0x28 (r29)
    fmuls f2, f0, f2
    blr
}
kmCall(0x80256D14, &setCollisionHeight2);

static asm void setCollisionHeight3()
{
    nofralloc
    lfs f0, -0xEDC (rtoc)
    lfs f2, 0x28 (r30)
    fmuls f0, f2, f0
    blr
}
kmCall(0x802573FC, &setCollisionHeight3);

static void setCollisionWidth()
{
    TMario *player;
    __asm { mr player, r29 };

    f32 width;
    __asm { fmr width, f0 };

    if (playerParams->isMario())
    {
        player->mCollisionXZSize = playerParams->DefaultAttrs.mCollisionWidth * player->mSize.x;
    }
    else
    {
        player->mCollisionXZSize = width;
    }
}
kmCall(0x802505F4, &setCollisionWidth);

static f32 manageGrabLength()
{
    TMario *player;
    __asm { mr player, r29 };

    return 60 * player->mSize.z;
}
kmCall(0x80256CE8, &manageGrabLength);
kmWrite32(0x80256CFC, 0xEC01283C);
kmWrite32(0x80256D04, 0xC05E003C);
kmWrite32(0x80256D0C, 0xEC0100BC);

static JUtility::TColor getEMarioHealthBarRGBA(TEnemyMario &eMario)
{
    JUtility::TColor color;
    s16 maxHealth = ((s16 *)eMario.mEnemyManager)[0x40 / 2];

    color.set(0xFF, 0x00, 0x00, 0xFF);
    color.G = lerp<u8>(0, 255, (float)eMario.mEnemyHealth / (float)maxHealth);
    color.R -= color.G;
    
    return color;
}

static void manageEMarioHealthWrapper(s32 chan, JUtility::TColor *color)
{
    TEnemyMario *eMario;
    __asm { mr eMario, r30 };

    GXSetChanMatColor(chan, getEMarioHealthBarRGBA(*eMario));
}

kmCall(0x8003FFEC, &manageEMarioHealthWrapper);
kmWrite32(0x8003FD94, 0x60000000);
#endif