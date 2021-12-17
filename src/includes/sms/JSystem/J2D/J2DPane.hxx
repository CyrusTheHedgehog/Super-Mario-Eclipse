#pragma once

#include "JSU/JSUInputStream.hxx"
#include "JSU/JSUList.hxx"
#include "JUT/JUTColor.hxx"
#include "JUT/JUTRect.hxx"
#include "GX.h"
#include "MTX.h"
#include "types.h"


enum J2DBasePosition { UNK_BASEPOS_4 = 4, UNK_BASEPOS_5 = 5 };

class J2DGrafContext {
public:
  J2DGrafContext(int, int, int, int);
  J2DGrafContext(const JUTRect &);
  ~J2DGrafContext();

  void place(const JUTRect &);
  void place(int, int, int, int);
  void setPort();
  void setup2D();
  void setScissor();
  void setLookat();

  void scissor(const JUTRect &);
  void fillBox(const JUTRect &);

  u32 *vTable; // _0
  u32 _4;      // padding?
  JUTRect _8;
  JUTRect _18;
  JUtility::TColor _28;
  JUtility::TColor _2C;
  JUtility::TColor _30;
  JUtility::TColor _34;
  u8 mLineWidth; // _38
  u8 _39;        // padding?
  u8 _3A;        // ^^
  u8 _3B;        // ^^
  u32 _3C;
  u32 _40;
  u8 _44[0xB4 - 0x44]; // who knows what's here
  u32 _B4;
  u32 _B8;
  u32 _BC;
  u32 _C0;
  u32 _C4;
  u32 _C8;
  u32 _CC;
  u32 _D0;
  u32 _D4;
};

class J2DPane {
public:
  J2DPane();
  J2DPane(J2DPane *, s16, bool, u32, const JUTRect &);
  J2DPane(s16, u32, const JUTRect &);
  J2DPane(J2DPane *, JSURandomInputStream *, bool);
  virtual ~J2DPane();

  virtual void drawSelf(int, int, Mtx *);
  virtual bool setConnectParent(bool);
  virtual void drawSelf(int, int);
  virtual void resize(int, int);
  virtual void move(int, int);
  virtual void add(int, int);
  virtual J2DPane *search(u32 id);
  virtual void makeMatrix(int, int);

  void draw(int, int, const J2DGrafContext *, bool);
  void clip(const JUTRect &);
  void setCullBack(GXCullback cullback);
  void setBasePosition(J2DBasePosition basePos);

  u16 _4;
  u16 _6;          // padding?
  u32 mID;         // _8
  bool mIsVisible; // _C
  u8 _D;           // padding?
  u8 _E;           // ^^
  u8 _F;           // ^^
  u32 _10;
  JUTRect mRect;         // _14
  JUTRect mCRect;        // _24
  JUTRect mClipRect;     // _34
  JUTRect mScissorBound; // _44
  Mtx _54;
  u8 _84[0xB4 - 0x84];
  u32 _B4;
  u32 _B8;
  u8 _BC;
  f32 mRotation;                 // _C0
  J2DBasePosition mBasePosition; // _C4
  GXCullback mCullingMode;       // _C8
  u8 mAlpha;
  u8 mAlphaCopy;
  u8 _CE;
  u8 _CF; // connectParent
  JSUPtrList mChildrenList;
  JSUPtrLink mPtrLink;
};