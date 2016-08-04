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

#include "compiler_interface_phase_2.h"

void new_register_data_structures_objects(struct build_state *);

int main(void){
	struct build_state * state;
	state = create_build_state();

	new_register_data_structures_objects(state);
	construct_generated_c_entities(state);

        destroy_build_state(state);
	return 0;
}
