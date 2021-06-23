#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <Windows.h>


namespace MemoryUtils
{
	template<typename T>
	T* CaptureInterface(const std::string& module_name, const std::string& interface_name)
	{
		typedef void* (*interface_type)(const char* name, int ret);
		const auto temp = reinterpret_cast<interface_type>(GetProcAddress(GetModuleHandle(module_name.c_str()), "CreateInterface"));
		return static_cast<T*>(temp(interface_name.c_str(), 0));
	}
	
	template <typename T>
	static constexpr auto RelativeToAbsolute(uintptr_t address) noexcept
	{
		return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
	}
	
	std::uint8_t* PatternScanner(const std::string& module_name, const std::string& signature) noexcept;
}