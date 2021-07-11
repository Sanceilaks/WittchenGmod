#pragma once
#include <cstdint>
#include "../../math/math.h"
#include <d3d9.h>

class ivtf_texture;
class i_texture;
class rect_t;
class i_material;
class key_values;

enum image_format
{
	image_format_unknown = -1,
	image_format_rgba8888 = 0,
	image_format_abgr8888,
	image_format_rgb888,
	image_format_bgr888,
	image_format_rgb565,
	image_format_i8,
	image_format_ia88,
	image_format_p8,
	image_format_a8,
	image_format_rgb888_bluescreen,
	image_format_bgr888_bluescreen,
	image_format_argb8888,
	image_format_bgra8888,
	image_format_dxt1,
	image_format_dxt3,
	image_format_dxt5,
	image_format_bgrx8888,
	image_format_bgr565,
	image_format_bgrx5551,
	image_format_bgra4444,
	image_format_dxt1_onebitalpha,
	image_format_bgra5551,
	image_format_uv88,
	image_format_uvwq8888,
	image_format_rgba16161616_f,
	image_format_rgba16161616,
	image_format_uvlx8888,
	image_format_r32_f,			// Single-channel 32-bit floating point
	image_format_rgb323232_f,
	image_format_rgba32323232_f,

	// Depth-stencil texture formats for shadow depth mapping
	image_format_nv_dst16,		// 
	image_format_nv_dst24,		//
	image_format_nv_intz,		// Vendor-specific depth-stencil texture
	image_format_nv_rawz,		// formats for shadow depth mapping 
	image_format_ati_dst16,		// 
	image_format_ati_dst24,		//
	image_format_nv_null,		// Dummy format which takes no video memory

	// Compressed normal map formats
	image_format_ati2_n,			// One-surface ATI2N / DXN format
	image_format_ati1_n,			// Two-surface ATI1N format

	num_image_formats
};

enum material_var_flags_t
{
	material_var_debug = (1 << 0),
	material_var_no_debug_override = (1 << 1),
	material_var_no_draw = (1 << 2),
	material_var_use_in_fillrate_mode = (1 << 3),
	material_var_vertexcolor = (1 << 4),
	material_var_vertexalpha = (1 << 5),
	material_var_selfillum = (1 << 6),
	material_var_additive = (1 << 7),
	material_var_alphatest = (1 << 8),
	material_var_multipass = (1 << 9),
	material_var_znearer = (1 << 10),
	material_var_model = (1 << 11),
	material_var_flat = (1 << 12),
	material_var_nocull = (1 << 13),
	material_var_nofog = (1 << 14),
	material_var_ignorez = (1 << 15),
	material_var_decal = (1 << 16),
	material_var_envmapsphere = (1 << 17),
	material_var_noalphamod = (1 << 18),
	material_var_envmapcameraspace = (1 << 19),
	material_var_basealphaenvmapmask = (1 << 20),
	material_var_translucent = (1 << 21),
	material_var_normalmapalphaenvmapmask = (1 << 22),
	material_var_needs_software_skinning = (1 << 23),
	material_var_opaquetexture = (1 << 24),
	material_var_envmapmode = (1 << 25),
	material_var_suppress_decals = (1 << 26),
	material_var_halflambert = (1 << 27),
	material_var_wireframe = (1 << 28),
	material_var_allowalphatocoverage = (1 << 29),
	material_var_ignore_alpha_modulation = (1 << 30),
};

enum MaterialVarType_t
{
	material_var_type_float = 0,
	material_var_type_string,
	material_var_type_vector,
	material_var_type_texture,
	material_var_type_int,
	material_var_type_fourcc,
	material_var_type_undefined,
	material_var_type_matrix,
	material_var_type_material,
};

using preview_image_ret_val_t = int;
using vertex_format_t = int;
using morph_format_t = int;
using material_property_types_t = int;

typedef unsigned short material_var_sym_t;

