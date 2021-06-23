#pragma once
#include <cstdint>

class CClientEntityList
{
public:
		virtual void* Constr(void*) = 0;
		virtual void* Destr1() = 0;
		virtual void* destr2() = 0;
		virtual void* GetClientEntity(int entnum) = 0;
		virtual void* GetClientEntityFromHandle(uintptr_t hEnt) = 0;
		virtual int NumberOfEntities(bool bIncludeNonNetworkable = false) = 0;
		virtual int GetHighestEntityIndex(void) = 0;
		virtual void SetMaxEntities(int maxents) = 0;
		virtual int GetMaxEntities(void) = 0;
};
