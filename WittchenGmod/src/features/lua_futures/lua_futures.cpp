#include "lua_futures.h"

#include <mutex>
#include <vector>

#include "../../interfaces.h"

std::mutex lua_run_mutex;
std::vector<std::string> lua_to_run;

void lua_futures::add_code_to_run(const std::string& str) {
	std::unique_lock l(lua_run_mutex);
	lua_to_run.push_back(str);
}

void lua_futures::run_all_code() {
	std::unique_lock l(lua_run_mutex);

	if (lua_to_run.empty())
		return;

	auto intr = interfaces::lua_shared->get_lua_interface(0);
	if (!intr)
		return;
	
	for (const auto& i : lua_to_run) {
		try {
			intr->run_string(last_file_name.c_str(), "", i.c_str(), true, true);
		}
		catch (...) {
		}
	}

	lua_to_run.clear();
}

std::string lua_futures::bypass = u8R"(

local _string_sub = string.sub
local _istable = istable
local _pairs = pairs
local _tostring = tostring
local _string_byte = string.byte
local _debug_getinfo = debug.getinfo
local _debug_getregistry = debug.getregistry
local funcs = {}

local funcsBlacklist = {}
funcsBlacklist[_tostring] = true
funcsBlacklist[_string_byte] = true
funcsBlacklist[_string_sub] = true

local tablesBlacklist = {}
tablesBlacklist[package] = true
tablesBlacklist[math] = true
tablesBlacklist[bit] = true
tablesBlacklist[chat] = true
tablesBlacklist[mesh] = true

local function iter(t, name, done)
    done = done or {}
    done[t] = true

    for k, v in _pairs(t) do
        local newname = name .. '.' .. _tostring(k)

        if (_istable(v) and not done[v] and not tablesBlacklist[v]) then
            iter(v, newname, done)
        elseif (_isfunction(v)) then
            local info = _debug_getinfo(v)
            if (info and info.short_src == "[C]" and not funcsBlacklist[v] and not funcs[v]) then
                funcs[v] = newname
            end
        end
    end
end

local _R = _debug_getregistry()

iter(_G, "_G")

iter(_R.CMoveData, "_R.CMoveData")
iter(_R.CUserCmd, "_R.CUserCmd")
iter(_R.Entity, "_R.Entity")
iter(_R.Player, "_R.Player")
iter(_R.File, "_R.File")
iter(_R.ConVar, "_R.ConVar")

)";