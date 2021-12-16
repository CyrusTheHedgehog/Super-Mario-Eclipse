#include "SME.hxx"
#include "types.h"

#include "sms/G2D/ExPane.hxx"
#include "sms/JSystem/JUT/JUTRect.hxx"
#include "sms/game/GCConsole2.hxx"

#include "sms/GC2D/SelectDir.hxx"
#include "sms/GC2D/SelectMenu.hxx"
#include "sms/manager/SelectShineManager.hxx"

#if 1

static f32 getScreenTransX() {
  return (SME::TGlobals::getScreenToFullScreenRatio() - 1.0f) * 600.0f;
}

static f32 getScreenWidthTranslated() {
  return SME::TGlobals::getScreenWidth() + getScreenTransX();
}
SME_PATCH_BL(SME_PORT_REGION(0x802A3E78, 0x8029bd54, 0, 0), getScreenWidthTranslated);
SME_WRITE_32(SME_PORT_REGION(0x802A3E7C, 0x8029bd58, 0, 0), 0xD03C0038);

static f32 getNegScreenTransX() { return -getScreenTransX(); }
SME_PATCH_BL(SME_PORT_REGION(0x80176AA4, 0x8016ca6c, 0, 0), getNegScreenTransX);
SME_WRITE_32(SME_PORT_REGION(0x80176AA8, 0x8016ca70, 0, 0), 0xD03B0030);
SME_PATCH_BL(SME_PORT_REGION(0x8029B974, 0x80293850, 0, 0), getNegScreenTransX);
SME_WRITE_32(SME_PORT_REGION(0x8029B978, 0x80293854, 0, 0), 0xD0350030);
SME_PATCH_BL(SME_PORT_REGION(0x80176C40, 0x8016cc00, 0, 0), getNegScreenTransX);
SME_WRITE_32(SME_PORT_REGION(0x80176C44, 0x8016cc04, 0, 0), 0xD03B0030);
SME_PATCH_BL(SME_PORT_REGION(0x80176FF4, 0x8016d160, 0, 0), getNegScreenTransX);
SME_WRITE_32(SME_PORT_REGION(0x80176FF8, 0x8016d164, 0, 0), 0xD03B0030);
SME_PATCH_BL(SME_PORT_REGION(0x80177198, 0x8016cfbc, 0, 0), getNegScreenTransX);
SME_WRITE_32(SME_PORT_REGION(0x8017719c, 0x8016cfc0, 0, 0), 0xD03B0030);

SME_PATCH_BL(SME_PORT_REGION(0x80176AB4, 0x8016ca7c, 0, 0),
             SME::TGlobals::getScreenWidth);
SME_WRITE_32(SME_PORT_REGION(0x80176AB8, 0x8016ca80, 0, 0), 0xD03B0038);
SME_PATCH_BL(SME_PORT_REGION(0x8029B984, 0x80293860, 0, 0),
             SME::TGlobals::getScreenWidth);
SME_WRITE_32(SME_PORT_REGION(0x8029B988, 0x80293864, 0, 0), 0xD0350038);
SME_PATCH_BL(SME_PORT_REGION(0x80176C50, 0x8016cc10, 0, 0),
             SME::TGlobals::getScreenWidth);
SME_WRITE_32(SME_PORT_REGION(0x80176C54, 0x8016cc14, 0, 0), 0xD03B0038);
SME_PATCH_BL(SME_PORT_REGION(0x80177004, 0x8016d170, 0, 0),
             SME::TGlobals::getScreenWidth);
SME_WRITE_32(SME_PORT_REGION(0x80177008, 0x8016d174, 0, 0), 0xD03B0038);
SME_PATCH_BL(SME_PORT_REGION(0x801771A8, 0x8016cfcc, 0, 0),
             SME::TGlobals::getScreenWidth);
SME_WRITE_32(SME_PORT_REGION(0x801771AC, 0x8016cfd0, 0, 0), 0xD03B0038);

