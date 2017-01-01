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

#include "compiler_interface_phase_3.h"

void register_ri_object(struct build_state *, const char *, unsigned int);

void register_ri_object(struct build_state * state, const char * object, unsigned int libdatastructures_assoc){
	unsigned char * c_name = create_formatted_string(state->memory_pool_collection, 50, "%s.c", object);
	unsigned char * i_name = create_formatted_string(state->memory_pool_collection, 50, "%s.i", object);
	unsigned char * l2_name = create_formatted_string(state->memory_pool_collection, 50, "%s.l2", object);

	register_entity(state, (const char *)c_name, ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, (const char *)c_name, "terminal", "true");
	register_entity(state, (const char *)i_name, ENTITY_TYPE_PREPROCESSED_FILE);
	register_dependency(state, (const char *)i_name, (const char *)c_name);
	register_entity(state, (const char *)l2_name, ENTITY_TYPE_L2_FILE);
	register_dependency(state, (const char *)l2_name, (const char *)i_name);
	if(libdatastructures_assoc){
		register_dependency(state, "recc-implementation/libdata-structures.l2", (const char *)l2_name);
	}

	heap_memory_pool_free(state->memory_pool_collection, c_name);
	heap_memory_pool_free(state->memory_pool_collection, i_name);
	heap_memory_pool_free(state->memory_pool_collection, l2_name);
}

void register_compiler_objects(struct build_state *);

void register_compiler_objects(struct build_state * state){
	/*  Implemention of emulated ext like filesystem */
	register_entity(state, "recc-implementation/filesystem_impl.l2",         ENTITY_TYPE_FILESYSTEM_IMPLEMENTATION);
	/*  Parts of the compiler */
	register_ri_object(state, "recc-implementation/io", 0);
	register_ri_object(state, "recc-implementation/preprocessor", 0);
	register_ri_object(state, "recc-implementation/code_generator", 0);
	register_ri_object(state, "recc-implementation/parser", 0);
	register_ri_object(state, "recc-implementation/linker", 0);
	register_ri_object(state, "recc-implementation/lexer", 0);
	register_ri_object(state, "recc-implementation/memory_pool_collection", 0);
	register_ri_object(state, "recc-implementation/regex_engine", 0);
	register_ri_object(state, "recc-implementation/asm", 0);
	register_ri_object(state, "recc-implementation/replace_tool", 0);
	register_ri_object(state, "recc-implementation/binary_exponential_buffer", 0);
	register_entity(state, "recc-implementation/libdata-structures.l2", ENTITY_TYPE_L2_FILE);
}
