#include "GX.h"
#include "OS.h"

#include "J2D/J2DPrint.hxx"
#include "JKR/JKRFileLoader.hxx"
#include "sms/GC2D/ConsoleStr.hxx"
#include "sms/actor/Mario.hxx"
#include "sms/mapobj/MapObjNormalLift.hxx"
#include "sms/mapobj/MapObjTree.hxx"

#include "SME.hxx"
#include "defines.h"
#include "macros.h"

constexpr f32 DrawDistance = 300000.0f * 100.0f;

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

void patchWaterDownWarp(f32 y) {
  TMario *player;
  SME_FROM_GPR(31, player);

  if (player->mFloorTriangleWater == player->mFloorTriangle)
    changePlayerStatus__6TMarioFUlUlb(player, TMario::State::FALL, 0, false);
  else
    player->mPosition.y = y;
}
SME_PATCH_BL(SME_PORT_REGION(0x80272710, 0x8026a49c, 0, 0), patchWaterDownWarp);

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

static SME_PURE_ASM bool makeWaterHitCheckForDeath(TBGCheckData *col) {
  // clang-format off
  asm volatile (
    "lhz 0, 0 (3)             \n\t"
    "cmpwi 0, 2048            \n\t"
    "bne .makeWaterCheck_tmp0 \n\t"
    "li 0, 1025               \n\t"
    ".makeWaterCheck_tmp0:    \n\t"
    SME_PORT_REGION (
      "lis 12, 0x8018           \n\t"
      "ori 12, 12, 0xC36C       \n\t",

      "lis 12, 0x8018           \n\t"
      "ori 12, 12, 0x4bf4       \n\t",

      "lis 12, 0           \n\t"
      "ori 12, 12, 0       \n\t",

      "lis 12, 0           \n\t"
      "ori 12, 12, 0       \n\t"
    )
    "mtctr 12                 \n\t"
    "bctr                     \n\t"
  );
  // clang-format on
}
SME_PATCH_B(SME_PORT_REGION(0x8018C368, 0x80184bf0, 0, 0), makeWaterHitCheckForDeath);

// Make illegal data not downwarp anymore
SME_WRITE_32(SME_PORT_REGION(0x8018D08C, 0x80185914, 0, 0), 0x60000000);

static void patchRideMovementUpWarp(Mtx out, Vec *ride, Vec *pos) {
  TMario *player;
  
  SME_FROM_GPR(30, player);

  if (!(player->mState & static_cast<u32>(TMario::State::AIRBORN))) {
    PSMTXMultVec(out, ride, pos);
  }
}
SME_PATCH_BL(SME_PORT_REGION(0x80250514, 0x802482a0, 0, 0), patchRideMovementUpWarp);

static void initBinaryNullptrPatch(TSpcBinary *binary) {
  if (binary)
    binary->init();
}
SME_PATCH_BL(SME_PORT_REGION(0x80289098, 0x80280e24, 0, 0), initBinaryNullptrPatch);

static void scaleDrawDistanceNPC(f32 x, f32 y, f32 near, f32 far) {
  SetViewFrustumClipCheckPerspective__Fffff(x, y, near, far * 2);
}
SME_PATCH_BL(SME_PORT_REGION(0x8020A2A4, 0x80202188, 0, 0), scaleDrawDistanceNPC);

static f32 sLastFactor = 1.0f;
static bool cameraQOLFixes(CPolarSubCamera *cam) {
  JSGSetProjectionFar__Q26JDrama7TCameraFf(cam, DrawDistance); // Draw Distance

  f32 factor = Math::scaleLinearAtAnchor<f32>(
      gpMarioAddress->mForwardSpeed / 100.0f, 0.5f, 1.0f);

  factor = Math::lerp<f32>(sLastFactor, factor, 0.01f);

  if (factor > 1.0f &&
      gpMarioAddress->mState == static_cast<u32>(TMario::State::DIVESLIDE)) {
    sLastFactor = factor;
    reinterpret_cast<f32 *>(cam)[0x48 / 4] *= factor;
  } else {
    sLastFactor = Math::lerp<f32>(sLastFactor, 1.0f, 0.01f);
    reinterpret_cast<f32 *>(cam)[0x48 / 4] *= sLastFactor;
  }
  return cam->isNormalDeadDemo();
}
SME_PATCH_BL(SME_PORT_REGION(0x80023828, 0x800238a4, 0, 0), cameraQOLFixes);

// READY GO TEXT PATCH FOR THIS BULLSHIT THING DADDY NINTENDO DID
SME_WRITE_32(SME_PORT_REGION(0x80171C30, 0x80167a40, 0, 0), 0x2C000005);
SME_WRITE_32(SME_PORT_REGION(0x80171C38, 0x80167a48, 0, 0), 0x38000005);