static f32 getScreenXRatio2() {
  const f32 ratio = SME::TGlobals::getScreenToFullScreenRatio();
  return ratio + (ratio - 1.0f);
}

static f32 getShineSelectXRatio() { return getScreenXRatio2() * 1.33333337307; }

static f32 getCameraXRatio() { return getScreenXRatio2() * 0.913461446762f; }

// Shine Select Model Rot Width
SME_PATCH_BL(SME_PORT_REGION(0x80176E58, 0x8016ce20, 0, 0), getShineSelectXRatio);
SME_WRITE_32(SME_PORT_REGION(0x80176E5C, 0x8016ce24, 0, 0), 0xD03B004C);

// Camera Width
SME_WRITE_32(SME_PORT_REGION(0x802B8B6C, 0x802b0b3c, 0, 0), 0x90010AE4);
SME_PATCH_BL(SME_PORT_REGION(0x802B8B70, 0x802b0b40, 0, 0), getCameraXRatio);
SME_WRITE_32(SME_PORT_REGION(0x802B8B74, 0x802b0b44, 0, 0), SME_PORT_REGION(0xC842FFF0, 0xc842fe70, 0, 0));
SME_WRITE_32(SME_PORT_REGION(0x802B8B78, 0x802b0b48, 0, 0), 0x3C80803E);
SME_WRITE_32(SME_PORT_REGION(0x802B8B7C, 0x802b0b4c, 0, 0), 0x3C60803E);
SME_WRITE_32(SME_PORT_REGION(0x802B8B88, 0x802b0b58, 0, 0), 0xC8010AE0);
SME_WRITE_32(SME_PORT_REGION(0x802B8B94, 0x802b0b64, 0, 0), 0xEC001028);
SME_WRITE_32(SME_PORT_REGION(0x802B8B9C, 0x802b0b6c, 0, 0), 0xEC010032);

#if 1

static void scaleNintendoIntro(JUTRect *rect, int x1, int y1, int x2, int y2) {
  const f32 translate = getScreenTransX();

  x1 -= translate;
  x2 += translate;

  rect->set(x1, y1, x2, y2);
}
 SME_PATCH_BL(SME_PORT_REGION(0x80296078, 0x8028df10, 0, 0), scaleNintendoIntro);

static void scaleGCConsoleExPane140(TExPane *pane) {
  TGCConsole2 *console;
  SME_FROM_GPR(31, console);

  pane->mRect.mX1 -= getScreenTransX();

  reinterpret_cast<TExPane **>(console)[0x140 / 4] = pane;
}
SME_PATCH_BL(SME_PORT_REGION(0x8014EF74, 0x80143c04, 0, 0), scaleGCConsoleExPane140);

static void scaleGCConsoleExPane108(TExPane *pane) {
  TGCConsole2 *console;
  SME_FROM_GPR(31, console);

  pane->mRect.mX1 -= getScreenTransX();

  reinterpret_cast<TExPane **>(console)[0x108 / 4] = pane;
}
SME_PATCH_BL(SME_PORT_REGION(0x8014EE24, 0x80143ab4, 0, 0), scaleGCConsoleExPane108);

static void scaleGCConsoleExPane160(TExPane *pane) {
  TGCConsole2 *console;
  SME_FROM_GPR(31, console);

  pane->mRect.mX1 -= getScreenTransX();

  reinterpret_cast<TExPane **>(console)[0x160 / 4] = pane;
}
SME_PATCH_BL(SME_PORT_REGION(0x8014F09C, 0x80143d2c, 0, 0), scaleGCConsoleExPane160);

static void scaleGCConsoleExPane2F8(TExPane *pane) {
  TGCConsole2 *console;
  SME_FROM_GPR(31, console);

  pane->mRect.mX1 += getScreenTransX();

  reinterpret_cast<TExPane **>(console)[0x2F8 / 4] = pane;
}
SME_PATCH_BL(SME_PORT_REGION(0x8014F308, 0x80143f98, 0, 0), scaleGCConsoleExPane2F8);

