

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

/*GETS_REPLACED_WITH_INCLUDES*/

enum asm_operation_type{
	ASM_OP_ADD_AGGREGATE_TYPE_SPECIFIER,
	ASM_OP_ADD_SIMPLE_TYPE_SPECIFIER,
	ASM_OP_ADD_SIMPLE_TYPE_QUALIFIER,
	ASM_OP_ADD_SIMPLE_STORAGE_CLASS_SPECIFIER,
	ASM_OP_ADD_SPECIFIER_OR_QUALIFIER,
	ASM_OP_ADD_SPECIFIER_OR_QUALIFIER_LIST_ITEM,
	ASM_OP_ADD_PARAMETER_LIST,
	ASM_OP_ADD_DECLARATOR_PART,
	ASM_OP_ADD_ANY_STATEMENT_LIST_ITEM,
	ASM_OP_ADD_ANONYMOUS_TAG_DEFINITION,
	ASM_OP_ADD_NAMED_TAG_PREDECLARATION,
	ASM_OP_ADD_NAMED_TAG_DEFINITION,
	ASM_OP_ADD_TAG_DEFINITION,
	ASM_OP_ADD_STRUCT_OR_UNION_MEMBER_LIST_ITEM,
	ASM_OP_ADD_ENUM_MEMBER_LIST_ITEM,
	ASM_OP_ADD_ENUM_MEMBER,
	ASM_OP_ADD_STRUCT_OR_UNION_MEMBER,
	ASM_OP_ADD_INITIALIZER,
	ASM_OP_ADD_INITIALIZER_LIST_ITEM,
	ASM_OP_ADD_DECLARATOR_PART_LIST_ITEM,
	ASM_OP_ADD_POINTER_PART,
	ASM_OP_ADD_FUNCTION_PART,
	ASM_OP_ADD_ARRAY_PART,
	ASM_OP_ADD_GENERAL_TYPE,
	ASM_OP_ADD_IDENTIFIER_NAMESPACE_ENTRY,
	ASM_OP_ADD_GENERAL_TYPE_LIST_ITEM,
	ASM_OP_ADD_POSTFIX_EXPRESSION_PART_LIST_ITEM,
	ASM_OP_ADD_POSTFIX_EXPRESSION_PART,
	ASM_OP_ADD_UNSCOPED_TAG_SPECIFIER,
	ASM_OP_ADD_SCOPED_TYPENAME_SPECIFIER,
	ASM_OP_ADD_SCOPED_TAG_SPECIFIER,
	ASM_OP_ADD_UNSCOPED_UNION_SPECIFIER,
	ASM_OP_ADD_UNSCOPED_STRUCT_SPECIFIER,
	ASM_OP_ADD_UNSCOPED_ENUM_SPECIFIER,
	ASM_OP_ADD_BITFIELD,
	ASM_OP_ADD_JUMP_STATEMENT,
	ASM_OP_ADD_SWITCH_STATEMENT,
	ASM_OP_ADD_IF_STATEMENT,
	ASM_OP_ADD_WHILE_STATEMENT,
	ASM_OP_ADD_FOR_STATEMENT,
	ASM_OP_ADD_CODE_STATEMENT,
	ASM_OP_ADD_LABELED_STATEMENT,
	ASM_OP_ADD_POSTFIX_EXPRESSION,
	ASM_OP_ADD_PRIMARY_EXPRESSION,
	ASM_OP_ADD_ANY_STATEMENT,
	ASM_OP_ADD_SCOPED_STATEMENT,
	ASM_OP_ADD_DECLARATION_STATEMENT,
	ASM_OP_ADD_FUNCTION_DEFINITION,
	ASM_OP_ADD_SIMPLE_DECLARATION,
	ASM_OP_ADD_EXPRESSION,
	ASM_OP_ADD_CAST_EXPRESSION,
	ASM_OP_ADD_UNARY_EXPRESSION,
	ASM_OP_ADD_BINARY_EXPRESSION,
	ASM_OP_ADD_TERNARY_EXPRESSION,
	ASM_OP_ADD_CONSTANT_DESCRIPTION,
	ASM_OP_ADD_DATA_PACKAGE,
	ASM_OP_ADD_ERROR_NODE,
	ASM_OP_ADD_BYTE_PACKAGE,
	ASM_OP_ADD_HWORD_PACKAGE,
	ASM_OP_ADD_WORD_PACKAGE,
	ASM_OP_ADD_QWORD_PACKAGE,
	ASM_OP_ADD_DWORD_PACKAGE,
	ASM_OP_ADD_STRING_PACKAGE,
	ASM_OP_ADD_IDENTIFIER,
	ASM_OP_ADD_SCOPE_GUID,
	ASM_OP_ADD_ANONYMOUS_ENTRY
};

