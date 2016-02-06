#ifndef TYPES_struct_build_target_H_
#define TYPES_struct_build_target_H_
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
#ifndef TYPES_enum_build_target_type_H_
#include "enum_build_target_type.h"
#endif
#ifndef TYPES_struct_l0_build_info_H_
#include "struct_l0_build_info.h"
#endif
#ifndef TYPES_struct_l1_build_info_H_
#include "struct_l1_build_info.h"
#endif
#ifndef TYPES_struct_l2_build_info_H_
#include "struct_l2_build_info.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_struct_build_target_ptr_map_H_
#include "../../types/data-structures/struct_unsigned_char_ptr_to_struct_build_target_ptr_map.h"
#endif

struct build_target{
	struct l0_build_info * l0_info;
	struct l1_build_info * l1_info;
	struct l2_build_info * l2_info;
	unsigned char * name;
	struct unsigned_char_ptr_to_struct_build_target_ptr_map children; /*  Things this target depends on */
	struct unsigned_char_ptr_to_struct_build_target_ptr_map parents;  /*  Things that depend on this target */
	enum build_target_type type;
	unsigned int satisfied;     /*  True when this target has already been built. */
	unsigned int is_relocatable;
	unsigned int linking_offset;
};

#endif
