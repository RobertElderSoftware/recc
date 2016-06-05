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

#include "../../recc-implementation/compiler_interface_phase_3.h"

void build_tests(void);

int main(void){
	struct build_state * state = create_build_state();
	register_entity(state, "demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h",  ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h",  "terminal", "true");
	register_entity(state, "demos/brainfuck-cpp/result.i",  ENTITY_TYPE_PREPROCESSED_FILE);
        register_dependency(state, "demos/brainfuck-cpp/result.i", "demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h");
	construct_entity(state, "demos/brainfuck-cpp/result.i");
	destroy_build_state(state);
	return 0;
}