static void normalizeHoverSlopeSpeed(f32 floorPos) {
  TMario *player;
  SME_FROM_GPR(22, player);

  player->mPosition.y = floorPos;

  if (!(player->mState == static_cast<u32>(TMario::State::HOVER)))
    return;

  const f32 playerRotY = f32(player->mAngle.y) / 182.0f;
  const Vec playerForward = {sinf(Math::angleToRadians(-playerRotY)), 0.0f,
                             cosf(Math::angleToRadians(playerRotY))};
  const Vec up = {0.0f, 1.0f, 0.0f};

  Vec floorNormal;
  PSVECNormalize(reinterpret_cast<Vec *>(player->mFloorTriangle->getNormal()),
                 &floorNormal);

  const f32 slopeStrength = PSVECDotProduct(&up, &floorNormal);
  if (slopeStrength < 0.0f)
    return;

  const f32 lookAtRatio =
      Math::Vector3::lookAtRatio(playerForward, floorNormal);

  player->mForwardSpeed =
      Min(player->mForwardSpeed,
          10.0f * Math::clamp(Math::scaleLinearAtAnchor(slopeStrength,
                                                        lookAtRatio, 1.0f),
                              0.0f, 1.0f));
}
SME_PATCH_BL(SME_PORT_REGION(0x802568F0, 0x8024e67c, 0, 0), normalizeHoverSlopeSpeed);

static void touchEnemy__item(THitActor *touched) {
  //...
}

// Make enemies collect coins
// SME_WRITE_32(SME_PORT_REGION(0x803CA494, 0, 0, 0), 0x801bf2c0);

#if 0
static u8 sLineBuffer[sizeof(String) * 64];
static JKRSolidHeap sLineHeap(&sLineBuffer, 64, JKRHeap::sRootHeap, false);

static SME_NO_INLINE size_t getSplitLines(const char *str, String **out, size_t maxLines = __UINT32_MAX__) {
  String string(str, 1024);

  size_t nlinePos = string.find('\n', 0);
  size_t nlineLast = String::npos;
  size_t numLines = 0;
  do {
    out[numLines] = new (&sLineHeap, 4) String(nlineLast+1 + nlinePos);
    string.substr(out[numLines], nlineLast+1, nlinePos == String::npos ? String::npos : nlinePos + 1);
    numLines += 1;
  } while (nlinePos != String::npos && numLines < maxLines-1);

  return numLines;
}

static int sPrintX = 0, sPrintY = 0;

static void _capturePrintPos(J2DPane *pane, int x, int y) {
  sPrintX = x;
  sPrintY = y;
  pane->makeMatrix(x, y);
}
SME_PATCH_BL(SME_PORT_REGION(0x802d0bec, 0, 0, 0), _capturePrintPos);

static void cullJ2DPrint(J2DPrint *printer, int unk_0, int unk_1, u8 unk_2, const char *formatter, ...) {
  constexpr int fontWidth = 20;

  va_list vargs;
  va_start(vargs, formatter);

  char *msg = va_arg(vargs, char *);

  va_end(vargs);

  if (sPrintX > 0 && (sPrintX + strlen(msg)) < SME::TGlobals::getScreenWidth())
    printer->print(0, 0, unk_2, formatter, msg);

  String finalStr(1024);

  String *lines[64];
  size_t numLines = getSplitLines(msg, lines, 64);

  for (u32 i = 0; i < numLines; ++i) {
    String *line = lines[i];
    size_t cullL = Max((-sPrintX / fontWidth) - 2, 0);
    size_t cullR = ((-sPrintX + (line->size() * fontWidth)) / SME::TGlobals::getScreenWidth()) + 2;

    line->substr(const_cast<char *>(finalStr.data()) + finalStr.end(), cullL, cullR);
  }

  printer->print(0, 0, unk_2, formatter, finalStr.data());
}
SME_PATCH_BL(SME_PORT_REGION(0x802d0c20, 0, 0, 0), cullJ2DPrint);

#else

static int sPrintX = 0, sPrintY = 0;

static void _capturePrintPos(J2DPane *pane, int x, int y) {
  sPrintX = x;
  sPrintY = y;
  pane->makeMatrix(x, y);
}
// SME_PATCH_BL(SME_PORT_REGION(0x802d0bec, 0, 0, 0), _capturePrintPos);

