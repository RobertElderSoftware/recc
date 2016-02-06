#ifndef TYPES_CODE_GENERATOR_struct_compile_time_constant_H_
#define TYPES_CODE_GENERATOR_struct_compile_time_constant_H_
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


#ifndef TYPES_PARSER_struct_constant_description_H_
#include "../../types/parser/struct_constant_description.h"
#endif
#ifndef TYPES_PARSER_struct_normalized_declaration_element_H_
#include "../../types/parser/struct_normalized_declaration_element.h"
#endif

struct compile_time_constant{
	struct constant_description * constant_description; /* For everything else */
	struct normalized_declaration_element * element;    /*  For enum types */
};

#endif
