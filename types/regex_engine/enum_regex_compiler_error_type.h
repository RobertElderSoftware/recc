

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

enum regex_compiler_error_type{
	REGEX_ERROR_INVALID_CHARACTER_SEQUENCE,
	REGEX_ERROR_INVALID_CHARACTER_CLASS,
	REGEX_ERROR_NOT_ESCAPABLE,
	REGEX_ERROR_EXPECTED_HEX_DIGIT,
	REGEX_ERROR_EXPECTED_INFIX_EXPRESSION_REST,
	REGEX_ERROR_INCOMPLETE_PARSING,
	REGEX_ERROR_PARSING_FAILED,
	REGEX_ERROR_INVALID_REGULAR_EXPRESSION,
	REGEX_ERROR_INVALID_CHARACTER_RANGE
};


