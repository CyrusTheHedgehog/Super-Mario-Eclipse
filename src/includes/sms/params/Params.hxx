#pragma once

#include "types.h"
#include "sms/JSystem/JDrama.hxx"
#include "sms/JSystem/JKR/JKRMemArchive.hxx"
#include "sms/JSystem/JSU/JSUMemoryStream.hxx"
#include "BaseParam.hxx"

class TParams
{
public:
    void finalize();
    void init();
    void load(const char *);
    void load(JSUMemoryInputStream &);

    u32 _00;
    TBaseParam *mBaseParam;

    static JKRMemArchive *mArc;
    static JKRMemArchive *mSceneArc;
};

template <typename T>
class TParamT : public TBaseParam
{
    T mValue;
public:
    TParamT(TParams *parent, T param, u16 keycode, const char *name) : mValue(param) {}

    T get() const { return mValue; }
    void set(T param) { mValue = param; }

    void load(JSUMemoryInputStream &) override;
};

template <typename T>
class TParamRT : public TParamT<T>
{
public:
    TParamRT(TParams *parent, T param, u16 keycode, const char *name) : TParamT<T>(parent, param, keycode, name) {}

    void set(T param) {};
};