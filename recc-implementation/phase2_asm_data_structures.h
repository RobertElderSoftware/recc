#ifndef PHASE2_ASM_DATA_STRUCTURES
#define PHASE2_ASM_DATA_STRUCTURES
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

#include "../recc-implementation/phase2_data_structures.h"

void register_asm_data_structures_objects(struct build_state *, struct bootstrap_state *);
void register_asm_details_and_ids(struct build_state *);
void register_asm_generated_aggregate_includes(struct build_state *);
void register_asm_state_includes(struct build_state *);

#endif
