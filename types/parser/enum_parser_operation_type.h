#ifndef TYPES_PARSER_enum_parser_operation_type_H_
#define TYPES_PARSER_enum_parser_operation_type_H_
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

enum parser_operation_type{
	INCREMENT_NEWLINE_COUNT,
	PROCESS_CONSTANT,
	ACCEPT_IDENTIFIER,
	ADVANCE_TOKEN_POSITION,
	ADVANCE_PARSER_POSITION,
	INCREMENT_SCOPE_DEPTH,
	DECREMENT_SCOPE_DEPTH,
	RECORD_EXTERN_DECLARATION,
	SAVE_DECLARATION_ELEMENTS,
	NAMESPACE_MODIFICATION,
	CREATE_PARSER_NODE_TREE,
	SET_NEXT_NAMESPACE_OBJECT,
	SET_PREVIOUS_NAMESPACE_OBJECT,
	SET_CURRENT_FUNCTION
};

#endif
