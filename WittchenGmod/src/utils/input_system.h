#pragma once
#include <string>

//TODO: Optimization

namespace input_system {
	enum class bind_type : uint8_t {
		pressed,
		just_pressed
	};
	
	void initialize_input_system();

	void add_bind(int key, const std::string& name, bool* var, bind_type bt = bind_type::pressed);
	void set_bind_key(const std::string& name, int key);
	void remove_bind(const std::string& name);
	
	bool is_key_pressed(int key);
	bool is_key_just_pressed(int key);

	void process_binds();
}
