#ifndef OP_CPU_enum_l0_directive_type_H_
#define OP_CPU_enum_l0_directive_type_H_
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

enum l0_directive_type {
	L0_MACHINE_INSTRUCTION,
	L0_DW_DIRECTIVE,
	L0_SW_DIRECTIVE,
	L0_OFFSET_ADDRESS_DIRECTIVE,
	L0_STRING_DIRECTIVE,
	L0_LINKAGE_DIRECTIVE,
	L0_UNRESOLVED_DIRECTIVE,
	L0_FUNCTION_DIRECTIVE,
	L0_VARIABLE_DIRECTIVE,
	L0_CONSTANT_DIRECTIVE,
	L0_START_DIRECTIVE,
	L0_END_DIRECTIVE,
	L0_IMPLEMENTED_DIRECTIVE,
	L0_REQUIRED_DIRECTIVE,
	L0_REGION_DIRECTIVE,
	L0_PERMISSION_DIRECTIVE,
	L0_IMAGE_SIZE,
	L0_NUM_L0_ITEMS
};

#endif
