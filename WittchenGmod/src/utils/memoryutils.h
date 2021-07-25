#pragma once

#if not defined(_WIN32)
#error memoryutils works only on windows
#endif

/* CONFIG SECTION */
#define VALVE_GAME //u can undef this, if you target not based on source engine
#define MEMORYUTILS_NAMESPACE_NAME memory_utils
#define READMEMORY(process, address, size, bytes_was_read) ReadProcessMemory((HANDLE)process, (LPCVOID)address, (LPVOID)buffer, (SIZE_T)size, &bytes_was_read)
#define WRITEMEMORY(process, address, data, bytes_was_write) WriteProcessMemory((HANDLE)process, (LPCVOID)address, (LPVOID)buffer, (SIZE_T)size, &bytes_was_write)

/* INCLUDE SECTION */
#include <string>
#include <vector>
#include <Windows.h>

/* DEFINES */
#define PAGE_EXECUTE_FLAGS (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)

namespace MEMORYUTILS_NAMESPACE_NAME
{
	using address_t = uintptr_t;
	
#if defined(VALVE_GAME)
	template<typename T>
	T* capture_interface(const std::string& module_name, const std::string& interface_name)
	{
		typedef void* (*interface_type)(const char* name, int ret);
		const auto temp = reinterpret_cast<interface_type>(GetProcAddress(GetModuleHandle(module_name.c_str()), "CreateInterface"));
		return static_cast<T*>(temp(interface_name.c_str(), 0));
	}
#endif
	
#if defined(_WIN64)
	template <typename T> static constexpr auto relative_to_absolute(address_t address) noexcept {
		return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
	}

	inline auto relative_to_absolute(address_t address, int offset, int instruction_size = 6) noexcept {
		auto instruction = address + offset;

		int relativeAddress = *(int*)(instruction);
		auto realAddress = address + instruction_size + relativeAddress;
		return realAddress;
	}
	
	template<typename T>
	T* get_vmt_from_instruction(address_t address) noexcept {
		size_t step = 3;
		size_t instructionSize = 7;
		address_t instruction = address;

		address_t relativeAddress = *(DWORD*)(instruction + step);
		address_t realAddress = instruction + instructionSize + relativeAddress;
		return *(T**)(realAddress);
	}

	template<typename T>
	T* get_vmt_from_instruction(address_t address, size_t offset) noexcept {
		size_t step = 3;
		size_t instructionSize = 7;
		uintptr_t instruction = address + offset;

		return *(T**)(relative_to_absolute(instruction, step, instructionSize));
	}

	template<typename T>
	T* get_vmt_from_vtable_function(address_t from_vtble, int index, address_t offset_from_funcrion_start, uint32_t instruction_offset = 3, uint32_t
	                                instruction_size = 7) noexcept
	{
		uintptr_t instruction = ((*(uintptr_t**)(from_vtble))[index] + offset_from_funcrion_start);
		return *(T**)(relative_to_absolute(instruction, instruction_offset, instruction_size));
	}
#endif
	
	inline std::uint8_t* pattern_scanner(const std::string& module_name, const std::string& signature) noexcept {
		auto* const module_handle = GetModuleHandleA(module_name.c_str());

		if (!module_handle)
			return nullptr;

		static auto pattern_to_byte = [](const std::string& pattern) {
			auto bytes = std::vector<int>();
			auto* const start = const_cast<char*>(pattern.c_str());
			auto* const end = const_cast<char*>(pattern.c_str()) + std::strlen(pattern.c_str());

			for (auto* current = start; current < end; ++current) {
				if (*current == '?') {
					++current;

					if (*current == '?')
						++current;

					bytes.push_back(-1);
				}
				else {
					bytes.push_back(std::strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		auto* const dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
		auto* const nt_headers =
			reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

		const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
		auto pattern_bytes = pattern_to_byte(signature);
		auto* const scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

		const auto s = pattern_bytes.size();
		auto* const d = pattern_bytes.data();

		for (auto i = 0ul; i < size_of_image - s; ++i) {
			auto found = true;

			for (auto j = 0ul; j < s; ++j) {
				if (scan_bytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found)
				return &scan_bytes[i];
		}

		return nullptr;
	}

	typedef MEMORY_BASIC_INFORMATION memory_basic_information_t;
	
	inline memory_basic_information_t get_memory_basic_information(address_t address) {
		memory_basic_information_t i; VirtualQuery((LPVOID)address, &i, sizeof(i));
		return i;
	}
	
	inline bool address_is_executable(address_t address) {
		return get_memory_basic_information(address).Protect & PAGE_EXECUTE_FLAGS
		&& get_memory_basic_information(address).State == MEM_COMMIT;
	}

	template <typename fn>
	fn get_virtual_fn(PVOID** c, int idx) {
		return (fn)(*c)[idx];
	}
}