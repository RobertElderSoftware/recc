

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

struct regex_matcher_state{
	unsigned int * next_occupied_list;     /*  Describes whether a node is in the 'next' list of things to compute */
	unsigned int * current_occupied_list;  /*  Describes whether a node is in the 'current' list of things to compute */
	struct regex_computation_node ** next_list;    /*  The list of things to compute for the next character */
	struct regex_computation_node ** current_list; /*  The list of things to compute for the current character */
	struct regex_compiler_state ** compiler_states;/*  All the states of the compiled regexes */
	unsigned int num_compiled_regexes;    /*  Total number of regexes this matcher is trying to match */
	unsigned int num_unique_matches_found; /*  Number of unique regex matches we've found so far. */
	unsigned int next_current_list_index;  /*  The index of the next node in the list for the current characte */
	unsigned int next_next_list_index;     /*  The index of the next node in the list for the next characte */
	struct regex_match * possible_matches; /*  An array of all possible matches */
	struct regex_match ** actual_matches;  /*  An array of pointers to things that have actually matched */
	unsigned int * num_optimized_possible_matches; /*  Used as an optimization of pre-computed possible matches based on the first character */
	unsigned int ** optimized_match_ids; /*  Use for optimization */
};