static void scaleGCConsoleExPane400(TExPane *pane) {
  TGCConsole2 *console;
  SME_FROM_GPR(31, console);

  pane->mRect.mX1 -= getScreenTransX();

  reinterpret_cast<TExPane **>(console)[0x400 / 4] = pane;
}
SME_PATCH_BL(SME_PORT_REGION(0x8014F70C, 0x8014439c, 0, 0), scaleGCConsoleExPane400);

static void scaleGCConsoleExPane42C(TExPane *pane) {
  TGCConsole2 *console;
  SME_FROM_GPR(31, console);

  pane->mRect.mX1 -= getScreenTransX();

  reinterpret_cast<TExPane **>(console)[0x42C / 4] = pane;
}
SME_PATCH_BL(SME_PORT_REGION(0x8014F830, 0x801444c0, 0, 0), scaleGCConsoleExPane42C);

static void scaleGCConsoleExPane450(TExPane *pane) {
  TGCConsole2 *console;
  SME_FROM_GPR(31, console);

  pane->mRect.mX1 -= getScreenTransX();

  reinterpret_cast<TExPane **>(console)[0x450 / 4] = pane;
}
SME_PATCH_BL(SME_PORT_REGION(0x8014F93C, 0x801445cc, 0, 0), scaleGCConsoleExPane450);

static void scaleGCConsoleLoadAfter2F8(JUTRect *dst, const JUTRect &ref) {
  dst->copy(ref);
  dst->mX1 += getScreenTransX();
}
SME_PATCH_BL(SME_PORT_REGION(0x8014D8E8, 0x80142578, 0, 0), scaleGCConsoleLoadAfter2F8);

static void scaleGCConsoleLoadAfter550Add(JUTRect *src, int x, int y) {
  x += getScreenTransX();
  src->add(x, y);
}
 SME_PATCH_BL(SME_PORT_REGION(0x8014E7E0, 0x80143470, 0, 0),
               scaleGCConsoleLoadAfter550Add);

static void setRecursivePanePos(JSUInputStream *stream, u16 *dst, size_t len) {
  stream->read(dst, len);

  if (*dst == 415) {
    *dst += getScreenTransX();
  } else if (*dst == 397) {
    *dst += getScreenTransX();
  }
}
SME_PATCH_BL(SME_PORT_REGION(0x802CB320, 0x802c33b4, 0, 0), setRecursivePanePos);

static void scaleGCConsoleExPane1C4(TExPane *pane) {
  TGCConsole2 *console;
  SME_FROM_GPR(31, console);

  pane->mRect.mX1 += getScreenTransX();

  reinterpret_cast<TExPane **>(console)[0x1C4 / 4] = pane;
}
SME_PATCH_BL(SME_PORT_REGION(0x8014F114, 0x80143da4, 0, 0), scaleGCConsoleExPane1C4);

static void scalePauseMenuMask(J2DScreen *screen, int x, int y,
                               J2DGrafContext *context) {
  u32 *tPauseMenu;
  SME_FROM_GPR(31, tPauseMenu);

  J2DPane *pane = reinterpret_cast<J2DPane *>(tPauseMenu[0x18 / 4]);
  pane->mRect.mX1 -= getScreenTransX();
  pane->mRect.mX2 += getScreenTransX();

  screen->draw(x, y, context);
}
SME_PATCH_BL(SME_PORT_REGION(0x8015600C, 0x8014b028, 0, 0), scalePauseMenuMask);

static void scaleSelectMenuMask(TSelectMenu *menu) {
  TExPane *pane;

  pane = menu->mMask1;
  pane->mRect.mX1 -= getScreenTransX();
  pane->mRect.mX2 += getScreenTransX();
  pane->mPane->mRect.copy(pane->mRect);

  pane = menu->mMask2;
  pane->mRect.mX1 -= getScreenTransX();
  pane->mRect.mX2 += getScreenTransX();
  pane->mPane->mRect.copy(pane->mRect);

  startMove__11TSelectMenuFv(menu);
}
SME_PATCH_BL(SME_PORT_REGION(0x80175F50, 0x8016bef4, 0, 0), scaleSelectMenuMask);

