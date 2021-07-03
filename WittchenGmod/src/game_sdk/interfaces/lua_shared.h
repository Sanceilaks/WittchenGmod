#pragma once
#include "../../math/math.h"
#include <color.h>
#include <vector>

class i_lua_callback
{
public:

	/*0*/	virtual void* create_lua_object() = 0;
	/*1*/	virtual void destroy_lua_object(void* pObject) = 0;

	/*2*/	virtual void error_print(const char* strError) = 0;
	/*3*/	virtual void msg(const char* strMsg) = 0;

	/*4*/	virtual bool can_run_script(const char* strFilename, unsigned long CRC) = 0;
	/*5*/	virtual void on_run_script(const char* strFilename, bool bRun, const char* strScriptContents) = 0;
};
enum class lua_object_type
{

	INVALID = -1,
	NIL,
	BOOL,
	LIGHTUSERDATA,
	NUMBER,
	STRING,
	TABLE,
	FUNCTION,
	USERDATA,
	THREAD,

	ENTITY,
	VECTOR,
	ANGLE,
	PHYSOBJ,
	SAVE,
	RESTORE,
	DAMAGEINFO,
	EFFECTDATA,
	MOVEDATA,
	RECIPIENTFILTER,
	USERCMD,
	SCRIPTEDVEHICLE,

	MATERIAL,
	PANEL,
	PARTICLE,
	PARTICLEEMITTER,
	TEXTURE,
	USERMSG,

	CONVAR,
	IMESH,
	MATRIX,
	SOUND,
	PIXELVISHANDLE,
	DLIGHT,
	VIDEO,
	FILE,

	COUNT
};

