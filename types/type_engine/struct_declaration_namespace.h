

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

struct declaration_namespace{
	/*  An ordered list of type ids of things that are declared in this namespace */
	struct unsigned_int_list ordered_type_ids;
	/*  Allows us to map an identifier to the type id it is associated with */
	struct unsigned_int_to_unsigned_int_map identifier_ids_to_type_ids;
	/*  For anonymous types, we simply start with a type id and find out the ordering from there. */
	struct unsigned_int_to_unsigned_int_map type_ids_to_ordered_index;
};
