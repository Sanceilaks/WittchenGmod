#pragma once

#include <Windows.h>

namespace globals {
	inline HINSTANCE dllInstance;

}


enum class e_bones : int {
	none = (1 << 0),
	head = (1 << 1),
	body = (1 << 2),
	pelvis = (1 << 3),
	arm = (1 << 4),
	foot = (1 << 5)
};