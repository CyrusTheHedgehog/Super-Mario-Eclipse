#include "SME.hxx"
#include "OS.h"

#include "kuribo_sdk.h"
#include "sms/actor/Mario.hxx"
#include "macros.h"

// mario.cpp
extern "C" s16 mario_shadowCrashPatch();

// shine.cpp
extern "C" void shine_animationFreezeCheck();
extern "C" void shine_thinkCloseCamera();

extern OSAlarm gctAlarm;

#ifdef SME_DEBUG
extern OSStopwatch gctStopwatch;
#endif

using namespace SME;

KURIBO_MODULE_BEGIN("Eclipse", "JoshuaMK",
                    __VERSION__ "[" SME_STRINGIZE(SME_MAX_SHINES) " Shines]") {
  KURIBO_EXECUTE_ON_LOAD {
    SME_DEBUG_LOG(
        "Codeblocker - Creating OSAlarm at %p; Calls %p every %0.4f seconds\n",
        &gctAlarm, &SME::Util::Security::checkUserCodes, 0.001f);
#ifdef SME_DEBUG
    OSInitStopwatch(&gctStopwatch, "Codeblocker");
#endif
    OSCreateAlarm(&gctAlarm);
    OSSetPeriodicAlarm(&gctAlarm, OSGetTime(), OSMillisecondsToTicks(1),
                       reinterpret_cast<OSAlarmHandler>(&SME::Util::Security::checkUserCodes));
    SME_DEBUG_LOG("Mario health offset = %X\n",
                  offsetof(TMario, mHealth));
  }
  KURIBO_EXECUTE_ON_UNLOAD {
    SME_DEBUG_LOG("-- Destroying Module --\n");
#ifdef SME_DEBUG
    OSStopStopwatch(&gctStopwatch);
#endif
    OSCancelAlarm(&gctAlarm);
  }

  /* -- HOOKS -- */

 
  // c_kit.cpp
  KURIBO_PATCH_BL(SME_PORT_REGION(0x802998B8, 0x80291750, 0, 0), Patch::CKit::onSetup);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x802A616C, 0x8029e07c, 0, 0), Patch::CKit::onUpdate);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80143F14, 0x80138b50, 0, 0), Patch::CKit::onDraw2D);
  KURIBO_PATCH_B(SME_PORT_REGION(0x802A8B58, 0x802a0c00, 0, 0), Util::SMS::isExMap);
  KURIBO_PATCH_B(SME_PORT_REGION(0x802A8B30, 0x802a0bd8, 0, 0), Util::SMS::isMultiplayerMap);
  KURIBO_PATCH_B(SME_PORT_REGION(0x802A8AFC, 0x802a0ba4, 0, 0), Util::SMS::isDivingMap);
  KURIBO_PATCH_B(SME_PORT_REGION(0x802A8AE0, 0x802a0b88, 0, 0), Util::SMS::isOptionMap);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8027C6A4, 0x80274430, 0, 0), Patch::CKit::manageLightSize);
  kWrite32(SME_PORT_REGION(0x8027C6A8, 0x80274434, 0, 0), 0x28030001);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80153DE8, 0x80148d68, 0, 0), Patch::CKit::formatTalkMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80153E1C, 0x80148d9c, 0, 0), Patch::CKit::formatTalkMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8024D3A8, 0x80245134, 0, 0), Patch::CKit::realTimeCustomAttrsHandler);

  // Odd behaviour
  // camera.cpp
  // KURIBO_PATCH_B(SME_PORT_REGION(0x80027548, 0x80027600, 0, 0), Patch::Camera::modifyCameraRangeToSize);

  // CheatCode.cpp
  KURIBO_PATCH_B(SME_PORT_REGION(0x80295B6C, 0x8028da04, 0, 0), Patch::Cheat::handleDebugCheat);

  // collision.cpp
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x80258334, 0x802500c0, 0, 0), Patch::Collision::checkIsGlideBounce);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x80264CFC, 0x8025ca88, 0, 0), Patch::Collision::checkIsGlideBounce);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8024C558, 0x802442e4, 0, 0), Patch::Collision::checkIsRestoreTypeNoFallDamage);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x802500B8, 0x80247e44, 0, 0), Patch::Collision::updateCollisionContext);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x80250CA0, 0x80248a2c, 0, 0), Patch::Collision::masterGroundCollisionHandler);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8025059C, 0x80248328, 0, 0), Patch::Collision::masterAllCollisionHandler);
// 
  // // file_flags.cpp
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x802B1794, 0x802a96a4, 0, 0), Patch::Flag::setFileCompleteBool);

  // // flagmanager.cpp
  // KURIBO_PATCH_B(SME_PORT_REGION(0x80294EF4, 0x8028cd0c, 0, 0), Patch::Flag::resetStage);

  // fludd.cpp
  // KURIBO_PATCH_B(SME_PORT_REGION(0x80248F14, 0x80240ca0, 0, 0), Patch::Fludd::isPumpOK);
  // kWrite32(SME_PORT_REGION(0x803DCA02, 0x803d41e2, 0, 0), 0x00300000 | TMarioAnimeData::FLUDD::FLUDD_ENABLED);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8014206C, 0x80136c80, 0, 0), Patch::Fludd::hasWaterCardOpen);
  // kWrite32(SME_PORT_REGION(0x80142070, 0x80136c84, 0, 0), 0x28030000);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x80283058, 0x8027ade4, 0, 0), Patch::Fludd::canCollectFluddItem);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x800678C4, 0x80060f64, 0, 0), Patch::Fludd::sprayGoopMapWrapGlobalMar);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x801A3ED0, 0x8019c758, 0, 0), Patch::Fludd::sprayGoopMapWrapGlobalMar);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x801B42D8, 0x801ac190, 0, 0), Patch::Fludd::sprayGoopMapWrapGlobalMar);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8024E710, 0x8024649c, 0, 0), Patch::Fludd::sprayGoopMapWrapMar30);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8027F7DC, 0x80277568, 0, 0), Patch::Fludd::sprayGoopMapWrapGlobalMar);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8027F94C, 0x802776d8, 0, 0), Patch::Fludd::sprayGoopMapWrapGlobalMar);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x800FED3C, 0x800f83dc, 0, 0), Patch::Fludd::canCleanSeals);
  // kWrite32(SME_PORT_REGION(0x800FED40, 0x800f83e0, 0, 0), 0x2C030000);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8024D53C, 0x802452c8, 0, 0), Patch::Fludd::bindFluddtojoint);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8024E548, 0x802462d4, 0, 0), Patch::Fludd::checkExecWaterGun);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8026C370, 0x802640fc, 0, 0), Patch::Fludd::killTriggerNozzle);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8026C018, 0x80263da4, 0, 0), Patch::Fludd::spamHoverWrapper);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x80262580, 0x8025a30c, 0, 0), Patch::Fludd::checkAirNozzle);
  // kWrite32(SME_PORT_REGION(0x80262584, 0x8025a310, 0, 0), 0x2C030000);

  // fruit.cpp
  KURIBO_PATCH_BL(SME_PORT_REGION(0x801E542C, 0x801dd304, 0, 0), Patch::Fruit::canFruitDieWater);
  kWrite32(SME_PORT_REGION(0x801E5430, 0x801dd308, 0, 0), 0x2C030000);
  kWrite32(SME_PORT_REGION(0x801E5434, 0x801dd30c, 0, 0), 0x41820140);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8023F964, 0x802376f0, 0, 0), Patch::Fruit::chooseGrabDistancing);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8023F9DC, 0x80237768, 0, 0), Patch::Fruit::isGrabWaitOver);

  // init_mods.cpp
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80005328, 0x80005328, 0, 0), Patch::Init::initCodeProtection);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x802A750C, 0x8029f588, 0, 0), Patch::Init::createGlobalHeaps);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x802A7140, 0x8029f02c, 0, 0), Patch::Init::setupMarioDatas);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x802A716C, 0x8029f058, 0, 0), Patch::Init::initFirstModel);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x802998B4, 0x8029174c, 0, 0), Patch::Init::initFileMods);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80280180, 0x80277f0c, 0, 0), Patch::Init::initShineShadow);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x802B7A4C, 0x802afa1c, 0, 0), Patch::Init::initSoundBank);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x802983F0, 0x80290288, 0, 0), Patch::Init::initMusicTrack);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80298420, 0x802902b8, 0, 0), Patch::Init::initMusicTrack);
  kWrite32(SME_PORT_REGION(0x80276C90, 0x8026ea1c, 0, 0), 0x60000000);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80276C94, 0x8026ea20, 0, 0), Patch::Init::fromMarioInit);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x800397DC, 0x80039894, 0, 0), Patch::Init::fromShadowMarioInit);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80271580, 0x8026930c, 0, 0), Patch::Init::initYoshi);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8029CCB0, 0x80294b8c, 0, 0), Patch::Init::initCardColors);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x802B8B20, 0x802b0af0, 0, 0), Patch::Init::initCollisionWarpLinks);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x802B57E4, 0x802ad768, 0, 0), Patch::Init::createUIHeap);