class c_lua_interface
{
public:
	/*0*/	virtual void* top(void) = 0;
	/*1*/	virtual void* push(int iStackPos) = 0;
	/*2*/	virtual void* pop(int iStackPos) = 0;
	/*3*/	virtual void* get_table(int iStackPos) = 0;
	/*4*/	virtual void* const get_field(int iStackPos, char const*) = 0;
	/*5*/	virtual void set_field(int iStackPos, char const*) = 0;
	/*6*/	virtual void* create_table(void) = 0;
	/*7*/	virtual void* set_table(int) = 0;
	/*8*/	virtual void* set_meta_table(int) = 0;
	/*9*/	virtual void* get_meta_table(int) = 0;
	/*10*/	virtual void* call(int iStackPos, int) = 0;
	/*11*/	virtual void* p_call(int, int, int) = 0;
	/*12*/	virtual void* equal(int, int) = 0;
	/*13*/	virtual void* raw_equal(int, int) = 0;
	/*14*/	virtual void* insert(int) = 0;
	/*15*/	virtual void* remove(int) = 0;
	/*16*/	virtual void* next(int) = 0;
	/*17*/	virtual void* new_userdata(unsigned int) = 0;
	/*18*/	virtual void* throw_error(char const*) = 0;
	/*19*/	virtual void* check_type(int, int) = 0;
	/*20*/	virtual void* arg_error(int, char const*) = 0;
	/*21*/	virtual void* raw_get(int iStackPos) = 0;
	/*22*/	virtual void* raw_set(int iStackPost) = 0;
	/*23*/	virtual const char* get_string(int, unsigned int*) = 0;
	/*24*/	virtual double get_number(int) = 0;
	/*25*/	virtual bool get_bool(int) = 0;
	/*26*/	virtual void* get_c_function(int) = 0;
	/*27*/	virtual void* get_userdata(int) = 0;
	/*28*/	virtual void push_nil(void) = 0;
	/*29*/	virtual void push_string(char const*, unsigned int) = 0;
	/*30*/	virtual void push_number(double) = 0;
	/*31*/	virtual void push_bool(bool) = 0;
	/*32*/	virtual void push_c_function(int (*)(void*)) = 0;
	/*33*/	virtual void push_c_closure(int (*)(void*), int) = 0;
	/*34*/	virtual void push_userdata(void*) = 0;
	/*35*/	virtual void* reference_create(void) = 0;
	/*36*/	virtual void* reference_free(int) = 0;
	/*37*/	virtual void* reference_push(int) = 0;
	/*38*/	virtual void* push_special(int) = 0;
	/*39*/	virtual bool is_type(int, lua_object_type) = 0;
	/*40*/	virtual lua_object_type get_type(int) = 0;
	/*41*/	virtual void* get_type_name(int) = 0;
	/*42*/	virtual void* create_meta_table_type(char const*, int) = 0;
	/*43*/	virtual void* check_string(int) = 0;
	/*44*/	virtual void* check_number(int) = 0;
	/*45*/	virtual void* obj_len(int) = 0;
	/*46*/	virtual void* get_angle(int) = 0;
	/*47*/	virtual void* get_vector(int) = 0;
	/*48*/	virtual void* push_angle(q_angle const&) = 0;
	/*49*/	virtual void* push_vector(c_vector const&) = 0;
	/*50*/	virtual void* set_state(void*) = 0;
	/*51*/	virtual void* create_meta_table(char const*) = 0;
	/*52*/	virtual void* push_meta_table(int) = 0;
	/*53*/	virtual void* push_user_type(void*, int) = 0;
	/*54*/	virtual void* set_user_type(int, void*) = 0;
	/*55*/	virtual void* init(i_lua_callback*, bool) = 0;
	/*56*/	virtual void* shutdown(void) = 0;
	/*57*/	virtual void* cycle(void) = 0;
	/*58*/	virtual void* global(void) = 0;
	/*59*/	virtual void* get_objectt(int) = 0;
	/*60*/	virtual void* push_lua_object(void*) = 0;
	/*61*/	virtual void* push_lua_function(int (*)(void*)) = 0;
	/*62*/	virtual void* lua_error(char const*, int) = 0;
	/*63*/	virtual void* type_error(char const*, int) = 0;
	/*64*/	virtual void* call_internal(int, int) = 0;
	/*65*/	virtual void* call_internal_no_returns(int) = 0;
	/*66*/	virtual void* call_internal_get_bool(int) = 0;
	/*67*/	virtual void* call_internal_get_string(int) = 0;
	/*68*/	virtual void* call_internal_get(int, void*) = 0;
	/*69*/	virtual void* new_global_table(char const*) = 0;
	/*70*/	virtual void* new_temporary_object(void) = 0;
	/*71*/	virtual bool is_user_data(int) = 0;
	/*72*/	virtual void* get_meta_table_object(char const*, int) = 0;
	/*73*/	virtual void* get_meta_table_object(int) = 0;
	/*74*/	virtual void* get_return(int) = 0;
	/*75*/	virtual bool is_server(void) = 0;
	/*76*/	virtual bool is_client(void) = 0;
	/*77*/	virtual bool is_menu(void) = 0;
	/*78*/	virtual void* destroy_object(void*) = 0;
	/*79*/	virtual void* create_object(void) = 0;
	/*80*/	virtual void* set_member(void*, void*, void*) = 0;
	/*81*/	virtual void* get_new_table(void) = 0;
	/*82*/	virtual void* set_member(void*, float) = 0;
	/*83*/	virtual void* set_member(void*, float, void*) = 0;
	/*84*/	virtual void* set_member(void*, char const*) = 0;
	/*85*/	virtual void* set_member(void*, char const*, void*) = 0;
	/*86*/	virtual void* set_type(unsigned char) = 0;
	/*87*/	virtual void* push_long(long) = 0;
	/*88*/	virtual void* get_flags(int) = 0;
	/*89*/	virtual void* find_on_objects_meta_table(int, int) = 0;
	/*90*/	virtual void* find_object_on_table(int, int) = 0;
	/*91*/	virtual void* set_member_fast(void*, int, int) = 0;
	/*92*/	virtual void* run_string(char const*, char const*, char const*, bool, bool) = 0;
	/*93*/	virtual bool is_equal(void*, void*) = 0;
	/*94*/	virtual void* error(char const*) = 0;
	/*95*/	virtual void* get_string_or_error(int) = 0;
	/*96*/	virtual void* run_lua_module(char const*) = 0;
	/*97*/	virtual void* find_and_run_script(char const*, bool, bool, char const*, bool) = 0;
	/*98*/	virtual void* set_path_id(char const*) = 0;
	/*99*/	virtual void* get_path_id(void) = 0;
	/*100*/	virtual void* error_no_halt(char const*, ...) = 0;
	/*101*/	virtual void* msg(char const*, ...) = 0;
	/*102*/	virtual void* push_path(char const*) = 0;
	/*103*/	virtual void* pop_path(void) = 0;
	/*104*/	virtual void* get_path(void) = 0;
	/*105*/	virtual void* get_color(int) = 0;
	/*106*/	virtual void* push_color(c_color) = 0;
	/*107*/	virtual void* get_stack(int, void*) = 0;
	/*108*/	virtual void* get_info(char const*, void*) = 0;
	/*109*/	virtual void* get_local(void*, int) = 0;
	/*110*/	virtual void* get_upvalue(int, int) = 0;
	/*111*/	virtual void* run_string_ex(char const*, char const*, char const*, bool, bool, bool, bool) = 0;
	/*112*/	virtual void* get_data_string(int, void**) = 0;
	/*113*/	virtual void* error_from_lua(char const*, ...) = 0;
	/*114*/	virtual void* get_current_location(void) = 0;
	/*115*/	virtual void* msg_colour(c_color const&, char const*, ...) = 0;
	/*116*/	virtual void* get_current_file(std::string, std::string) = 0;
	/*117*/	virtual void* compile_string(void*, std::string const&) = 0;
	/*118*/	virtual void* call_function_protected(int, int, bool) = 0;
	/*119*/	virtual void* require(char const*) = 0;
	/*120*/	virtual void* get_actual_type_name(int) = 0;
	/*121*/	virtual void* pre_create_table(int, int) = 0;
	/*122*/	virtual void* push_pooled_string(int) = 0;
	/*123*/	virtual void* get_pooled_string(int) = 0;
	/*124*/	virtual void* add_threaded_call(void*) = 0;
	/*125*/	virtual void* append_stack_trace(char*, unsigned long) = 0;
	/*126*/	virtual void* create_con_var(char const*, char const*, char const*, int) = 0;
	/*127*/	virtual void* create_con_command(char const*, char const*, int, void (*)(void const*), int (*)(char const*, char(*)[128])) = 0;
};


