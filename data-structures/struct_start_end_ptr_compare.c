/*
    Copyright 2015 Robert Elder Software Inc.
    
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
#ifndef struct_start_end_ptr_compare__H__DEFINED__
#include "struct_start_end_ptr_compare.h"
#endif

int struct_start_end_ptr_compare(struct start_end * a, struct start_end * b){
	if(a->symbol->l2_item->pre_linking_offset > b->symbol->l2_item->pre_linking_offset){
		return 1;
	}else if(a->symbol->l2_item->pre_linking_offset < b->symbol->l2_item->pre_linking_offset){
		return -1;
	}else{
		/*  If they are from the same source symbol, order so that */
		if(a->is_start && !b->is_start){
			return 1;
		}else if(!a->is_start && b->is_start){
			return -1;
		}else{
			/*  If all else is the same, sort them based on which symbol they belong to 
			    so that symbols starts and ends will be grouped together */
			if(a->source > b->source){
				return 1;
			}else if(a->source < b->source){
				return -1;
			}else{
				return 0;
			}
		}
	}
}