class i_material_var
{
public:
	typedef unsigned long four_cc;
public:
	// class factory methods
	/*static i_material_var* create(i_material* pMaterial, char const* pKey, D3DMATRIX const& matrix);
	static i_material_var* create(i_material* pMaterial, char const* pKey, char const* pVal);
	static i_material_var* create(i_material* pMaterial, char const* pKey, float* pVal, int numcomps);
	static i_material_var* create(i_material* pMaterial, char const* pKey, float val);
	static i_material_var* create(i_material* pMaterial, char const* pKey, int val);
	static i_material_var* create(i_material* pMaterial, char const* pKey);
	static void destroy(i_material_var* pVar);
	static material_var_sym_t	get_symbol(char const* pName);
	static material_var_sym_t	find_symbol(char const* pName);
	static bool symbol_matches(char const* pName, material_var_sym_t symbol);
	static void delete_unreferenced_textures(bool enable);*/

	virtual i_texture* get_texture_value(void) = 0;

	virtual char const* get_name(void) const = 0;
	virtual material_var_sym_t	get_name_as_symbol() const = 0;

	virtual void			set_float_value(float val) = 0;

	virtual void			set_int_value(int val) = 0;

	virtual void			set_string_value(char const* val) = 0;
	virtual char const* get_string_value(void) const = 0;

	// Use FourCC values to pass app-defined data structures between
	// the proxy and the shader. The shader should ignore the data if
	// its FourCC type not correct.	
	virtual void			set_four_cc_value(four_cc type, void* pData) = 0;
	virtual void			get_four_cc_value(four_cc* type, void** ppData) = 0;

	// Vec (dim 2-4)
	virtual void			set_vec_value(float const* val, int numcomps) = 0;
	virtual void			set_vec_value(float x, float y) = 0;
	virtual void			set_vec_value(float x, float y, float z) = 0;
	virtual void			set_vec_value(float x, float y, float z, float w) = 0;
	virtual void			get_linear_vec_value(float* val, int numcomps) const = 0;

	// revisit: is this a good interface for textures?
	virtual void			set_texture_value(i_texture*) = 0;

	virtual i_material* get_material_value(void) = 0;
	virtual void			set_material_value(i_texture*) = 0;

	virtual bool			is_defined() const = 0;
	virtual void			set_undefined() = 0;
};