static void scaleSelectMenuGrad(u32 type, u32 idx, u32 count) {
  TSelectGrad *grad;
  SME_FROM_GPR(31, grad);

  u8 yColorR = (grad->mColorX2.r + grad->mColorX1.r) / 2;
  u8 yColorG = (grad->mColorX2.g + grad->mColorX1.g) / 2;
  u8 yColorB = (grad->mColorX2.b + grad->mColorX1.b) / 2;

  GXBegin(type, idx, count);

  GXPosition3f32(-getScreenTransX(), 16.0f, -100.0f);
  GXColor3u8(grad->mColorX1.r, grad->mColorX1.g, grad->mColorX1.b);

  GXPosition3f32(SME::TGlobals::getScreenWidth(), 16.0f, -100.0f);
  GXColor3u8(yColorR, yColorG, yColorB);

  GXPosition3f32(SME::TGlobals::getScreenWidth(), 464.0f, -100.0f);
  GXColor3u8(grad->mColorX2.r, grad->mColorX2.g, grad->mColorX2.b);

  GXPosition3f32(-getScreenTransX(), 464.0f, -100.0f);
  GXColor3u8(yColorR, yColorG, yColorB);
}
SME_PATCH_BL(SME_PORT_REGION(0x80175868, 0x8016b80c, 0, 0), scaleSelectMenuGrad);
SME_WRITE_32(SME_PORT_REGION(0x8017586C, 0x8016b810, 0, 0), 0x48000090);

static u32 fixShootingStarsNoDelete() {
  u32 *emitterManager4D2 = *(u32 **)SME_PORT_REGION(0x8040E1E4, 0x804058bc, 0, 0);
  TConsoleStr *consoleStr;
  SME_FROM_GPR(31, consoleStr);

  for (s32 i = 2; i >= 0; --i) {
    deleteEmitter__17JPAEmitterManagerFP14JPABaseEmitter(
        emitterManager4D2, consoleStr->mJPABaseEmitters[i]);
    consoleStr->mJPABaseEmitters[i] = nullptr;
  }

  return 0;
}
// SME_PATCH_BL(SME_PORT_REGION(0x80171314, 0x800fac3c, 0, 0), fixShootingStarsNoDelete);
// SME_WRITE_32(SME_PORT_REGION(0x80171318, 0x800fac40, 0, 0), 0x809F0028);

static void fixShootingStarsWideScreen(TBoundPane *pane, u32 size,
                                       JUTPoint &begin, JUTPoint &mid,
                                       JUTPoint &end) {
  const f32 ratio = SME::TGlobals::getScreenToFullScreenRatio() * 1.2f;

  mid.x *= ratio;
  end.x *= ratio;

  pane->setPanePosition(size, begin, mid, end);
}
SME_PATCH_BL(SME_PORT_REGION(0x80170EFC, 0x80166d08, 0, 0), fixShootingStarsWideScreen);
SME_PATCH_BL(SME_PORT_REGION(0x80170F34, 0x80166d54, 0, 0), fixShootingStarsWideScreen);
SME_PATCH_BL(SME_PORT_REGION(0x80170F6C, 0x80166da0, 0, 0), fixShootingStarsWideScreen);