static void cullJ2DPrint(J2DPrint *printer, int unk_0, int unk_1, u8 unk_2,
                         const char *formatter, ...) {
  constexpr f32 fontWidth = 16;

  va_list vargs;
  va_start(vargs, formatter);

  char *msg = va_arg(vargs, char *);

  va_end(vargs);

  if ((sPrintX > 0 &&
       (sPrintX + strlen(msg)) < SME::TGlobals::getScreenWidth()) ||
      strchr(msg, '\n') != nullptr)
    printer->print(0, 0, unk_2, formatter, msg);

  String culledMsg(msg, 1024);

  f32 cullL = Max((f32(-sPrintX) / fontWidth) - 2.0f, 0);
  f32 cullR = Max(((f32(-sPrintX) + (f32(culledMsg.size()) * fontWidth)) /
                   SME::TGlobals::getScreenWidth()) +
                      2.0f,
                  cullL);

  culledMsg.substr(&culledMsg, size_t(cullL), size_t(cullR));
  culledMsg.append('\0');

  printer->print(int(cullL * fontWidth), 0, unk_2, formatter, culledMsg.data());
}
// SME_PATCH_BL(SME_PORT_REGION(0x802d0c20, 0, 0, 0), cullJ2DPrint);

static Mtx44 sDrawMtx;

static void captureTextboxDrawMtx(Mtx44 mtx, u8 index) {
  PSMTXCopy(mtx, sDrawMtx);
  GXLoadPosMtxImm(mtx, index);
}
SME_PATCH_BL(SME_PORT_REGION(0x802d0bf8, 0x802c8da0, 0, 0), captureTextboxDrawMtx);

static void maybePrintChar(JUTFont *font, f32 x, f32 y, f32 w, f32 h, int ascii,
                           bool unk_1) {
  const int offset = static_cast<int>(
      (SME::TGlobals::getScreenToFullScreenRatio() - 1.0f) * 600.0f);
  const int fontWidth = font->getWidth();

  int absX = static_cast<int>(sDrawMtx[0][3] + x);
  int absY = static_cast<int>(sDrawMtx[1][3] + y);

  if (absX + fontWidth > -offset && absX < SME::TGlobals::getScreenWidth())
    font->drawChar_scale(x, y, w, h, ascii, unk_1);
}
SME_PATCH_BL(SME_PORT_REGION(0x802cec2c, 0x802c6dd4, 0, 0), maybePrintChar);

static OSStopwatch stopwatch;
static bool sInitialized = false;
static bool sIsWaiting = false;
static OSTick sLastStart = 0;
static void J2D_BenchMarkPrint(J2DTextBox *printer, int x, int y) {
  if (!sInitialized) {
    OSInitStopwatch(&stopwatch, "J2DPrintTest");
    sInitialized = true;
  }

  if (!sIsWaiting) {
    sLastStart = OSGetTick();
    sIsWaiting = true;
  }

  OSStartStopwatch(&stopwatch);
  printer->draw(x, y);
  OSStopStopwatch(&stopwatch);

  if (sIsWaiting && OSTicksToSeconds(OSGetTick() - sLastStart) > 5.0f) {
    OSDumpStopwatch(&stopwatch);
  }
}
// SME_PATCH_BL(SME_PORT_REGION(0x80144010, 0, 0, 0), J2D_BenchMarkPrint);

#endif

// Title Screen Never Fades to THP
SME_WRITE_32(SME_PORT_REGION(0x8016D53C, 0x801628ac, 0, 0), 0x48000344);

// Load msound.aaf from AudioRes folder (NTSC-U) [Xayrga/JoshuaMK]
// SME_WRITE_32(SME_PORT_REGION(0x80014F9C, 0x80014ff8, 0, 0), 0x60000000);

static bool sIs100ShineSpawned = false;
static bool is100CoinShine(TFlagManager *manager, u32 id) {
  if (!sIs100ShineSpawned && manager->getFlag(id) > 99) {
    sIs100ShineSpawned = true;
    return true;
  }
  return false;
}
SME_PATCH_BL(SME_PORT_REGION(0x801BED3C, 0x801b6bf4, 0, 0), is100CoinShine);
SME_WRITE_32(SME_PORT_REGION(0x801BED40, 0x801b6bf8, 0, 0), 0x2C030001);

static void *loadFromGlobalAndScene(const char *mdl, u32 unk_0,
                                    const char *path) {
  u32 **sdlModel = reinterpret_cast<u32 **>(
      loadModelData__16TModelDataKeeperFPCcUlPCc(mdl, unk_0, path));
  if (*sdlModel == nullptr) {
    delete sdlModel;
    sdlModel =
        reinterpret_cast<u32 **>(loadModelData__16TModelDataKeeperFPCcUlPCc(
            mdl, unk_0, "/common/mapobj"));
  }
  return sdlModel;
}
SME_PATCH_BL(SME_PORT_REGION(0x8021CD34, 0x80214c88, 0, 0), loadFromGlobalAndScene);

