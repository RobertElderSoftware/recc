/*
    Copyright 2019 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/

#include "../recc-implementation/compiler_interface_phase_3.h"

void register_libc_objects(struct build_state *);

void register_libc_objects(struct build_state * state){
	/*  Standard Library Objects */
        register_entity(state, "libc/string.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "libc/string.c", "terminal", "true");
        register_entity(state, "libc/printf.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "libc/printf.c", "terminal", "true");
        register_entity(state, "libc/malloc.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "libc/malloc.c", "terminal", "true");
        register_entity(state, "libc/assert.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "libc/assert.c", "terminal", "true");
        register_entity(state, "libc/fopen.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "libc/fopen.c", "terminal", "true");
        register_entity(state, "libc/filesystem.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "libc/filesystem.c", "terminal", "true");
        register_entity(state, "libc/getcwd.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "libc/getcwd.c", "terminal", "true");
        register_entity(state, "libc/putchar.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "libc/putchar.c", "terminal", "true");

        register_entity(state, "libc/string.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "libc/printf.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "libc/malloc.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "libc/assert.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "libc/fopen.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "libc/filesystem.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "libc/getcwd.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "libc/putchar.i", ENTITY_TYPE_PREPROCESSED_FILE);

        register_dependency(state, "libc/string.i",     "libc/string.c");
        register_dependency(state, "libc/printf.i",     "libc/printf.c");
        register_dependency(state, "libc/malloc.i",     "libc/malloc.c");
        register_dependency(state, "libc/assert.i",     "libc/assert.c");
        register_dependency(state, "libc/fopen.i",      "libc/fopen.c");
        register_dependency(state, "libc/filesystem.i", "libc/filesystem.c");
        register_dependency(state, "libc/getcwd.i",     "libc/getcwd.c");
        register_dependency(state, "libc/putchar.i",    "libc/putchar.c");

        register_entity(state, "libc/string.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "libc/printf.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "libc/malloc.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "libc/assert.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "libc/fopen.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "libc/filesystem.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "libc/getcwd.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "libc/putchar.l2", ENTITY_TYPE_L2_FILE);

        register_dependency(state, "libc/string.l2",    "libc/string.i");
        register_dependency(state, "libc/printf.l2",    "libc/printf.i");
        register_dependency(state, "libc/malloc.l2",    "libc/malloc.i");
        register_dependency(state, "libc/assert.l2",    "libc/assert.i");
        register_dependency(state, "libc/fopen.l2",     "libc/fopen.i");
        register_dependency(state, "libc/filesystem.l2","libc/filesystem.i");
        register_dependency(state, "libc/getcwd.l2",    "libc/getcwd.i");
        register_dependency(state, "libc/putchar.l2",   "libc/putchar.i");

	register_entity(state, "libc/libc.l2", ENTITY_TYPE_L2_FILE);

        register_dependency(state, "libc/libc.l2", "libc/string.l2");
        register_dependency(state, "libc/libc.l2", "libc/printf.l2");
        register_dependency(state, "libc/libc.l2", "libc/malloc.l2");
        register_dependency(state, "libc/libc.l2", "libc/assert.l2");
        register_dependency(state, "libc/libc.l2", "libc/fopen.l2");
        register_dependency(state, "libc/libc.l2", "libc/getcwd.l2");
        register_dependency(state, "libc/libc.l2", "libc/filesystem.l2");
        register_dependency(state, "libc/libc.l2", "libc/putchar.l2");
}
