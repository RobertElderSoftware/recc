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

int T0_IDENTIFIER_compare(T0_LITERAL * a, T0_LITERAL * b){
        if(a->key.item_id.id > b->key.item_id.id){
                return 1;
        }else if(a->key.item_id.id < b->key.item_id.id){
                return -1;
        }else{
		if(a->key.prev_id.id > b->key.prev_id.id){
			return 1;
		}else if(a->key.prev_id.id < b->key.prev_id.id){
			return -1;
		}else{
			if(a->key.type > b->key.type){
				return 1;
			}else if(a->key.type < b->key.type){
				return -1;
			}else{
				return 0;
			}
		}
        }
}