static void fixDemoMasksWideScreen_InitStaticGoPanes(TConsoleStr *consoleStr) {
  loadAfter__Q26JDrama8TNameRefFv(consoleStr);

  const f32 ratio = SME::TGlobals::getScreenToFullScreenRatio();

  for (u32 i = 0; i < 2; ++i) {
    JUTRect &rect = consoleStr->mDemoWipeExPanes[i]->mPane->mRect;
    rect.mX1 = static_cast<s32>(-((ratio - 1) * 600.0f));
    rect.mX2 = static_cast<s32>(SME::TGlobals::getScreenWidth());

    consoleStr->mDemoWipeExPanes[i]->mRect.copy(rect);
  }

  JUTRect *rect = &consoleStr->mDemoMaskExPanes[0]->mPane->mRect;
  rect->mX1 = static_cast<s32>(-((ratio - 1) * 600.0f));

  consoleStr->mDemoMaskExPanes[0]->mRect.copy(*rect);

  rect = &consoleStr->mDemoMaskExPanes[1]->mPane->mRect;
  rect->mX2 = static_cast<s32>(SME::TGlobals::getScreenWidth());

  consoleStr->mDemoMaskExPanes[1]->mRect.copy(*rect);
}



SME_PATCH_BL(SME_PORT_REGION(0x801723F0, 0x801681e0, 0, 0),
             fixDemoMasksWideScreen_InitStaticGoPanes);

static JUTRect sGuideBorderRects[2];
static J2DPane sGuideBorderPanes[2];

static void fixGuideWideScreenOpen(TSMSFader *fader, u32 type, f32 time,
                                   f32 delay) {
  TGuide *guide;
  SME_FROM_GPR(29, guide);

  fader->startWipe(type, time, delay);

  guide->mScreen->mRect.mX2 = 600;

  TConsoleStr *consoleStr = gpMarDirector->mGCConsole->mConsoleStr;
  const f32 ratio = SME::TGlobals::getScreenToFullScreenRatio();

  JUTRect *rect = &consoleStr->mDemoMaskExPanes[0]->mPane->mRect;
  J2DPane *pane = consoleStr->mDemoMaskExPanes[0]->mPane;

  sGuideBorderRects[0] = *rect;
  sGuideBorderPanes[0] = *pane;

  rect->mX1 = static_cast<s32>(-((ratio - 1) * 600.0f));
  rect->mX2 = guide->mScreen->mRect.mX1;
  pane->mAlpha = 0xFF;
  pane->mIsVisible = true;

  consoleStr->mDemoMaskExPanes[0]->mRect.copy(*rect);

  rect = &consoleStr->mDemoMaskExPanes[1]->mPane->mRect;
  pane = consoleStr->mDemoMaskExPanes[1]->mPane;

  sGuideBorderRects[1] = *rect;
  sGuideBorderPanes[1] = *pane;

  rect->mX1 = guide->mScreen->mRect.mX2;
  rect->mX2 = static_cast<s32>(SME::TGlobals::getScreenWidth());
  pane->mAlpha = 0xFF;
  pane->mIsVisible = true;

  consoleStr->mDemoMaskExPanes[1]->mRect.copy(*rect);
}
SME_PATCH_BL(SME_PORT_REGION(0x8017940C, 0x8016f2b4, 0, 0), fixGuideWideScreenOpen);

static void fixGuideWideScreenClose(TSMSFader *fader, u32 type, f32 time,
                                    f32 delay) {
  fader->startWipe(type, time, delay);

  TConsoleStr *consoleStr = gpMarDirector->mGCConsole->mConsoleStr;

  JUTRect *rect = &consoleStr->mDemoMaskExPanes[0]->mRect;
  J2DPane *pane = consoleStr->mDemoMaskExPanes[0]->mPane;

  *rect = sGuideBorderRects[0];
  *pane = sGuideBorderPanes[0];

  rect = &consoleStr->mDemoMaskExPanes[1]->mRect;
  pane = consoleStr->mDemoMaskExPanes[1]->mPane;

  *rect = sGuideBorderRects[1];
  *pane = sGuideBorderPanes[1];
}
SME_PATCH_BL(SME_PORT_REGION(0x80179880, 0x8016f738, 0, 0), fixGuideWideScreenClose);

static void renderGuideWideScreenFix(J2DScreen *screen, int x, int y,
                                     J2DGrafContext *context) {
  TConsoleStr *consoleStr = gpMarDirector->mGCConsole->mConsoleStr;
  consoleStr->mOpeningDemoScreen->draw(x, y, context);

  screen->draw(x, y, context);
}
SME_PATCH_BL(SME_PORT_REGION(0x80179390, 0x8016f238, 0, 0), renderGuideWideScreenFix);

