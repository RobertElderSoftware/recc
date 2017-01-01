

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

enum asm_jump_statement_kind{
	ASM_JUMP_STATEMENT_RETURN_EXPRESSION,
	ASM_JUMP_STATEMENT_RETURN_NO_EXPRESSION,
	ASM_JUMP_STATEMENT_BREAK,
	ASM_JUMP_STATEMENT_CONTINUE,
	ASM_JUMP_STATEMENT_GOTO
};

