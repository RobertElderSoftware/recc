#ifndef OP_CPU_struct_l0_item_H_
#define OP_CPU_struct_l0_item_H_
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

#ifndef OP_CPU_enum_l0_instruction_type_H_
#include "enum_l0_instruction_type.h"
#endif
#ifndef OP_CPU_enum_l0_directive_type_H_
#include "enum_l0_directive_type.h"
#endif

struct l0_item{
	unsigned int instruction_registers[3];
	enum l0_instruction_type type;
	enum l0_directive_type directive_type;
	unsigned int constant_is_negative;
	unsigned int constant;
};

#endif