void checkInstantReset_NormalLift(u32 *railflags) {
  s16 *mRailObj;
  SME_FROM_GPR(31, mRailObj);

  u32 flag = railflags[2];
  if (flag & 0x2000) {
    mRailObj[0x14A / 2] = 0;
  } else {
    mRailObj[0x14A / 2] = 180;
  }
}

void checkInstantReset_RailObj(s16 *mRailObj, u32 *railflags) {
  u32 flag = railflags[2];
  if (flag & 0x2000) {
    mRailObj[0x14A / 2] = 0;
  } else {
    mRailObj[0x14A / 2] = 180;
  }
}
SME_PATCH_BL(SME_PORT_REGION(0x801F0B90, 0x801e8a68, 0, 0),
             checkInstantReset_NormalLift);
SME_PATCH_BL(SME_PORT_REGION(0x801F1054, 0x801e8f2c, 0, 0), checkInstantReset_RailObj);

void checkResetToNode(TNormalLift *lift) {
  TGraphWeb *graph = lift->mGraphTracer->mGraph;
  TRailNode *node;
  {
    s32 nodeIdx = lift->mGraphTracer->mPreviousNode;
    node = reinterpret_cast<TRailNode *>(graph->mNodes[nodeIdx << 2]);
  }
  if (node->mFlags & 0x2000) {
    lift->mPosition.set(
        graph->getNearestPosOnGraphLink(lift->mInitialPosition));
    lift->mRotation.set(lift->mInitialRotation);
    lift->mRailStatus = 0;
    lift->mContextTimer = 180;
    lift->mLastRailStatus = 1;
    {
      int idx = graph->findNearestNodeIndex(lift->mPosition, 0xFFFFFFFF);
      lift->mGraphTracer->setTo(idx);
    }
    lift->readRailFlag();
  } else {
    lift->resetPosition();
  }
}
SME_PATCH_BL(SME_PORT_REGION(0x801EFBDC, 0x801e7ab4, 0, 0), checkResetToNode);
SME_WRITE_32(SME_PORT_REGION(0x801EFBE0, 0x801e7ab8, 0, 0), 0x60000000);
SME_WRITE_32(SME_PORT_REGION(0x801EFBE4, 0x801e7abc, 0, 0), 0x60000000);
SME_WRITE_32(SME_PORT_REGION(0x801EFBE8, 0x801e7ac0, 0, 0), 0x60000000);
SME_PATCH_BL(SME_PORT_REGION(0x801F13FC, 0x801e92d4, 0, 0), checkResetToNode);
SME_WRITE_32(SME_PORT_REGION(0x801F1400, 0x801e92d8, 0, 0), 0x60000000);
SME_WRITE_32(SME_PORT_REGION(0x801F1404, 0x801e92dc, 0, 0), 0x60000000);
SME_WRITE_32(SME_PORT_REGION(0x801F1408, 0x801e92e0, 0, 0), 0x60000000);

f32 enhanceWaterCheck(f32 x, f32 y, f32 z, TMario *player) {
  SME_FROM_GPR(29, player);

  const TBGCheckData **tri =
      const_cast<const TBGCheckData **>(&player->mFloorTriangleWater);

  const TMapCollisionData *mapCol = gpMapCollisionData;
  {
    f32 yPos = mapCol->checkGround(x, player->mCeilingAbove - 10.0f, z, 0, tri);
    if (*tri && (*tri)->mCollisionType > 255 && (*tri)->mCollisionType < 260)
      return yPos;
  }

  return mapCol->checkGround(x, y, z, 0, tri);
}
SME_PATCH_BL(SME_PORT_REGION(0x8024F12C, 0x80246eb8, 0, 0), enhanceWaterCheck);

u32 clampRotation(TLiveActor *actor) {
  JGeometry::TVec3<f32> &rot = actor->mRotation;

  auto clampPreserve = [](f32 rotation) {
    if (rotation > 360.0f)
      rotation -= 360.0f;
    else if (rotation < -360.0f)
      rotation += 360.0f;
    return rotation;
  };

  rot.x = clampPreserve(rot.x);
  rot.y = clampPreserve(rot.y);
  rot.z = clampPreserve(rot.z);

  return actor->mStateFlags.asU32;
}
SME_PATCH_BL(SME_PORT_REGION(0x80217EDC, 0x8020fdc4, 0, 0), clampRotation);
SME_WRITE_32(SME_PORT_REGION(0x80217EE0, 0x8020fdc8, 0, 0), 0x70600201);


// STATIC RESETTER
void patches_staticResetter() { sIs100ShineSpawned = false; }
