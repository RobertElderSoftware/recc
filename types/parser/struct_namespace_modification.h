#ifndef TYPES_PARSER_struct_namespace_modification_H__
#define TYPES_PARSER_struct_namespace_modification_H__
/*
    Copyright 2015 Robert Elder Software Inc.
    
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

struct namespace_modification{
	struct normalized_declaration_element * new_element;
	struct namespace_object * object;
	struct scope_level * scope_level;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * scope_namespace;
	unsigned char * identifier_str;
	enum scope_type scope_type;
	enum object_location object_location;
	int offset;
	int pad;
};

#endif
