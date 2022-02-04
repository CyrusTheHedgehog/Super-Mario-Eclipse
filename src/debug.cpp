#include "MTX.h"
#include "types.h"

#include "sme/obj/WaterBalloon.hxx"
#include "sms/actor/Mario.hxx"
#include "sms/enemy/EnemyMario.hxx"
#include "sms/npc/BaseNPC.hxx"

#include "sme/libs/sContainer.hxx"

#include "SME.hxx"

using namespace SME;

bool gInXYZMode = false;

// extern c_kit.cpp
// 0x8024D194
void Patch::Debug::xyzModifierMario(TMario *player) {
  constexpr f32 baseSpeed = 83.0f;
  constexpr u32 buttons = TMarioGamePad::Buttons::DPAD_UP;

  // if (!TGlobals::isDebugMode())
  //   return;

  const JUTGamePad::CStick &mainStick = player->mController->mControlStick;
  const f32 speedMultiplier =
      Util::Math::lerp<f32>(1, 2, player->mController->mButtons.mAnalogR);

  if (player->mController->mButtons.mFrameInput & buttons && !gInXYZMode) {
    setPlayerVelocity__6TMarioFf(player, 0.0f);
    player->mSpeed.y = 0.0f;

    gInXYZMode = true;
  } else if (player->mController->mButtons.mFrameInput & buttons &&
             gInXYZMode) {
    player->mState = static_cast<u32>(TMario::State::IDLE);
    gInXYZMode = false;
  }

  if (gInXYZMode) {
    const f32 cameraRotY = (f32)(gpCamera->mHorizontalAngle) / 182.0f;

    Vec playerPos;
    player->JSGGetTranslation(&playerPos);

    player->mState = static_cast<u32>(TMario::State::IDLE) |
                     static_cast<u32>(TMario::State::CUTSCENE);

    playerPos.x +=
        ((-sinf(Util::Math::angleToRadians(cameraRotY)) * baseSpeed) *
         speedMultiplier) *
        mainStick.mStickY;
    playerPos.z +=
        ((-cosf(Util::Math::angleToRadians(cameraRotY)) * baseSpeed) *
         speedMultiplier) *
        mainStick.mStickY;
    playerPos.x -=
        ((-sinf(Util::Math::angleToRadians(cameraRotY + 90.0f)) *
          baseSpeed) *
         speedMultiplier) *
        mainStick.mStickX;
    playerPos.z -=
        ((-cosf(Util::Math::angleToRadians(cameraRotY + 90.0f)) *
          baseSpeed) *
         speedMultiplier) *
        mainStick.mStickX;

    if (player->mController->mButtons.mInput & TMarioGamePad::Buttons::B) {
      playerPos.y -= (baseSpeed * speedMultiplier);
    } else if (player->mController->mButtons.mInput &
               TMarioGamePad::Buttons::A) {
      playerPos.y += (baseSpeed * speedMultiplier);
    }

    player->JSGSetTranslation(playerPos);
  }
  return;
}

static u8 sHomeID = 0;
static u8 sTargetID = 254;
static TBGCheckData *sHomeTriangle;

void Patch::Debug::updateDebugCollision(TMario *player) {
  constexpr u32 SetHomeTriangleButtons =
      TMarioGamePad::Buttons::Z | TMarioGamePad::Buttons::DPAD_LEFT;
  constexpr u32 SetTargetTriangleButtons =
      TMarioGamePad::Buttons::Z | TMarioGamePad::Buttons::DPAD_RIGHT;

#ifndef SME_DEBUG
  if (!Class::TCheatHandler::sDebugHandler.isActive())
    return;
#endif

  const JUTGamePad::CButton &buttons = player->mController->mButtons;

  if (buttons.mFrameInput == SetHomeTriangleButtons) {
    sHomeTriangle = player->mFloorTriangle;
    sHomeID = (sHomeID + 1) % 255;
  } else if (buttons.mFrameInput == SetTargetTriangleButtons && sHomeTriangle) {
    if (sHomeTriangle == player->mFloorTriangle)
      return;

    sHomeTriangle->mCollisionType = 16042;
    sHomeTriangle->mValue4 = s16((sTargetID << 8) | (sHomeID - 1));
    player->mFloorTriangle->mCollisionType = 16042;
    player->mFloorTriangle->mValue4 = s16(((sHomeID - 1) << 8) | sTargetID);
    TGlobals::sWarpColArray->addLink(sHomeTriangle, player->mFloorTriangle);
    TGlobals::sWarpColArray->addLink(player->mFloorTriangle, sHomeTriangle);
    sTargetID = sTargetID != 0 ? (sTargetID - 1) : 254;
  }

  return;
}

static TRingBuffer<16, TWaterBalloon> sBalloonBuffer;
TWaterBalloon *sWaterBalloon;

void createWaterBalloonAndThrow(TMario *player) {
  if (player->mHeldObject)
    return;

  #if 1
  if (!sWaterBalloon) {
    sWaterBalloon = new TWaterBalloon("waterballoon");
    sWaterBalloon->initAndRegister("waterballoon");
  } else {
    sWaterBalloon->kill();
  }

  sWaterBalloon->mPosition.set(player->mPosition);
  sWaterBalloon->appear();

  bool received = sWaterBalloon->receiveMessage(player, 4); // 4 = grab
  if (received) {
    player->mHeldObject = sWaterBalloon;
    player->_380 = 2;
  } else {
    player->mHeldObject = nullptr;
  }
  

  #if 0
  if (player->mState == static_cast<u32>(TMario::State::IDLE)) {
    changePlayerStatus__6TMarioFUlUlb(player, 0x80000387, 0, 0);
  } else if (player->mState == static_cast<u32>(TMario::State::RUNNING)) {
    changePlayerStatus__6TMarioFUlUlb(player, 0x80000588, 0, 0);
  } else if (player->mState == static_cast<u32>(TMario::State::JUMP) ||
             player->mState == static_cast<u32>(TMario::State::D_JUMP) ||
             player->mState == static_cast<u32>(TMario::State::FALL)) {
    changePlayerStatus__6TMarioFUlUlb(player, 0x820008AB, 0, 0);
  }
  #endif
  #endif
}

static void setEmitPrm() {
  TWaterBalloon::sEmitInfo = new TWaterEmitInfo("/mario/waterballoon/waterballoon.prm");
  TParams::finalize();
}
SME_PATCH_BL(SME_PORT_REGION(0x802B8DC8, 0x802b0d98, 0, 0), setEmitPrm);
