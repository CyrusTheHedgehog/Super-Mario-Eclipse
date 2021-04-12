#pragma once

#include "types.h"
#include "sms/JSystem/JDrama.hxx"
#include "sms/JSystem/JSU/JSUMemoryStream.hxx"
#include "sms/actor/HitActor.hxx"
#include "sms/object/ObjChara.hxx"

/* Size -- 0x34 */
class TObjManager : public JDrama::TViewObj
{
	public:
	TObjManager(char const *);
	virtual ~TObjManager();
	
	virtual void load(JSUMemoryInputStream &);
	virtual TObjManager* searchF(u16, const char *);
	virtual void perform(u32, JDrama::TGraphics *);
	virtual void createModelData();
	virtual void createAnmData();

	void manageObj(THitActor *);
	
	s32 mMaxObjs; // _10
	u32 _14; // I think this is obj number
	u32* _18;
	TObjChara* _1C;
	u32 _20;
	u32 _24;
	u32 _28;
	u32 _2C;
	u32 _30;
};