#include "settings.h"

#include <any>
#include <fstream>

#include "file_tools.h"
#include "json.hpp"
#include "../globals.h"

using namespace settings;

constexpr int bool_idx = 0;
constexpr int int_idx = 1;

internal::settings_storage_type settings_storage{
	{"bhop", false},
	{"aimbot", false},
	{"aimbot_autofire", false},
	{"norecoil", false},
	{"nospread", false},
	{"aimbot_fov_draw", false},
	{"aimbot_silent", false},
	{"aimbot_draw_target", false},
	{"fixmovement", false},
	{"third_person", false},
	{"faf", false},
	{"fake_lags", false},
	{"fake_duck", false},
	
	{"aimbot_fov", 0},
	{"aimbot_bones", (int)e_bones::head},
	{"custom_viewmodel_fov", 0},
	{"custom_fov", 0},
	{"custom_aspect_ratio", 0},
	{"third_person_distance", 50},
	{"fake_lags_amount", 0}
};

internal::settings_storage_type& internal::get_settings_storage() {
	return settings_storage;
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
	settings_storage[n] = val;
}

void settings::set_float(var_id_t_non_copy n, float val) {
	settings_storage[n] = val;
}

void settings::set_int(var_id_t_non_copy n, int val) {
	settings_storage[n] = val;
}

bool settings::is_bool(var_id_t_non_copy s) {
	return is_type<bool>(s);
}

bool settings::is_int(var_id_t_non_copy s) {
	return is_type<int>(s);
}

bool settings::is_float(var_id_t_non_copy s) {
	return is_type<float>(s);
}

void settings::save_to_file(const std::filesystem::path& path) {
	nlohmann::json json;

	for (auto& i : settings_storage) {
		auto get_value = [&]() -> nlohmann::json {
			const auto& t = i.second.type();
			if (t == typeid(bool)) return std::any_cast<bool>(i.second);
			else if (t == typeid(float)) return std::any_cast<float>(i.second);
			else if (t == typeid(int)) return std::any_cast<int>(i.second);
			else if (t == typeid(uint32_t)) return std::any_cast<uint32_t>(i.second);
			else if (t == typeid(std::string)) return std::any_cast<std::string>(i.second);
			return 0;
		};

		json["config"][i.first] = get_value();
	}

	//globals::on_save(json);

	std::ofstream s(path.generic_u8string());
	s << json.dump() << std::endl;
}

void settings::load_from_file(const std::filesystem::path& path) {
	std::string file_content; file_tools::read_file(file_content, path.generic_u8string());
	auto json = nlohmann::json::parse(file_content)["config"];

	if (json.is_null())
		MessageBox(0, "It config too old", "OTV", MB_OK);

	for (const auto& i : json.items()) {
		auto get_value = [&]() -> std::any {
			switch (i.value().type()) {
			case nlohmann::detail::value_t::boolean: return i.value().get<bool>(); break;
			case nlohmann::detail::value_t::number_float: return i.value().get<float>(); break;
			case nlohmann::detail::value_t::null: break;
			case nlohmann::detail::value_t::object: break;
			case nlohmann::detail::value_t::array: break;
			case nlohmann::detail::value_t::string: return i.value().get<std::string>(); break;
			case nlohmann::detail::value_t::number_integer: return i.value().get<int>(); break;
			case nlohmann::detail::value_t::number_unsigned: return i.value().get<int>(); break;
			case nlohmann::detail::value_t::binary: break;
			case nlohmann::detail::value_t::discarded: break;
			default: break;
			}
			return 0;
		};

		settings_storage[i.key()] = get_value();
	}

	///globals::on_load(nlohmann::json::parse(file_content));
}

bool var_exist(var_id_t_non_copy name) {
	return settings_storage.find(name) != settings_storage.end();
}
