

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

struct regex_compiler_state{
	struct memory_pool_collection * m; /*  Used for optimizing memory allocation/deallocation */
	struct regex_compiler_error * errors; /*  Is non-null if there was an error during compillation. */
	struct regex_parser_node * top_node; /*  The top node of the parse tree */
	struct regex_computation_node * first_computation_node; /*  The first 'instruction' to execute in the regex */
	unsigned char * buffer;
	unsigned int position;
	unsigned int buffer_length;
	unsigned int match_id;
	unsigned int pad;
};
