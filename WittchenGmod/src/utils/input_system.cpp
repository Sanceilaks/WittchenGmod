#include "input_system.h"

#include <imgui/imgui.h>

#include <map>
#include <string>
#include <vector>

struct bool_bind_t {
	int key;
	bool* bind_var;
	input_system::bind_type bind_type;
};

std::map<std::string, bool_bind_t> key_binds;

auto find_bind(const std::string& name) {
	return std::find_if(key_binds.begin(), key_binds.end(), [name](const auto& it) {
		return it.first == name;
		});
}

bool bind_exist(const std::string& name) {
	return find_bind(name) != key_binds.end();
}

void input_system::initialize_input_system() {
	
}

void input_system::add_bind(int key, const std::string& name, bool* var, bind_type bt) {
	if (var && !bind_exist(name))
		key_binds.emplace(name, bool_bind_t{ key, var, bt });
}

void input_system::set_bind_key(const std::string& name, int key) {
	if (bind_exist(name)) {
		find_bind(name)->second.key = key;
	}
}

void input_system::remove_bind(const std::string& name) {
	if (bind_exist(name)) {
		key_binds.erase(find_bind( name));
	}
}

bool input_system::is_key_pressed(int key) {
	return ImGui::IsKeyDown(key);
}

bool input_system::is_key_just_pressed(int key) {
	return ImGui::IsKeyPressed(key);
}

void input_system::process_binds() {
	for (auto i : key_binds) {
		switch (i.second.bind_type) {
			case bind_type::pressed: {
				*i.second.bind_var = is_key_pressed(i.second.key);
				break;
			}
			case bind_type::just_pressed: {
				*i.second.bind_var = is_key_just_pressed(i.second.key);
				break;
			}
		}
	}
}
