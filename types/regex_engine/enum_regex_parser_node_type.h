

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

enum regex_parser_node_type{
	REGEX_HEX_DIGIT,
	REGEX_ESCAPABLE_CHARACTER,
	REGEX_ESCAPED_CHARACTER,
	REGEX_CHARACTER_RANGE,
	REGEX_CHARACTER_OR_RANGE,
	REGEX_CHARACTER_OR_CLASS,
	REGEX_CHARACTER_CLASS_REST,
	REGEX_CHARACTER_CLASS,
	REGEX_NON_CLASS_CHARACTER,
	REGEX_CHARACTER_SEQUENCE_REST,
	REGEX_CHARACTER_SEQUENCE,
	REGEX_POSTFIX_OPERATOR,
	REGEX_POSTFIX_EXPRESSION,
	REGEX_INFIX_EXPRESSION_REST,
	REGEX_INFIX_EXPRESSION,
	REGEX_REGULAR_EXPRESSION,
	REGEX_TERMINAL,
	REGEX_EPSILON
};


