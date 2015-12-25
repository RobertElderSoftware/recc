#ifndef TYPES_PARSER_struct_first_and_last_namespace_object_H_
#define TYPES_PARSER_struct_first_and_last_namespace_object_H_
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

#ifndef TYPES_PARSER_struct_namespace_object_H_
#include "struct_namespace_object.h"
#endif

struct first_and_last_namespace_object{
	struct namespace_object * first; /* The first namespace occupying namespace object resulting from a set of declarations  */
	struct namespace_object * last; /* The last ...   */
};

#endif
