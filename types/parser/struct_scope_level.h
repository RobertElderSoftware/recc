#ifndef TYPES_PARSER_struct_scope_level_H_
#define TYPES_PARSER_struct_scope_level_H_
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


#ifndef TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_list_H_
#include "../../types/data-structures/struct_unsigned_char_ptr_list.h"
#endif
#ifndef TYPES_PARSER_struct_namespace_object_H_
#include "struct_namespace_object.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_struct_namespace_object_ptr_map_H_
#include "../../types/data-structures/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_map.h"
#endif

struct scope_level;

struct scope_level{
	struct scope_level * parent_scope;
	struct scope_level ** scopes;
	struct unsigned_char_ptr_list evaluate_labels; /*  For continue statements  */
	struct unsigned_char_ptr_list end_labels;      /*  For break statements  */
	struct namespace_object * current_function;
	struct namespace_object * first_local_object;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map tag_namespace;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map label_namespace;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map identifier_namespace;
	unsigned int num_sub_scopes;
	unsigned int pad;
};

#endif
