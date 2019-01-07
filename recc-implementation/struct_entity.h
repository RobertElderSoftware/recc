#ifndef recc_implementation_DIR_struct_entity_phase_2_DOT_h
#define recc_implementation_DIR_struct_entity_phase_2_DOT_h
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

#ifndef TYPES_enum_entity_type_H_
#include "../types/recc-implementation/enum_entity_type.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_struct_entity_ptr_map_H_
#include "../generated/struct_unsigned_char_ptr_to_struct_entity_ptr_map.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_unsigned_char_ptr_map_H_
#include "../generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_map.h"
#endif

struct entity{
	struct unsigned_char_ptr_to_unsigned_char_ptr_map attributes;
	unsigned char * name;
	enum entity_type type;
	unsigned int satisfied;     /*  True when this target has already been built. */
	unsigned int registered;
};

#endif