//   // mario.cpp
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x802320E0, 0x8022a034, 0, 0), mario_shadowCrashPatch);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x802500B8, 0x80247e44, 0, 0), Patch::Mario::updateContexts);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8029A87C, 0x80292758, 0, 0), Patch::Mario::carryOrTalkNPC);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x802815F0, 0x8027937c, 0, 0), Patch::Mario::canGrabAtNPC);
  // kWrite32(SME_PORT_REGION(0x802815F4, 80279380, 0, 0), 0x2C030000);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x80207430, 0x801ff314, 0, 0), Patch::Mario::canCarryNPC);
  // kWrite32(SME_PORT_REGION(0x80207434, 0x801ff318, 0, 0), 0x2C030000);

//   // multiplayer.cpp
//   KURIBO_PATCH_B(SME_PORT_REGION(0x802EFAB4, 0, 0, 0), Patch::Multiplayer::draw3DOverhaul);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x8029D7E8, 0, 0, 0), Patch::Multiplayer::makeMarios);
//   KURIBO_PATCH_B(SME_PORT_REGION(0x80276BD0, 0, 0, 0), Patch::Multiplayer::loadMarioTrickyOverhaul);
//   KURIBO_PATCH_B(SME_PORT_REGION(0x8024D2A8, 0, 0, 0), Patch::Multiplayer::performMarioTrickyOverhaul);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x802983F8, 0, 0, 0), Patch::Multiplayer::setMarioOverhaul);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x80298428, 0, 0, 0), Patch::Multiplayer::setMarioOverhaul);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x802984D8, 0, 0, 0), Patch::Multiplayer::setMarioOverhaul);

