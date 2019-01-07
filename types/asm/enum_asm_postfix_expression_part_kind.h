

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

enum asm_postfix_expression_part_kind{
	ASM_POSTFIX_EXPRESSION_PART_ARRAY_DEREFERENCE,
	ASM_POSTFIX_EXPRESSION_PART_STRUCT_DOT_DEREFERENCE,
	ASM_POSTFIX_EXPRESSION_PART_STRUCT_ARROW_DEREFERENCE,
	ASM_POSTFIX_EXPRESSION_PART_FUNCTION_DEREFERENCE,
	ASM_POSTFIX_EXPRESSION_PART_INC,
	ASM_POSTFIX_EXPRESSION_PART_DEC
};

