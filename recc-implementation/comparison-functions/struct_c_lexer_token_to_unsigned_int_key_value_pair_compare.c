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
#ifndef struct_c_lexer_token_to_unsigned_int_key_value_pair_compare_H_DEFINED_
#include "struct_c_lexer_token_to_unsigned_int_key_value_pair_compare.h"
#endif

int struct_c_lexer_token_to_unsigned_int_key_value_pair_compare(struct struct_c_lexer_token_to_unsigned_int_key_value_pair * a, struct struct_c_lexer_token_to_unsigned_int_key_value_pair * b){
	if(a->key.type > b->key.type){
		return 1;
	}else if(a->key.type < b->key.type){
		return -1;
	}else{
		unsigned char * aa = a->key.first_byte;
		unsigned char * bb = b->key.first_byte;
		do{
			if(*aa > *bb){
				return 1;
			}else if(*aa < *bb){
				return -1;
			}else{
				/*  Keep going... */
			}
			if(aa == a->key.last_byte || bb == b->key.last_byte){
				break;
			}
			aa++;
			bb++;
		}while(1);

		/*  Make sure one is not a substring of the other */
		if(aa == a->key.last_byte && bb == b->key.last_byte){
			return 0;
		}else if(aa != a->key.last_byte && bb == b->key.last_byte){
			return 1;
		}else{
			return -1;
		}
	}
}
