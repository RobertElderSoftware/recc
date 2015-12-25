#ifndef TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_H__
#define TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_H__
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



struct unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair{
	unsigned char * key;
	struct namespace_object * value;
};

#ifndef TYPES_PARSER_struct_namespace_object_H__
#include "../../types/parser/struct_namespace_object.h"
#endif

#endif
