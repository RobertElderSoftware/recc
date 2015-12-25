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
int signed_greater_than(unsigned int, unsigned int);

int signed_greater_than(unsigned int a, unsigned int b){
	/* Takes a and b and treats their bit patters as twos complement signed integers and performs 32 bit signed a >= b on them */
	unsigned int high_a = a & (0x80000000);
	unsigned int high_b = b & (0x80000000);
	unsigned int low_a = a & (~(0x80000000));
	unsigned int low_b = b & (~(0x80000000));
	if(high_a && high_b){ /* both negative numbers */
		return low_a > low_b; /*  In twos complement, the smallest negatives have the 'largest' bit pattern (after the sign bit) */
	}else if(!high_a && !high_b){ /* both positive numbers */
		return low_a > low_b;
	}else if(!high_a && high_b){
		return 1;
	}else{ /*(high_a && !high_b)*/
		return 0;
	}
}
