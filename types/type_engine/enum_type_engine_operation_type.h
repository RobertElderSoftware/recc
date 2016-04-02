#ifndef TYPES_TYPE_ENGINE_enum_type_engine_operation_type_H_
#define TYPES_TYPE_ENGINE_enum_type_engine_operation_type_H_
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

enum type_engine_operation_type{
	TYPE_ENGINE_INCREMENT_SCOPE_DEPTH,
	TYPE_ENGINE_DECREMENT_SCOPE_DEPTH,
	TYPE_ENGINE_ADD_GENERAL_TYPE_LIST_ITEM,
	TYPE_ENGINE_ADD_ALL_SPECIFIER,
	TYPE_ENGINE_ADD_SIMPLE_SPECIFIER,
	TYPE_ENGINE_ADD_STORAGE_CLASS_SPECIFIER,
	TYPE_ENGINE_ADD_SIMPLE_QUALIFIER,
	TYPE_ENGINE_ADD_SPECIFIER_OR_QUALIFIER,
	TYPE_ENGINE_ADD_DECLARATOR_PART,
	TYPE_ENGINE_ADD_POINTER_PART,
	TYPE_ENGINE_ADD_ARRAY_PART,
	TYPE_ENGINE_ADD_FUNCTION_PART,
	TYPE_ENGINE_ADD_PARAMETER_LIST,
	TYPE_ENGINE_ADD_BITFIELD_OR_DECLARATOR,
	TYPE_ENGINE_ADD_GENERAL_TYPE,
	TYPE_ENGINE_ADD_CONSTANT_PARSER_NODE,
	TYPE_ENGINE_ADD_SPECIFIER_QUALIFIER_LIST_ITEM,
	TYPE_ENGINE_ADD_DECLARATOR_PART_LIST_ITEM
};

#endif
