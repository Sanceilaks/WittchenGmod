#pragma once

class CViewSetup;

class CHLClient {
public:
	virtual void Init(void) = 0;
	virtual void PostInit(void) = 0;
	virtual void Shutdown(void) = 0;
	virtual void unk1(void) = 0;
	virtual void ReplyPostInit(void) = 0;
	virtual void LevelInitPreEntity() = 0;
	virtual void LevelInitPostEntity() = 0;
	virtual void LevelShutdown() = 0;
	virtual void* GetAllClasses() = 0;

	bool GetPlayerView(CViewSetup* setup) {
		using fn = bool(__fastcall*)(void*, CViewSetup* out);
		return (*(fn**)this)[59](this, setup);
	}
};
