

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

/*GETS_REPLACED_WITH_INCLUDES*/

struct scope_level{
	struct declaration_namespace struct_or_union_declarations;
	struct declaration_namespace enum_declarations;
	struct declaration_namespace identifier_declarations;
	struct struct_or_union_definition_collection struct_or_union_definitions;
	struct enum_definition_collection enum_definitions;
	struct scope_level * parent_scope;
	struct struct_scope_level_ptr_list scopes;
	struct unsigned_char_ptr_list evaluate_labels; /*  For continue statements  */
	struct unsigned_char_ptr_list end_labels;      /*  For break statements  */
	struct namespace_object * current_function;
	struct namespace_object * first_local_object;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map tag_namespace;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map label_namespace;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map identifier_namespace;
	enum scope_level_type type;
	unsigned int next_anonymous_tag_id; /*  For uniquely identifiying anonymous tags in this scope */
};
