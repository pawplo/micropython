#include "py/obj.h"
#include "py/runtime.h"

#include "chacha_test.h"

STATIC mp_obj_t chacha_test_function() {
    chacha_test();
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(chacha_test_function_obj, chacha_test_function);

STATIC const mp_rom_map_elem_t chacha_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_chacha) },
    { MP_ROM_QSTR(MP_QSTR_chacha_test), MP_ROM_PTR(&chacha_test_function_obj) },
};
STATIC MP_DEFINE_CONST_DICT(chacha_module_globals, chacha_module_globals_table);

const mp_obj_module_t chacha_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&chacha_module_globals,
};
MP_REGISTER_MODULE(MP_QSTR_chacha, chacha_module);