static void scaleGuideMap(TGuide *guide) {
  resetObjects__6TGuideFv(guide);

  J2DPane *pane = reinterpret_cast<J2DPane *>(
      reinterpret_cast<J2DPane *>(
          guide->mScreen->mChildrenList.mFirst->mItemPtr)
          ->mChildrenList.mFirst->mItemPtr);
  pane->mRect.mX2 = 600.0f;
}
SME_PATCH_BL(SME_PORT_REGION(0x8017CB64, 0x80172b94, 0, 0), scaleGuideMap);

// -- DEBS -- //

static f32 sDEBSToTimerRatio = 0.0f;
static bool sHasRedAppeared = false;
static JUTRect sPaneRect;
static JUTRect sFillRect;

void setDEBSWidthByRedCoinTimer(TGCConsole2 *console, bool forceAppear) {
  sDEBSToTimerRatio =
      Math::lerp<f32>(0.0f, 1.0f, f32(console->mRedCoinCardTimer) / 117.1f);

  TExPane *pane = console->mTelopWindow;
  pane->mRect.mX1 = sPaneRect.mX1 + (180.0f * sDEBSToTimerRatio);
  pane->mPane->mRect.mX1 = sPaneRect.mX1 + (180.0f * sDEBSToTimerRatio);
  reinterpret_cast<J2DWindow *>(pane->mPane)->mFillRect.mX2 = sFillRect.mX2 - (180.0f * sDEBSToTimerRatio);

  startAppearTelop__11TGCConsole2Fb(console, forceAppear);
}

static void fixDEBSWideScreenText(s32 x1, s32 y1, s32 width, s32 height) {
  TGCConsole2 *console = gpMarDirector->mGCConsole;
  const f32 ratio = SME::TGlobals::getScreenToFullScreenRatio();

  sDEBSToTimerRatio =
      Math::lerp<f32>(0.0f, 1.0f, f32(console->mRedCoinCardTimer) / 117.1f);

  TExPane *pane = console->mTelopWindow;
  pane->mRect.mX1 = sPaneRect.mX1 + (180.0f * sDEBSToTimerRatio);
  pane->mPane->mRect.mX1 = sPaneRect.mX1 + (180.0f * sDEBSToTimerRatio);
  reinterpret_cast<J2DWindow *>(pane->mPane)->mFillRect.mX2 = sFillRect.mX2 - (180.0f * sDEBSToTimerRatio);

  const s32 offset =
      static_cast<s32>(((ratio - 1.0f) * 45.0f));
  x1 -= offset;

  GXSetScissor(x1 + (195.0f * sDEBSToTimerRatio), y1, width - (195.0f * sDEBSToTimerRatio), height);
}
// SME_PATCH_BL(SME_PORT_REGION(0x80143FDC, 0x80138cac, 0, 0), fixDEBSWideScreenText);

static void fixDEBSWideScreenPanel(TGCConsole2 *console) {
  const f32 ratio = SME::TGlobals::getScreenToFullScreenRatio();
  const s32 offset = getScreenTransX();

  TExPane *pane = console->mTelopWindow;
  pane->mRect.mX1 -= offset;
  pane->mRect.mX2 += offset;
  pane->mPane->mRect.mX1 -= offset;
  pane->mPane->mRect.mX2 += offset;
  reinterpret_cast<J2DWindow *>(pane->mPane)->mFillRect.mX2 += offset * 2;

  sPaneRect = pane->mRect;
  sFillRect = reinterpret_cast<J2DWindow *>(pane->mPane)->mFillRect;

  sDEBSToTimerRatio = 0.0f;
  console->mRedCoinCardTimer = 0;
  sHasRedAppeared = false;
}

