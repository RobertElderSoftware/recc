

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

#define NUM_L2_TOKEN_TYPES 43

enum l2_token_type {
	L2_SPACE,
	L2_NEWLINE,
	L2_COMMENT,
	L2_IDENTIFIER,
	L2_COLON_CHAR,
	L2_COMMA_CHAR,
	L2_MINUS_CHAR,
	L2_OFFSET,
	L2_RELOCATABLE,
	L2_IMPLEMENTS,
	L2_IMPLEMENTED,
	L2_REQUIRES,
	L2_REQUIRED,
	L2_INTERNAL,
	L2_EXTERNAL,
	L2_REGISTER,
	L2_FUNCTION,
	L2_VARIABLE,
	L2_CONSTANT,
	L2_STRING,
	L2_UNRESOLVED,
	L2_REGION,
	L2_START,
	L2_END,
	L2_PERMISSION,
	L2_CONSTANT_HEX,
	L2_CONSTANT_DECIMAL,
	L2_ADD,
	L2_SUB,
	L2_MUL,
	L2_DIV,
	L2_BEQ,
	L2_BLT,
	L2_LOA,
	L2_STO,
	L2_LL,
	L2_AND,
	L2_OR,
	L2_NOT,
	L2_SHR,
	L2_SHL,
	L2_DW,
	L2_SW
};
