#include "common.hxx"
#include "JDrama/JDRActor.hxx"
#include "JGeometry.hxx"

using namespace SME::Util;

void Spc::setActorPosToOther(TSpcInterp *interp, u32 argc) {
  interp->verifyArgNum(2, &argc);
  JDrama::TActor *target =
      reinterpret_cast<JDrama::TActor *>(Spc::Stack::popItem(interp).mValue);
  JDrama::TActor *self =
      reinterpret_cast<JDrama::TActor *>(Spc::Stack::popItem(interp).mValue);
  JGeometry::TVec3<f32> pos;
  target->JSGGetTranslation(reinterpret_cast<Vec *>(&pos));
  self->JSGSetTranslation(reinterpret_cast<Vec &>(pos));
}

void Spc::setActorRotToOther(TSpcInterp *interp, u32 argc) {
  interp->verifyArgNum(2, &argc);
  JDrama::TActor *target =
      reinterpret_cast<JDrama::TActor *>(Spc::Stack::popItem(interp).mValue);
  JDrama::TActor *self =
      reinterpret_cast<JDrama::TActor *>(Spc::Stack::popItem(interp).mValue);
  JGeometry::TVec3<f32> rot;
  target->JSGGetRotation(reinterpret_cast<Vec *>(&rot));
  self->JSGSetRotation(reinterpret_cast<Vec &>(rot));
}

void Spc::getActorPos(TSpcInterp *interp, u32 argc) {
  interp->verifyArgNum(1, &argc);
  JDrama::TActor *self =
      reinterpret_cast<JDrama::TActor *>(Spc::Stack::popItem(interp).mValue);
  JGeometry::TVec3<f32> *pos = new JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
  self->JSGGetTranslation(reinterpret_cast<Vec *>(pos));
  Spc::Stack::pushItem(interp, reinterpret_cast<u32>(pos), Spc::ValueType::INT); // Return a value
}

void Spc::setActorPos(TSpcInterp *interp, u32 argc) {
  interp->verifyArgNum(2, &argc);
  JGeometry::TVec3<f32> *pos =
      reinterpret_cast<JGeometry::TVec3<f32> *>(Spc::Stack::popItem(interp).mValue);
  JDrama::TActor *self =
      reinterpret_cast<JDrama::TActor *>(Spc::Stack::popItem(interp).mValue);
  self->JSGSetTranslation(reinterpret_cast<Vec &>(*pos));
}

void Spc::setActorPosf(TSpcInterp *interp, u32 argc) {
  interp->verifyArgNum(4, &argc);
  f32 z = static_cast<f32>(Spc::Stack::popItem(interp).mValue);
  f32 y = static_cast<f32>(Spc::Stack::popItem(interp).mValue);
  f32 x = static_cast<f32>(Spc::Stack::popItem(interp).mValue);
  JDrama::TActor *self =
      reinterpret_cast<JDrama::TActor *>(Spc::Stack::popItem(interp).mValue);

  Vec pos {.x = x, .y = y, .z = z};
  self->JSGSetTranslation(pos);
}

void Spc::getActorRot(TSpcInterp *interp, u32 argc) {
  interp->verifyArgNum(1, &argc);
  JDrama::TActor *self =
      reinterpret_cast<JDrama::TActor *>(Spc::Stack::popItem(interp).mValue);
  JGeometry::TVec3<f32> *pos = new JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
  self->JSGGetRotation(reinterpret_cast<Vec *>(pos));
  Spc::Stack::pushItem(interp, reinterpret_cast<u32>(pos), Spc::ValueType::INT); // Return a value
}

void Spc::setActorRot(TSpcInterp *interp, u32 argc) {
  interp->verifyArgNum(2, &argc);
  JGeometry::TVec3<f32> *rot =
      reinterpret_cast<JGeometry::TVec3<f32> *>(Spc::Stack::popItem(interp).mValue);
  JDrama::TActor *self =
      reinterpret_cast<JDrama::TActor *>(Spc::Stack::popItem(interp).mValue);
  self->JSGSetRotation(reinterpret_cast<Vec &>(*rot));
}

void Spc::setActorRotf(TSpcInterp *interp, u32 argc) {
  interp->verifyArgNum(4, &argc);
  f32 z = static_cast<f32>(Spc::Stack::popItem(interp).mValue);
  f32 y = static_cast<f32>(Spc::Stack::popItem(interp).mValue);
  f32 x = static_cast<f32>(Spc::Stack::popItem(interp).mValue);
  JDrama::TActor *self =
      reinterpret_cast<JDrama::TActor *>(Spc::Stack::popItem(interp).mValue);

  Vec rot {.x = x, .y = y, .z = z};
  self->JSGSetRotation(reinterpret_cast<Vec &>(rot));
}