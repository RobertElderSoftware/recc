

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

struct l2_item{
	struct linker_symbol * referenced_linker_symbol;
	enum l2_token_type op_type;
	unsigned int rx_number;
	unsigned int ry_number;
	unsigned int rz_number;
	unsigned int number_value;
	unsigned int number_value_is_negative;
	unsigned int post_linking_offset; /* The offset (in # of 4 byte words) of this instruction after we've considered instructions that need to be re-written by the linker */
	unsigned int pre_linking_offset; /* The offset (in # of 4 byte words) of this instruction before moving symbols around in a linker object */
};
