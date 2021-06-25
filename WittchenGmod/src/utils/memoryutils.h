#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <Windows.h>


namespace memory_utils
{
	template<typename T>
	T* capture_interface(const std::string& module_name, const std::string& interface_name)
	{
		typedef void* (*interface_type)(const char* name, int ret);
		const auto temp = reinterpret_cast<interface_type>(GetProcAddress(GetModuleHandle(module_name.c_str()), "CreateInterface"));
		return static_cast<T*>(temp(interface_name.c_str(), 0));
	}
	
	template <typename T>
	static constexpr auto relative_to_absolute(uintptr_t address) noexcept
	{
		return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
	}

	template<typename T>
	T* get_vmt_from_instruction(uintptr_t address)
	{
		uintptr_t step = 3;
		uintptr_t instructionSize = 7;
		uintptr_t instruction = address;

		uintptr_t relativeAddress = *(DWORD*)(instruction + step);
		uintptr_t realAddress = instruction + instructionSize + relativeAddress;
		return *(T**)(realAddress);
	}

	template<typename T>
	T* get_vmt_from_instruction(uintptr_t address, size_t offset)
	{
		uintptr_t step = 3;
		uintptr_t instructionSize = 7;
		uintptr_t instruction = address + offset;

		uintptr_t relativeAddress = *(DWORD*)(instruction + step);
		uintptr_t realAddress = instruction + instructionSize + relativeAddress;
		return *(T**)(realAddress);
	}

	template<typename T>
	T* get_vmt(uintptr_t address, int index, uintptr_t offset) // Address must be a VTable pointer, not a VTable !
	{
		uintptr_t step = 3;
		uintptr_t instructionSize = 7;
		uintptr_t instruction = ((*(uintptr_t**)(address))[index] + offset);

		uintptr_t relativeAddress = *(DWORD*)(instruction + step);
		uintptr_t realAddress = instruction + instructionSize + relativeAddress;
		return *(T**)(realAddress);
	}
	
	std::uint8_t* pattern_scanner(const std::string& module_name, const std::string& signature) noexcept;
}