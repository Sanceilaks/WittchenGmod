#pragma once

#define FCVAR_ARCHIVE 128
#define FCVAR_ARCHIVE_XBOX 16777216
#define FCVAR_CHEAT 16384
#define FCVAR_CLIENTCMD_CAN_EXECUTE 1073741824
#define FCVAR_CLIENTDLL 8
#define FCVAR_DEMO 65536
#define FCVAR_DONTRECORD 131072
#define FCVAR_GAMEDLL 4
#define FCVAR_LUA_CLIENT 262144
#define FCVAR_LUA_SERVER 524288
#define FCVAR_NEVER_AS_STRING 4096
#define FCVAR_NONE 0
#define FCVAR_NOTIFY 256
#define FCVAR_NOT_CONNECTED 4194304
#define FCVAR_PRINTABLEONLY 1024
#define FCVAR_PROTECTED 32
#define FCVAR_REPLICATED 8192
#define FCVAR_SERVER_CANNOT_QUERY 536870912
#define FCVAR_SERVER_CAN_EXECUTE 268435456
#define FCVAR_SPONLY 64
#define FCVAR_UNLOGGED 2048
#define FCVAR_UNREGISTERED 1
#define FCVAR_USERINFO 512
#include <cstdint>
#include <color.h>

void dummyCallback(void* convar, const char* pOldValString) { return; }

// Auto reconstructed from vtable block @ 0x00082C0C
// from "lua_shared.dylib", by ida_vtables.idc
// Modified VTable dumper script obviously by t.me/Gaztoof.
class ConVar
{
	// https://i.imgur.com/uisym0i.png
	// https://i.imgur.com/f2HoNEu.png
public:
	ConVar* next_convar; //0x0008
	uint32_t Registered; //0x0010
	char pad_0014[4]; //0x0014
	char* name; //0x0018
	char* help_string; //0x0020
	uint32_t flags; //0x0028
	char pad_002C[12]; //0x002C
	ConVar* parent; //0x0038
	char* default_value; //0x0040
	char* value_str; //0x0048
	uint32_t str_length; //0x0050
	float str_val; //0x0054
	int32_t int_value; //0x0058
	uint32_t has_min; //0x005C
	float min_val; //0x0060
	uint32_t has_max; //0x0064
	char pad_0068[8]; //0x0068
	void* callbackptr; //0x0070


	//Don't forget the constructor.
	/*0*/	virtual void* destr() = 0;
	/*2*/	virtual bool is_command(void)const = 0;
	/*3*/	virtual bool is_flag_set(int)const = 0;
	/*4*/	virtual void add_flags(int) = 0;
	/*4*/	virtual void remove_flags(int) = 0;

	/*4*/	virtual uint64_t get_flags() = 0;

	/*7*/	virtual const char* get_name(void)const = 0;
	/*8*/	virtual const char* get_help_text(void)const = 0;
	/*9*/	virtual bool is_registered(void)const = 0;
	/*10*/	virtual void* get_dll_identifier(void)const = 0;
	/*11*/	virtual void create_base(char const*, char const*, int) = 0;
	/*11*/	virtual void init(void) = 0;

	/*11*/	virtual void unk1(void) = 0;
	/*11*/	virtual void unk2(void) = 0;
	///*12*/	virtual void SetValue(char const*) = 0;
	///*13*/	virtual void SetValue(float) = 0;
	///*14*/	virtual void SetValue(int) = 0;
	/*15*/	virtual void InternalSetValue(char const*) = 0; // This doesn't works for me, for some reasons
	/*16*/	virtual void InternalSetValue(float) = 0;
	/*17*/	virtual void InternalSetValue(int) = 0;
	/*18*/	virtual void SetValue(int) = 0;
	/*19*/	virtual void* ClampValue(float&) = 0;
	/*20*/	virtual void* ChangeStringValue(char const*, float) = 0;
	/*21*/	virtual void* Create(char const*, char const*, int, char const*, bool, float, bool, float, void (*)(void*, char const*, float)) = 0;

	void DisableCallback() {
		if (this->callbackptr)
			*(void**)(this->callbackptr) = dummyCallback;
	}
};

typedef void* CVarDLLIdentifier_t;
typedef void* ConCommandBase;
typedef void* ConCommand;
typedef void* FnChangeCallback_t;

class CCvar
{
public:
	virtual void connect(void* (*)(char const*, int*)) = 0;
	virtual void disconnect(void) = 0;
	virtual void* query_interface(char const*) = 0;
	virtual void* init(void) = 0;
	virtual void shutdown(void) = 0;

	virtual void* nothing1(void) = 0;
	virtual void* nothing2(void) = 0;
	virtual void* nothing3(void) = 0;
	virtual void* nothing4(void) = 0;

	virtual CVarDLLIdentifier_t        allocate_dll_identifier() = 0; // 9
	virtual void                       register_con_command(ConCommandBase* pCommandBase) = 0; //10
	virtual void                       unregister_con_command(ConCommandBase* pCommandBase) = 0;
	virtual void                       unregister_con_commands(CVarDLLIdentifier_t id) = 0;
	virtual const char* get_command_line_value(const char* pVariableName) = 0;
	virtual ConCommandBase* find_command_base(const char* name) = 0;
	virtual const ConCommandBase* find_command_base(const char* name) const = 0;
	virtual ConVar* find(const char* var_name) = 0; //16
	virtual const ConVar* find(const char* var_name) const = 0;
	virtual ConCommand* find_command(const char* name) = 0;
	virtual const ConCommand* find_command(const char* name) const = 0;
	virtual void                       install_global_change_callback(FnChangeCallback_t callback) = 0;
	virtual void                       remove_global_change_callback(FnChangeCallback_t callback) = 0;
	virtual void                       call_callbacks(ConVar* var, const char* pOldString, float flOldValue) = 0;
	virtual void                       install_console_display_func(IConsoleDisplayFunc* pDisplayFunc) = 0;
	virtual void                       remove_console_display_func(IConsoleDisplayFunc* pDisplayFunc) = 0;
	virtual void                       console_color_printf(const Color& clr, const char* pFormat, ...) const = 0;
	virtual void                       console_printf(const char* pFormat, ...) const = 0;
	virtual void                       console_DPrintf(const char* pFormat, ...) const = 0;
	virtual void                       revert_flagged_con_vars(int nFlag) = 0;
};