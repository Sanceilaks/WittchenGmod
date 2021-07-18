#include "settings.h"
#include "../globals.h"

using namespace settings;

std::unordered_map<std::string, bool> states {
	{"bhop", false},
	{"aimbot", false},
	{"aimbot_autofire", false},
	{"norecoil", false},
	{"nospread", false},
	{"aimbot_fov_draw", false},
	{"aimbot_silent", false}
};

std::unordered_map<std::string, int> int_values{
	{"aimbot_fov", 0},
	{"aimbot_bones", (int)e_bones::head},
	{"custom_viewmodel_fov", 0},
	{"custom_fov", 0},
	{"custom_aspect_ratio", 0},
};

/*std::unordered_map<std::string, float> float_values{
	
};*/

template <typename t>
t& get_var(var_id_t_non_copy name) {
	if (states.find(name) != states.end() && typeid(t) == typeid(bool))
		return (t&)states[name];
	if (int_values.find(name) != int_values.end() && (typeid(t) == typeid(int) || typeid(t) == typeid(unsigned int)) ){
		return (t&)int_values[name];
	}
	/*if (float_values.find(name) != float_values.end()) {
		return (t&)float_values[name];
	}*/
	
	throw std::exception(("Cannot find variable " + name).c_str());
}

bool var_exist(var_id_t_non_copy name) {
	if (states.find(name) != states.end() ||
		int_values.find(name) != int_values.end() /*||
		float_values.find(name) != float_values.end()*/) {
		return true;
	}
	return false;
}

bool& settings::get_bool(var_id_t_non_copy name) {
	return get_var<bool>(name);
}

float& settings::get_float(var_id_t_non_copy name) {
	return get_var<float>(name);
}

int& settings::get_int(var_id_t_non_copy name) {
	return get_var<int>(name);
}

void settings::set_bool(var_id_t_non_copy n, bool val) {
	states[n] = val;
}

void settings::set_float(var_id_t_non_copy n, float val) {
	//float_values[n] = val;
	int_values[n] = (int)val;
}

void settings::set_int(var_id_t_non_copy n, int val) {
	int_values[n] = val;
}

bool settings::is_bool(var_id_t_non_copy s) {
	return states.find(s) != states.end();
}

bool settings::is_int(var_id_t_non_copy s) {
	return int_values.find(s) != int_values.end();
}

bool settings::is_float(var_id_t_non_copy s) {
	//return float_values.find(s) != float_values.end();
	return false;
}
