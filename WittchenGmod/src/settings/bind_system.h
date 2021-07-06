#pragma once

#include <cstdint>

namespace bind_system {
	enum class bind_type : int {
		none,
		pressed,
		toggle,
		active
	};

	template <typename t>
	struct bind_t {
		bind_type type = bind_type::none;
		int key = 0;
		t* value = nullptr;
		t bind_value = 0;
	};

	using float_bind = bind_t<float>;
	using int_bind = bind_t<int>;
	using uint_bind = bind_t<uint32_t>;
	using bool_bind = bind_t<bool>;
	using flags_bind_t = bind_t<int>;

	
}