class i_material
{
public:
	virtual const char* get_name() const = 0;
	virtual const char* get_texture_group_name() const = 0;
	virtual preview_image_ret_val_t get_preview_image_properties(int* width, int* height,
		image_format* image_format, bool* is_translucent) const = 0;
	virtual preview_image_ret_val_t get_preview_image(unsigned char* data,
		int width, int height,
		image_format imageFormat) const = 0;
	virtual int				get_mapping_width() = 0;
	virtual int				get_mapping_height() = 0;
	virtual int				get_num_animation_frames() = 0;
	virtual bool			in_material_page(void) = 0;
	virtual	void			get_material_offset(float* pOffset) = 0;
	virtual void			get_material_scale(float* pScale) = 0;
	virtual i_material* get_material_page(void) = 0;
	virtual i_material_var* find_var(const char* varName, bool* found, bool complain = true) = 0;
	virtual void			increment_reference_count(void) = 0;
	virtual void			decrement_reference_count(void) = 0;
	inline void add_ref() { increment_reference_count(); }
	inline void release() { decrement_reference_count(); }
	virtual int 			get_enumeration_id(void) const = 0;
	virtual void			get_low_res_color_sample(float s, float t, float* color) const = 0;
	virtual void			recompute_state_snapshots() = 0;
	virtual bool			is_translucent() = 0;
	virtual bool			is_alpha_tested() = 0;
	virtual bool			is_vertex_lit() = 0;
	virtual vertex_format_t	get_vertex_format() const = 0;
	virtual bool			has_proxy(void) const = 0;
	virtual bool			uses_env_cubemap(void) = 0;
	virtual bool			needs_tangent_space(void) = 0;
	virtual bool			needs_power_of_two_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			needs_full_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			needs_software_skinning(void) = 0;
	virtual void			alpha_modulate(float alpha) = 0;
	virtual void			color_modulate(float r, float g, float b) = 0;
	virtual void			set_material_var_flag(material_var_flags_t flag, bool on) = 0;
	virtual bool			get_material_var_flag(material_var_flags_t flag) const = 0;
	virtual void			get_reflectivity(c_vector& reflect) = 0;
	virtual bool			get_property_flag(material_property_types_t type) = 0;
	virtual bool			is_two_sided() = 0;
	virtual void			set_shader(const char* pShaderName) = 0;
	virtual int				get_num_passes(void) = 0;
	virtual int				get_texture_memory_bytes(void) = 0;
	virtual void			refresh() = 0;
	virtual bool			needs_lightmap_blend_alpha(void) = 0;
	virtual bool			needs_software_lighting(void) = 0;
	virtual int				shader_param_count() const = 0;
	virtual i_material_var** get_shader_params(void) = 0;
	virtual bool			is_error_material() const = 0;
	virtual void			set_use_fixed_function_baked_lighting(bool bEnable) = 0;
	virtual float			get_alpha_modulation() = 0;
	virtual void			get_color_modulation(float* r, float* g, float* b) = 0;
	virtual morph_format_t	get_morph_format() const = 0;
	virtual i_material_var* find_var_fast(char const* pVarName, unsigned int* pToken) = 0;
	virtual void			set_shader_and_params(key_values* pKeyValues) = 0;
	virtual const char* get_shader_name() const = 0;
	virtual void			delete_if_unreferenced() = 0;
	virtual bool			is_sprite_card() = 0;
	virtual void			call_bind_proxy(void* proxyData) = 0;
	virtual i_material* check_proxy_replacement(void* proxyData) = 0;
	virtual void			refresh_preserving_material_vars() = 0;
	virtual bool			was_reloaded_from_whitelist() = 0;
	virtual bool	is_precached() const = 0;
};

class i_texture_regenerator
{
public:
	virtual void regenerate_texture_bits(i_texture* pTexture, ivtf_texture* pVTFTexture, rect_t* pRect) = 0;
	virtual void release() = 0;
};

class i_texture
{
public:
	virtual const char* get_name(void) const = 0;
	virtual int get_mapping_width() const = 0;
	virtual int get_mapping_height() const = 0;
	virtual int get_actual_width() const = 0;
	virtual int get_actual_height() const = 0;
	virtual int get_num_animation_frames() const = 0;
	virtual bool is_translucent() const = 0;
	virtual bool is_mipmapped() const = 0;
	virtual void get_low_res_color_sample(float s, float t, float* color) const = 0;
	virtual void* get_resource_data(uint32_t eDataType, size_t* pNumBytes) const = 0;
	virtual void increment_reference_count(void) = 0;
	virtual void decrement_reference_count(void) = 0;
	inline void add_ref() { increment_reference_count(); }
	inline void release() { decrement_reference_count(); }
	virtual void set_texture_regenerator(i_texture_regenerator* pTextureRegen) = 0;
	virtual void download(rect_t* pRect = 0, int nAdditionalCreationFlags = 0) = 0;
	virtual int get_approximate_vid_mem_bytes(void) const = 0;
	virtual bool is_error() const = 0;
	virtual bool is_volume_texture() const = 0;
	virtual int get_mapping_depth() const = 0;
	virtual int get_actual_depth() const = 0;
	virtual image_format get_image_format() const = 0;
	virtual int get_normal_decode_mode() const = 0;
	virtual bool is_render_target() const = 0;
	virtual bool is_cube_map() const = 0;
	virtual bool is_normal_map() const = 0;
	virtual bool is_procedural() const = 0;
	virtual void delete_if_unreferenced() = 0;
	virtual void swap_contents(i_texture* pOther) = 0;
	virtual unsigned int get_flags(void) const = 0;
	virtual void force_lod_override(int iNumLodsOverrideUpOrDown) = 0;
	virtual bool save_to_file(const char* fileName) = 0;
};