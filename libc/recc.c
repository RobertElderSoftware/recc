/*
    Copyright 2016 Robert Elder Software Inc.
    
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

#include "../recc-implementation/compiler_interface.h"

void register_libc_objects(struct build_state *);

void register_libc_objects(struct build_state * state){
	/*  Standard Library Objects */
        register_c_to_l2(state, "libc/string");
        register_c_to_l2(state, "libc/printf");
        register_c_to_l2(state, "libc/malloc");
        register_c_to_l2(state, "libc/assert");
        register_c_to_l2(state, "libc/fopen");
        register_c_to_l2(state, "libc/filesystem");
        register_c_to_l2(state, "libc/getcwd");
        register_c_to_l2(state, "libc/putchar");

	register_l2_file(state, "libc/libc.l2", "RELOCATABLE");

        register_dependency(state, "libc/libc.l2", "libc/string.l2");
        register_dependency(state, "libc/libc.l2", "libc/printf.l2");
        register_dependency(state, "libc/libc.l2", "libc/malloc.l2");
        register_dependency(state, "libc/libc.l2", "libc/assert.l2");
        register_dependency(state, "libc/libc.l2", "libc/fopen.l2");
        register_dependency(state, "libc/libc.l2", "libc/getcwd.l2");
        register_dependency(state, "libc/libc.l2", "libc/filesystem.l2");
        register_dependency(state, "libc/libc.l2", "libc/putchar.l2");
}
