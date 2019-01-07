

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

enum regex_computation_node_type {
	REGEX_COMP_UNUSED, /*  To trick clang to not warn of unreachable code in default branch */
	REGEX_COMP_CHAR,
	REGEX_COMP_CHAR_STAR,
	REGEX_COMP_DOT,
	REGEX_COMP_DOT_STAR,
	REGEX_COMP_CLASS,
	REGEX_COMP_CLASS_STAR,
	REGEX_COMP_NEG_CLASS,
	REGEX_COMP_NEG_CLASS_STAR,
	REGEX_COMP_SPLIT,
	REGEX_COMP_JUMP,
	REGEX_COMP_MATCH
};


