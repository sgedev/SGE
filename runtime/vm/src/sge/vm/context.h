/*
 *
 */
#ifndef SGE_VM_CONTEXT_H
#define SGE_VM_CONTEXT_H

#include <lua.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

struct sge_vm_context {
    const char* name;

    int (*run)(lua_State* L);
    int (*wait_event)(lua_State* L);

    int (*get_graphics_enabled)(lua_State* L);
    int (*set_graphics_enabled)(lua_State* L);

    int (*get_sound_enabled)(lua_State* L);
    int (*set_sound_enabled)(lua_State* L);

    int (*get_name)(lua_State* L);
    int (*set_name)(lua_State* L);

    int (*get_enabled)(lua_State* L);
    int (*set_enabled)(lua_State* L);

    int (*get_visible)(lua_State* L);
    int (*set_visible)(lua_State* L);

    int (*get_color)(lua_State* L);
    int (*set_color)(lua_State* L);

    int (*get_ambient_light)(lua_State* L);
    int (*set_amtient_light)(lua_State* L);

    int (*get_gravity)(lua_State* L);
    int (*set_gravity)(lua_State* L);
};

extern const sge_vm_context_t SGE_VM_NONE;
extern const sge_vm_context_t SGE_VM_WORLD;
extern const sge_vm_context_t SGE_VM_LIGHT;
extern const sge_vm_context_t SGE_VM_ENTITY;

SGE_END_DECLS

#endif /* SGE_VM_CONTEXT_H */
