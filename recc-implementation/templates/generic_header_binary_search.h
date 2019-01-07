

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

enum T0_IDENTIFIER_binary_search_not_found_result_type{
	T0_IDENTIFIER_BINARY_SEARCH_RETURN_NEG_ONE,
	T0_IDENTIFIER_BINARY_SEARCH_RETURN_MIN,
	T0_IDENTIFIER_BINARY_SEARCH_RETURN_MAX
};

int T0_IDENTIFIER_binary_search(T0_LITERAL * a, T0_LITERAL *, int, int (*)(T0_LITERAL *, T0_LITERAL *), enum T0_IDENTIFIER_binary_search_not_found_result_type);
