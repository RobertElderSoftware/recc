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

#include "../../recc-implementation/compiler_interface.h"

void build_tests(void);

void register_libc_objects(struct build_state *);
void register_data_structures_objects(struct build_state *);
void register_builtin_objects(struct build_state *);
void register_compiler_objects(struct build_state *);
void register_kernel_objects(struct build_state *);

void build_tests(void){
	struct build_state * state = create_build_state();
	register_libc_objects(state);
	register_builtin_objects(state);
	register_data_structures_objects(state);
	register_compiler_objects(state);
	register_kernel_objects(state);

	construct_build_target(state, "emulators/java/OpCPU/KernelData.java");

	destroy_build_state(state);
}

int main(void){
	build_tests();
	return 0;
}