static void fixDeathScreenRatio(u32 *cardsave, TMarioGamePad *gamepad) {
  initData__9TCardSaveFP13TMarioGamePad(cardsave, gamepad);
  
  const s32 offset = getScreenTransX();
  J2DPane *pane = reinterpret_cast<J2DScreen *>(cardsave[0x14 / 4])->search('mask');
  
  pane->mRect.mX1 -= offset;
  pane->mRect.mX2 += offset;
}
SME_PATCH_BL(SME_PORT_REGION(0x80163468, 0x801584b4, 0, 0), fixDeathScreenRatio);

static void fixYoshiFruitText(TGCConsole2 *console) {
  const f32 ratio = SME::TGlobals::getScreenToFullScreenRatio();
  const s32 offset = getScreenTransX();

  J2DPicture *pane = reinterpret_cast<J2DPicture **>(console)[0x314 / 4];
  pane->mRect.mX1 += offset;
  pane->mRect.mX2 += offset;
}

static void loadAfterGCConsolePatches(TGCConsole2 *console) {
  loadAfter__Q26JDrama8TNameRefFv(console);
  fixDEBSWideScreenPanel(console);
  fixYoshiFruitText(console);
}
SME_PATCH_BL(SME_PORT_REGION(0x8014D8A4, 0x80142534, 0, 0), loadAfterGCConsolePatches);

static void patchSMSFaderInOut(JDrama::TRect *rect, JUtility::TColor color) {
  rect->mX1 -= 1;
  rect->mX2 += 1;
  GXSetScissor(rect->mX1, rect->mY1, rect->mX2, rect->mY2);
  GXSetViewport(rect->mX1, rect->mY1, rect->mX2, rect->mY2, 0.0f, 1.0f);
  fill_rect__9(rect, color);
}
SME_PATCH_BL(SME_PORT_REGION(0x8013FDAC, 0x80134928, 0, 0), patchSMSFaderInOut);

static void patchLevelSelectPosition(J2DScreen *screen, int x, int y,
                                     J2DGrafContext *context) {
  reinterpret_cast<J2DPane *>(screen->mChildrenList.mFirst->mItemPtr)
      ->mRect.move(getScreenTransX(), 0);
  screen->draw(x, y, context);
}
SME_PATCH_BL(SME_PORT_REGION(0x8013F430, 0x80133fac, 0, 0), patchLevelSelectPosition);

static SME_PURE_ASM void patchGXScissor() {
  // clang-format off
  // todo: account for screen ratio shit
    asm volatile (
        SME_PORT_REGION (
            "lwz       7, -0x72F8(13)   \n\t",
            "lwz       7, -0x7338(13)   \n\t",
            "lwz       7, -0x72F8(13)   \n\t",
            "lwz       7, -0x72F8(13)   \n\t"
        )
        "lwz       0, 0x1E8(7)        \n\t"
        "rlwinm    12,0,4,18,27       \n\t"
        "cmpwi     3, 0               \n\t"
        "beq-      .gx_loc_0x40       \n\t"
        "add       0, 3, 5            \n\t"
        "cmpw      0, 12              \n\t"
        "beq-      .gx_loc_0x40       \n\t"
        "rlwinm    0,12,31,1,31       \n\t"
        "sub       3, 3, 0            \n\t"
        "mulli     3, 3, 0x3          \n\t"
        "mulli     5, 5, 0x3          \n\t"
        "srawi     3, 3, 0x2          \n\t"
        "rlwinm    5,5,30,2,31        \n\t"
        "addze     3, 3               \n\t"
        "add       3, 3, 0            \n\t"
        ".gx_loc_0x40:                \n\t"
        "lis 12, GXSetScissor@h       \n\t"
        "ori 12, 12, GXSetScissor@l + 4       \n\t"
        "mtctr 12       \n\t"
        "bctr       \n\t"
    );
  // clang-format on
}
SME_PATCH_B(SME_PORT_REGION(0x80363138, 0x8035b358, 0, 0), patchGXScissor);

#endif

#endif