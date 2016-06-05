

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

struct regex_computation_node;
struct regex_computation_node {
	struct regex_computation_node * path1;
	struct regex_computation_node * path2;
	struct regex_computation_node * after;
	struct character_class_range * ranges; /*  Linked list of character ranges */
	enum regex_computation_node_type type;
	unsigned int match_id; /*  Used when matching against multiple regexes. */
	unsigned int node_id;  /*  Used to tell if a node is already in a list or not */
	unsigned char c;
	unsigned char pad1;
	unsigned char pad2;
	unsigned char pad3;
};
