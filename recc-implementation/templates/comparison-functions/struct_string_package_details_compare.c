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

int T0_IDENTIFIER_compare(T0_LITERAL * a, T0_LITERAL * b){
	if(a->key.type > b->key.type){
		return 1;
	}else if(a->key.type < b->key.type){
		return -1;
	}else{
		if(a->key.byte_length > b->key.byte_length){
			return 1;
		}else if(a->key.byte_length < b->key.byte_length){
			return -1;
		}else{
			if(a->key.word_length > b->key.word_length){
				return 1;
			}else if(a->key.word_length < b->key.word_length){
				return -1;
			}else{
				unsigned int i;
				for(i = 0; i < a->key.word_length; i++){
					if(a->key.data[i] > b->key.data[i]){
						return 1;
					}else if(a->key.data[i] < b->key.data[i]){
						return -1;
					}
				}
				/*  Everything was the same. */
				return 0;
			}
		}
        }
}