//   // shine.cpp
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x801BD664, 0x801b551c, 0, 0), Patch::Shine::manageShineVanish);
//   kWrite32(SME_PORT_REGION(0x801BD668, 0x801b5520, 0, 0), 0x48000568);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x802413E0, 0x8023916c, 0, 0), Patch::Shine::isKillEnemiesShine);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x802995BC, 0x80291454, 0, 0), Patch::Shine::checkBootOut);
//   kWrite32(SME_PORT_REGION(0x80297BE8, 0x8028fa80, 0, 0), 0x60848200);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x80293B10, 0x8028b928, 0, 0), Patch::Shine::extendShineIDLogic);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x801BCC98, 0x801b4b50, 0, 0), Patch::Shine::shineObjectStringMod);
//   kWrite32(SME_PORT_REGION(0x803DEE50, 0x803d6630, 0, 0), Patch::Shine::shineFlagSetter);
//   kWrite32(SME_PORT_REGION(0x803DEE7C, 0x803d665c, 0, 0), Patch::Shine::shineFlagGetter);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x802946D4, 0x8028c4ec, 0, 0), Patch::Shine::shineSetClamper);
//   kWrite32(SME_PORT_REGION(0x80294744, 0x8028c55c, 0, 0), 0x60000000);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x8029474C, 0x8028c564, 0, 0), Patch::Shine::shineGetClamper);
//   /*Shine casts, fix light*/
//   kWrite32(SME_PORT_REGION(0x80412548, 0x80409a90, 0, 0), (f32)SME_MAX_SHINES);
//   kWrite32(SME_PORT_REGION(0x80293AF8, 0x8028b910, 0, 0), 0x3BFF03E7);
//   kWrite32(SME_PORT_REGION(0x802946B8, 0x8028c4d0, 0, 0), 0x280003E7);
//   kWrite32(SME_PORT_REGION(0x8017BE78, 0x80171ea8, 0, 0), 0x5464037E);
//   kWrite32(SME_PORT_REGION(0x8017BEF4, 0x80171f24, 0, 0), 0x5464037E);
//   kWrite32(SME_PORT_REGION(0x8017BF34, 0x80171f6c, 0, 0), 0x5464037E);
//   kWrite32(SME_PORT_REGION(0x801BCE30, 0x801b4ce8, 0, 0), 0x5464037E);
//   kWrite32(SME_PORT_REGION(0x801FF850, 0x801f7734, 0, 0), 0x5404037E);
//   kWrite32(SME_PORT_REGION(0x802946B4, 0x8028c4cc, 0, 0), 0x5480043E);
//   kWrite32(SME_PORT_REGION(0x80294730, 0x8028c548, 0, 0), 0x5480043E);
//   kWrite32(SME_PORT_REGION(0x80294734, 0x8028c54c, 0, 0), 0x280003E7);
//   kWrite32(SME_PORT_REGION(0x80297BA0, 0x8028fa38, 0, 0), 0x5404037E);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x80294334, 0x8028c14c, 0, 0), Patch::Shine::extendFlagManagerLoad);
//   kWrite32(SME_PORT_REGION(0x80294338, 0x8028c150, 0, 0), 0x48000010);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x802939B8, 0x8028b7d0, 0, 0), Patch::Shine::extendFlagManagerSave);
//   kWrite32(SME_PORT_REGION(0x802939BC, 0x8028b7d4, 0, 0), 0x48000014);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x80297BD8, 0x8028fa70, 0, 0), Patch::Shine::thinkSetBootFlag);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x801BCD20, 0x801b4bd8, 0, 0), Patch::Shine::loadAfterMaskState);
//   kWrite32(SME_PORT_REGION(0x801BCD24, 0x801b4bdc, 0, 0), 0x28030002);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x801BCEEC, 0x801b4da4, 0, 0), Patch::Shine::setKillState);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x8029A590, 0x80292460, 0, 0), shine_thinkCloseCamera);
//   kWrite32(SME_PORT_REGION(0x8029A594, 80292464, 0, 0), 0x28000000);
//   KURIBO_PATCH_BL(SME_PORT_REGION(0x802999D8, 0x80291870, 0, 0), shine_animationFreezeCheck);
//   kWrite32(SME_PORT_REGION(0x802999DC, 0x80291874, 0, 0), 0x48000034);
//   // Make Shines glow more
//   kWrite32(SME_PORT_REGION(0x803C9190, 0x803c0980, 0, 0), 0x3F19999A);

  // stage.cpp
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80299230, 0x802910c8, 0, 0), Patch::Stage::setStageOnExit);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80175F58, 0x8016befc, 0, 0), Patch::Stage::startEpisodeSelect);

  // // yoshi.cpp
  // kWrite32(SME_PORT_REGION(0x8026E068, 0x80265df4, 0, 0), 0x2C000001); // Turn green when out of juice
  // kWrite32(SME_PORT_REGION(0x8026E0A0, 0x80265e28, 0, 0), 0x60000000); // No flickering
  // kWrite32(SME_PORT_REGION(0x8026EE14, 0x80266ba0, 0, 0), 0x4800020C); // Yoshi can't starve
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8026EB00, 0x80266bdc, 0, 0), Patch::Yoshi::isYoshiDie);
  // kWrite32(SME_PORT_REGION(0x8026EB04, 0x80266be0, 0, 0), 0x2C030000);
  // kWrite32(SME_PORT_REGION(0x8026EB08, 0x80266be4, 0, 0), 0x41820518);

  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8026EBFC, 0x80266988, 0, 0), Patch::Yoshi::isYoshiDie);
  // kWrite32(SME_PORT_REGION(0x8026EC00, 0x8026698c, 0, 0), 0x2C030000);
  // kWrite32(SME_PORT_REGION(0x8026EC04, 0x80266990, 0, 0), 0x4182041C);

  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8026F218, 0x80266fa4, 0, 0), Patch::Yoshi::isYoshiDie);
  // kWrite32(SME_PORT_REGION(0x8026F21C, 0x80266fa8, 0, 0), 0x2C030000);
  // kWrite32(SME_PORT_REGION(0x8026F220, 0x80266fac, 0, 0), 0x41820164);
  // // KURIBO_PATCH_BL(SME_PORT_REGION(0x801BC868, 0x801b4720, 0, 0), Patch::Yoshi::isYoshiEggNeedFruit);
  // // KURIBO_PATCH_BL(SME_PORT_REGION(0x801BC8B4, 0x801b476c, 0, 0), Patch::Yoshi::isYoshiEggFree);
  // kWrite32(SME_PORT_REGION(0x801BC8B8, 0x801b4770, 0, 0), 0xA01E00FC);
  // kWrite32(SME_PORT_REGION(0x801BC8BC, 0x801b4774, 0, 0), 0x2C00000B);
  // kWrite32(SME_PORT_REGION(0x801BC8C0, 0x801b4778, 0, 0), 0x418200E4);
  // kWrite32(SME_PORT_REGION(0x801BC8C4, 0x801b477c, 0, 0), 0x2C030001);
  // kWrite32(SME_PORT_REGION(0x801BC8C8, 0x801b4780, 0, 0), 0x4182003C);
  // kWrite32(SME_PORT_REGION(0x801BC8CC, 0x801b4784, 0, 0), 0x418100D8);
  // kWrite32(SME_PORT_REGION(0x801BC8D0, 0x801b4788, 0, 0), 0x48000134);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8024D68C, 0x80245418, 0, 0), Patch::Yoshi::isYoshiMaintainFluddModel);
  // kWrite32(SME_PORT_REGION(0x8024D690, 0x8024541c, 0, 0), 0x2C030000);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8024F240, 0x80246fcc, 0, 0), Patch::Yoshi::maybeYoshiDrown);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x802810F8, 0x80278e84, 0, 0), Patch::Yoshi::canMountYoshi);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x80281148, 0x80278ed4, 0, 0), Patch::Yoshi::getYoshiYPos_);
  // kWrite32(SME_PORT_REGION(0x802810FC, 0x80278e88, 0, 0), 0x2C030000);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8026E810, 0x8026659c, 0, 0), Patch::Yoshi::fixYoshiJuiceDecrement);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8024E58C, 0x80246318, 0, 0), Patch::Yoshi::canYoshiSpray);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x80273198, 0x8026af24, 0, 0), Patch::Yoshi::calcYoshiSwimVelocity);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x80270078, 0x80267e04, 0, 0), Patch::Yoshi::isYoshiWaterFlutter);
  // kWrite32(SME_PORT_REGION(0x8027007C, 0x80267e08, 0, 0), 0x7C7E1B78);
  // kWrite32(SME_PORT_REGION(0x80270080, 0x80267e0c, 0, 0), 0x60000000);
  // kWrite32(SME_PORT_REGION(0x80270084, 0x80267e10, 0, 0), 0x60000000);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8024E788, 0x80246514, 0, 0), Patch::Yoshi::isYoshiValidDrip);
  // KURIBO_PATCH_B(SME_PORT_REGION(0x801BC128, 0x801b3fe0, 0, 0), Patch::Yoshi::initFreeEggCard);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x801BC380, 0x801b4238, 0, 0), Patch::Yoshi::checkFreeEggCard);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8028121C, 0x80278fa8, 0, 0), Patch::Yoshi::saveNozzles);
  // KURIBO_PATCH_BL(SME_PORT_REGION(0x8024EC18, 0x802469a4, 0, 0), Patch::Yoshi::restoreNozzles);
  // kWrite32(SME_PORT_REGION(0x8024EC2C, 0x802469b8, 0, 0), 0x60000000);

