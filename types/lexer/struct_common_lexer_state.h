

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

struct common_lexer_state {
	struct regex_compiler_state ** compiler_states;
	struct regex_matcher_state matcher;
	unsigned char * buf;
	unsigned char * filename;
	struct memory_pool_collection * memory_pool_collection;
	struct unsigned_char_list * buffered_output;
	unsigned int position;
	unsigned int current_line;
	unsigned int buffer_size;
	unsigned int pad;
};
