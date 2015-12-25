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

#include "../../compiler_interface.h"

void build_tests(void);

int main(void){
	struct build_state * state = create_build_state();
	register_build_target(state, "demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h",  BUILD_TARGET_C_FILE);
	register_build_target(state, "demos/brainfuck-cpp/result.i",  BUILD_TARGET_PREPROCESSED_FILE);
        register_dependency(state, "demos/brainfuck-cpp/result.i", "demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h");
	construct_build_target(state, "demos/brainfuck-cpp/result.i");
	destroy_build_state(state);
	return 0;
}