//   // utilities/card.cpp
  KURIBO_PATCH_BL(SME_PORT_REGION(0x802B20F8, 0x802aa008, 0, 0), Patch::Card::mountCard);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80163C40, 0x80158bf0, 0, 0), Patch::Card::probeCard);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015B2C0, 0x8015030c, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015B2F8, 0x80150344, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015B5BC, 0x80150608, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015B5E4, 0x80150630, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015B638, 0x80150684, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015B660, 0x801506ac, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015BCB8, 0x80150d04, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015BCE0, 0x80150d2c, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015BE24, 0x80150e70, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015BE4C, 0x80150e98, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015BE9C, 0x80150ee8, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015BEC4, 0x80150f10, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015C508, 0x80151554, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015D194, 0x801521e0, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015D1CC, 0x80152218, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015DEFC, 0x80152f48, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015DF34, 0x80152f80, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015E34C, 0x80153398, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015E374, 0x801533c0, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015E3C8, 0x80153414, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015E3F0, 0x8015343c, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015F970, 0x801549bc, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8015F9A8, 0x801549f4, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80161320, 0x8015636c, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80161358, 0x801563a4, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016889C, 0x8015d954, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x801688D4, 0x8015d98c, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80169224, 0x8015e31c, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016925C, 0x8015e354, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016950C, 0x8015e604, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80169534, 0x8015e62c, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016958C, 0x8015e684, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x801695B4, 0x8015e6ac, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80169ACC, 0x8015ebc4, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x80169B04, 0x8015ebfc, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016A02C, 0x8015f164, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016A064, 0x8015f19c, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016A3D0, 0x8015f508, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016A3F8, 0x8015f530, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016A44C, 0x8015f584, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016A474, 0x8015f5ac, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016B19C, 0x801602d4, 0, 0), Patch::Card::formatCardMessage);
  KURIBO_PATCH_BL(SME_PORT_REGION(0x8016B1D4, 0x8016030c, 0, 0), Patch::Card::formatCardMessage);

