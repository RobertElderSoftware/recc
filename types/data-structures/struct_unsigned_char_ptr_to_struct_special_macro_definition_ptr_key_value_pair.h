#ifndef TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_H_
#define TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_H_
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

#ifndef TYPES_PREPROCESSOR_struct_special_macro_definition_h_
#include "../../types/preprocessor/struct_special_macro_definition.h"
#endif

struct unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair{
	unsigned char * key;
	struct special_macro_definition * value;
};

#endif
