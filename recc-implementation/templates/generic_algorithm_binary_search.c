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
#include "T0_IDENTIFIER_binary_search.h"
#include <assert.h>
#include <stdio.h>

int T0_IDENTIFIER_binary_search_impl(T0_LITERAL *, T0_LITERAL *, int, int, int (*)(T0_LITERAL *, T0_LITERAL *), enum T0_IDENTIFIER_binary_search_not_found_result_type);

int T0_IDENTIFIER_binary_search(T0_LITERAL * a, T0_LITERAL * key, int num_items, int (*cmp_fcn)(T0_LITERAL *, T0_LITERAL *), enum T0_IDENTIFIER_binary_search_not_found_result_type result_type){
	if(num_items > 0){
		return T0_IDENTIFIER_binary_search_impl(a, key, 0, num_items -1, cmp_fcn, result_type);
	}else if(num_items == 0){
		return -1;
	}else{
		assert(0 && "negative number of items.");
	}
}

int T0_IDENTIFIER_binary_search_impl(T0_LITERAL * a, T0_LITERAL * key, int imin, int imax, int (*cmp_fcn)(T0_LITERAL *, T0_LITERAL *), enum T0_IDENTIFIER_binary_search_not_found_result_type result_type){
	while (imax >= imin) {
		int imid = imin + ((imax - imin) / 2);
		int cmp_result = cmp_fcn(&a[imid], key);
		if(cmp_result < 0){
			imin = imid + 1;
		}else if(cmp_result > 0){
			imax = imid - 1;
		}else{        
			return imid; 
		}
	}
	switch(result_type){
		case T0_IDENTIFIER_BINARY_SEARCH_RETURN_NEG_ONE:{
			return -1;
		}case T0_IDENTIFIER_BINARY_SEARCH_RETURN_MIN:{
			return imin;
		}case T0_IDENTIFIER_BINARY_SEARCH_RETURN_MAX:{
			return imax;
		}default:{
			assert(0);
			return -1;
		}
	}
}
