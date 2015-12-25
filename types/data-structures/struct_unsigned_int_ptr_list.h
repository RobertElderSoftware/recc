#ifndef TYPES_DATA_STRUCTURES_struct_unsigned_int_ptr_list_H__
#define TYPES_DATA_STRUCTURES_struct_unsigned_int_ptr_list_H__
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

#ifndef BINARY_EXPONENTIAL_BUFFER__H__DEFINED__
#include "../../data-structures/binary_exponential_buffer.h"
#endif

struct unsigned_int_ptr_list{
	struct binary_exponential_buffer buffer;
};

#endif
