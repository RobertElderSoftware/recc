

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

enum asm_code_statement_kind{
	ASM_CODE_STATEMENT_LABELED_STATEMENT,
	ASM_CODE_STATEMENT_COMPOUND_STATEMENT,
	ASM_CODE_STATEMENT_EXPRESSION_STATEMENT,
	ASM_CODE_STATEMENT_FOR_STATEMENT,
	ASM_CODE_STATEMENT_WHILE_STATEMENT,
	ASM_CODE_STATEMENT_IF_STATEMENT,
	ASM_CODE_STATEMENT_SWITCH_STATEMENT,
	ASM_CODE_STATEMENT_JUMP_STATEMENT
};

