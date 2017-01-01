

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

enum asm_unary_operator_application_kind{
	ASM_UNARY_OPERATOR_APPLICATION_NOTHING,
	ASM_UNARY_OPERATOR_APPLICATION_INC,
	ASM_UNARY_OPERATOR_APPLICATION_DEC,
	ASM_UNARY_OPERATOR_APPLICATION_ADDRESS,
	ASM_UNARY_OPERATOR_APPLICATION_POINT_TO,
	ASM_UNARY_OPERATOR_APPLICATION_POSITIVE,
	ASM_UNARY_OPERATOR_APPLICATION_NEGATIVE,
	ASM_UNARY_OPERATOR_APPLICATION_BITWISE_NOT,
	ASM_UNARY_OPERATOR_APPLICATION_LOGICAL_NOT
};

