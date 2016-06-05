

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

struct namespace_object;

/*GETS_REPLACED_WITH_INCLUDES*/

struct namespace_object{
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * children;
	struct struct_normalized_declaration_element_ptr_list elements; /* All of the declaration elements related to one named symbol */
	struct scope_level * scope_level;
	struct namespace_object * previous; /* The previous declaration element (previous param, or previous local stack varable) */
	struct namespace_object * next; /* The previous declaration element (previous param, or previous local stack varable) */
	enum object_location object_location;
	unsigned int first_load;
	int offset;
	unsigned int pad;
};
