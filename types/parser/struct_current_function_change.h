#ifndef TYPES_PARSER_struct_current_function_change_H_
#define TYPES_PARSER_struct_current_function_change_H_
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

struct current_function_change {
	struct scope_level * target; /* The scope namespace for which we're going to change the current function */
	struct namespace_object * old_obj;
	struct namespace_object * new_obj;
};

#endif
