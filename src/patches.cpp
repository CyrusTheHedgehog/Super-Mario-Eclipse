#include "sms/JSystem/JKR/JKRFileLoader.hxx"
#include "sms/actor/Mario.hxx"
#include "sms/mapobj/MapObjTree.hxx"


#include "SME.hxx"
#include "macros.h"

// extern -> SME.cpp
// 0x802320E0
SME_PURE_ASM void SME::Patch::Fixes::shadowCrashPatch() {
  asm volatile("cmpwi       4, 0            \n\t"
               "li          0, 0            \n\t"
               "beqlr-                      \n\t"
               "lhz         0, 0x18 (4)     \n\t"
               "blr                         \n\t");
}

// extern -> SME.cpp
// 0x802571F0
u32 SME::Patch::Fixes::patchYStorage() {
  TMario *player;
  SME_FROM_GPR(31, player);

  if (player->mState != static_cast<u32>(TMario::State::IDLE))
    player->mSpeed.y = 0.0f;

  return 0;
}

static bool canDiePlane(f32 floorY) {
  TMario *player;
  SME_FROM_GPR(31, player);

  Vec playerPos;
  player->JSGGetTranslation(&playerPos);

  return (floorY > playerPos.y) && !player->mAttributes.mIsGameOver;
}
SME_PATCH_BL(SME_PORT_REGION(0x8024FB54, 0x802478e4, 0, 0), canDiePlane);
SME_WRITE_32(SME_PORT_REGION(0x8024FB58, 0x802478e8, 0, 0), 0x2C030000);
SME_WRITE_32(SME_PORT_REGION(0x8024FB5C, 0x802478ec, 0, 0), 0x41820084);

// make tree leaf count dynamic, based on number of leaf col files
static TMapObjTree *getLeafCount(TMapObjTree *tree) {
  char cacheBuffer[128];
  char buffer[128];

  snprintf(cacheBuffer, 96, "/scene/mapObj/%sLeaf", tree->mRegisterName);
  strcat(cacheBuffer, "%02d.col");

  s32 num = 0;
  while (true && num < 255) {
    snprintf(buffer, 100, cacheBuffer, num + 1);
    if (!JKRFileLoader::getGlbResource(buffer)) {
      tree->mLeafCount = num;
      return tree;
    }
    num += 1;
  }
  tree->mLeafCount = 0;
  return tree;
}
SME_PATCH_B(SME_PORT_REGION(0x801F6AE4, 0x801ee9bc, 0, 0), getLeafCount);
SME_PATCH_B(SME_PORT_REGION(0x801F6B20, 0x801ee9f8, 0, 0), getLeafCount);
SME_PATCH_B(SME_PORT_REGION(0x801F6B5C, 0x801eea34, 0, 0), getLeafCount);
SME_PATCH_B(SME_PORT_REGION(0x801F6B98, 0x801eea70, 0, 0), getLeafCount);
SME_PATCH_B(SME_PORT_REGION(0x801F6BD4, 0x801eeaac, 0, 0), getLeafCount);