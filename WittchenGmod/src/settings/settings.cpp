#include "settings.h"

using namespace settings;

std::map<std::string, bool> states{
	{"bhop", false},
};

std::map<std::string, int> int_values{

};

std::map<std::string, float> float_values{

};

template <typename t>
t& get_var(var_id_t_non_copy name) {
	if (states.find(name) != states.end())
		return (t&)states[name];
	if (int_values.find(name) != int_values.end()) {
		return (t&)int_values[name];
	}
	if (float_values.find(name) != float_values.end()) {
		return (t&)float_values[name];
	}
	throw std::exception(("Cannot find variable " + name).c_str());
}

bool var_exist(var_id_t_non_copy name) {
	if (states.find(name) != states.end() ||
		int_values.find(name) != int_values.end() ||
		float_values.find(name) != float_values.end()) {
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
	float_values[n] = val;
}

void settings::set_int(var_id_t_non_copy n, int val) {
	int_values[n] = val;
}
