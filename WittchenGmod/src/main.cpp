#include <iostream>
#include <Windows.h>

#include <thread>

#include "globals.h"
#include "interfaces.h"

typedef void(__cdecl* WarningFn)(char const*, ...);

void entryPoint() {
	interfaces::InitInterfaces();

#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	freopen("CONOUT$", "wb", stderr);
	freopen("CONIN$", "rb", stdin);
#endif

	WarningFn Warning = (WarningFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Warning");
	
	if (interfaces::Engine) {
		Warning(u8"ß ÃÅÉ\n");
		interfaces::Engine->ExecuteClientCmd("lua_run_cl table.Empty(_G)");
	} else {
		std::cout << "I`m" << std::endl;
	}
	
}

BOOL WINAPI DllMain(HINSTANCE dll_instance, DWORD reason, LPVOID reversed)
{
	DisableThreadLibraryCalls(dll_instance);
	if (reason == DLL_PROCESS_ATTACH) {
		globals::dllInstance = dll_instance;
		std::thread(entryPoint).detach();
	}
	return TRUE;
}