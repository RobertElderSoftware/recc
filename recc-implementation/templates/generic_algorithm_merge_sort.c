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
#include "T0_IDENTIFIER_merge_sort.h"

void T0_IDENTIFIER_split_and_merge(T0_LITERAL *, unsigned int, unsigned int, T0_LITERAL *, int (*)(T0_LITERAL *, T0_LITERAL *));
void T0_IDENTIFIER_do_merge(T0_LITERAL *, unsigned int, unsigned int, unsigned int, T0_LITERAL *, int (*)(T0_LITERAL *, T0_LITERAL *));
void T0_IDENTIFIER_copy_items(T0_LITERAL *, unsigned int, unsigned int, T0_LITERAL *);

void T0_IDENTIFIER_merge_sort(T0_LITERAL * a, unsigned int size, int (*cmp_fcn)(T0_LITERAL *, T0_LITERAL *)){
	T0_LITERAL * b = (T0_LITERAL *)malloc(size * sizeof(T0_LITERAL));
	T0_IDENTIFIER_split_and_merge(a, 0, size, b, cmp_fcn);
	free((void*)b);
}

void T0_IDENTIFIER_split_and_merge(T0_LITERAL * a, unsigned int first_index, unsigned int last_index_plus_one, T0_LITERAL * b, int (*cmp_fcn)(T0_LITERAL *, T0_LITERAL *)){
	unsigned int middle;
	if(last_index_plus_one - first_index < 2){
        	return;
	}
	middle = (last_index_plus_one + first_index) / 2;
	T0_IDENTIFIER_split_and_merge(a, first_index, middle, b, cmp_fcn);
	T0_IDENTIFIER_split_and_merge(a, middle, last_index_plus_one, b, cmp_fcn);
	T0_IDENTIFIER_do_merge(a, first_index, middle, last_index_plus_one, b, cmp_fcn);
	T0_IDENTIFIER_copy_items(b, first_index, last_index_plus_one, a);
}

void T0_IDENTIFIER_do_merge(T0_LITERAL * a, unsigned int first_index, unsigned int middle, unsigned int last_index_plus_one, T0_LITERAL * b, int (*cmp_fcn)(T0_LITERAL *, T0_LITERAL *)){
	unsigned int j;
	unsigned int list1_head = first_index;
	unsigned int list2_head = middle;
    
	for(j = first_index; j < last_index_plus_one; j++){
		if(list1_head < middle && (list2_head >= last_index_plus_one || (cmp_fcn(&a[list1_head], &a[list2_head]) < 0))){
			b[j] = a[list1_head];
			list1_head++;
		}else{
			b[j] = a[list2_head];
			list2_head++;
		}
	}
}

void T0_IDENTIFIER_copy_items(T0_LITERAL * b, unsigned int first_index, unsigned int last_index_plus_one, T0_LITERAL * a){
	unsigned int k;
	for(k = first_index; k < last_index_plus_one; k++){
		a[k] = b[k];
	}
}
