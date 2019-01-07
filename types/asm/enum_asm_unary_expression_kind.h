

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

enum asm_unary_expression_kind{
	ASM_UNARY_EXPRESSION_SIZEOF_EXPRESSION,
	ASM_UNARY_EXPRESSION_POSTFIX_EXPRESSION,
	ASM_UNARY_EXPRESSION_SIZEOF_TYPE,
	ASM_UNARY_EXPRESSION_INCREMENT,
	ASM_UNARY_EXPRESSION_DECREMENT,
	ASM_UNARY_EXPRESSION_ADDRESS_OF,
	ASM_UNARY_EXPRESSION_POINT_TO,
	ASM_UNARY_EXPRESSION_PLUS,
	ASM_UNARY_EXPRESSION_MINUS,
	ASM_UNARY_EXPRESSION_BITWISE_NOT,
	ASM_UNARY_EXPRESSION_LOGICAL_NOT
};

