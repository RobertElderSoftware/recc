#ifndef OP_CPU_enum_l0_instruction_type_H__
#define OP_CPU_enum_l0_instruction_type_H__
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

enum l0_instruction_type {
	L0_ADD_INSTRUCTION,
	L0_SUB_INSTRUCTION,
	L0_MUL_INSTRUCTION,
	L0_DIV_INSTRUCTION,
	L0_BEQ_INSTRUCTION,
	L0_BLT_INSTRUCTION,
	L0_LOA_INSTRUCTION,
	L0_STO_INSTRUCTION,
	L0_LL_INSTRUCTION,
	L0_AND_INSTRUCTION,
	L0_OR_INSTRUCTION,
	L0_NOT_INSTRUCTION,
	L0_SHR_INSTRUCTION,
	L0_SHL_INSTRUCTION
};

#endif
