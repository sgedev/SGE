/*
 *
 */
#ifndef SGE_VM_SYSCALLS_H
#define SGE_VM_SYSCALLS_H

#include <lua.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

void sge_vm_init_syscalls(lua_State* L);

SGE_END_DECLS

#endif /* SGE_VM_SYSCALLS_H */