enum lua_find_result
{
	i_have_no_idea
};

class c_lua_shared
{
public:
	/*0*/	virtual void* destr1() = 0;
	/*2*/	virtual void* init(void* (*)(char const*, int*), bool, void*, void*) = 0;
	/*3*/	virtual void* shutdown(void) = 0;
	/*4*/	virtual void* dump_stats(void) = 0;
	/*5*/	virtual c_lua_interface* create_lua_interface(unsigned char, bool) = 0;
	/*6*/	virtual void* close_lua_interface(void*) = 0;
	/*7*/	virtual c_lua_interface* get_lua_interface(unsigned char) = 0;
	/*8*/	virtual void load_file(std::string const&, std::string const&, bool, bool) = 0;
	/*9*/	virtual void* get_cache(std::string const&) = 0;
	/*10*/	virtual void* mount_lua(char const*) = 0;
	/*11*/	virtual void* mount_lua_add(char const*, char const*) = 0;
	/*12*/	virtual void* un_mount_lua(char const*) = 0;
	/*13*/	virtual void* set_file_contents(char const*, char const*) = 0;
	/*14*/	virtual void* set_lua_find_hook(void*) = 0;
	/*15*/	virtual void* find_scripts(std::string const&, std::string const&, std::vector<lua_find_result>&) = 0;
	/*16*/	virtual void* get_stack_traces(void) = 0;
	/*17*/	virtual void* invalidate_cache(std::string const&) = 0;
	/*18*/	virtual void* empty_cache(void) = 0;
};