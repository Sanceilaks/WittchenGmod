#pragma once

namespace hooks_manager
{
	void Init();
	void Shutdown();

	void CreateHook(void* target, void* detour, void** original);
}