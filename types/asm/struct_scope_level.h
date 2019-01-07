

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

struct scope_level{
	struct tag_declaration_namespace tag_declarations;
	struct identifier_declaration_namespace identifier_declarations;
	struct struct_unscoped_tag_specifier_id_to_struct_struct_or_union_definition_ptr_map struct_or_union_definitions;
	struct struct_unscoped_tag_specifier_id_to_struct_enum_definition_ptr_map enum_definitions;
	struct scope_level * parent_scope;
	struct struct_scope_level_ptr_list scopes;
	struct unsigned_char_ptr_list evaluate_labels; /*  For continue statements  */
	struct unsigned_char_ptr_list end_labels;      /*  For break statements  */
	enum scope_level_type type;
	unsigned int next_anonymous_tag_id; /*  For uniquely identifiying anonymous tags in this scope */
};
