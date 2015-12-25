/*
    Copyright 2015 Robert Elder Software Inc.
    
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

#include "compiler_interface.h"

void register_compiler_objects(struct build_state *);

void register_compiler_objects(struct build_state * state){
	/*  Implemention of emulated ext like filesystem */
	register_build_target(state, "recc-implementation/filesystem_impl.l2",         BUILD_TARGET_FILESYSTEM_IMPLEMENTATION);
	/*  Parts of the compiler */
	register_c_to_l2(state, "recc-implementation/io");
	register_c_to_l2(state, "recc-implementation/preprocessor");
	register_c_to_l2(state, "recc-implementation/code_generator");
	register_c_to_l2(state, "recc-implementation/parser");
	register_c_to_l2(state, "recc-implementation/linker");
	register_c_to_l2(state, "recc-implementation/lexer");
	register_c_to_l2(state, "recc-implementation/heap_memory_pool");
	register_c_to_l2(state, "recc-implementation/memory_pool_collection");
}