//   /* -- PATCHES -- */

  // Restore Chao Seed
  kWrite32(SME_PORT_REGION(0x802FD1A0, 0x802f5330, 0, 0), 0x808D8C70);

  // Show Exception Handler
  kWrite32(SME_PORT_REGION(0x8029D0BC, 0x80294f98, 0, 0), 0x60000000);

#ifdef SME_DEBUG
  // Skip FMVs
  kWrite32(SME_PORT_REGION(0x802B5E8C, 0x802ade20, 0, 0), 0x38600001);
  kWrite32(SME_PORT_REGION(0x802B5EF4, 0x802ade88, 0, 0), 0x38600001);
#endif

#ifdef SME_DEBUG
  // Level Select
  kWrite32(SME_PORT_REGION(0x802A6788, 0x8029e6e0, 0, 0), 0x3BC00009);
#endif

  // Fix Infinte Flutter
  #if defined(PAL)
  kWrite32(SME_PORT_REGION(0x80278ec8, 0x80278ec8, 0, 0), 0xC002F69C);
  #else
  kWrite32(SME_PORT_REGION(0x8028113C, 0, 0, 0), 0xC002F824);
  #endif

  // Remove Dive While Wall Sliding
  kWrite32(SME_PORT_REGION(0x8024BC10, 0x8024399c, 0, 0), 0x48000068);

  // Flood Till Corona Beat
  kWrite32(SME_PORT_REGION(0x8029961C, 0x802914b4, 0, 0), 0x38840077);

  // Map on D Pad down
  kWrite32(SME_PORT_REGION(0x8017A830, 0x801706f4, 0, 0), 0x5400077B);
  kWrite32(SME_PORT_REGION(0x80297A60, 0x8028f8f8, 0, 0), 0x5400077B);

  // Hover move on roofs
  kWrite32(SME_PORT_REGION(0x802569BC, 0x8024e748, 0, 0), 0xC02300B0);
  // Need to reconsider this one for PAL
  // // Global surfing bloopies
  // kWrite32(SME_PORT_REGION(0x801B74F8, 0x801af3b0, 0, 0), 0x60000000);
  // kWrite32(SME_PORT_REGION(0x801B74FC, 0x801af3b4, 0, 0), 0x60000000);
  // kWrite32(SME_PORT_REGION(0x801B7500, 0x801af3b8, 0, 0), 0x60000000);
  // kWrite32(SME_PORT_REGION(0x801B7504, 0x801af3bc, 0, 0), 0x60000000);
  // kWrite32(SME_PORT_REGION(0x801B7508, 0x801af3c0, 0, 0), 0x60000000);
  // kWrite32(SME_PORT_REGION(0x801B750C, 0x801af3c4, 0, 0), 0x60000000);
  // kWrite32(SME_PORT_REGION(0x801B7510, 0x801af3c8, 0, 0), 0x387E0780);
  // kWrite32(SME_PORT_REGION(0x801B7514, 0x801af3cc, 0, 0), 0x4810BA9D);
  // kWrite32(SME_PORT_REGION(0x801B7518, 0x801af3d0, 0, 0), 0x28030000);
  // kWrite32(SME_PORT_REGION(0x801B751C, 0x801af3d4, 0, 0), 0x418200A4);
}
KURIBO_MODULE_END();