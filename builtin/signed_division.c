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
int signed_division(unsigned int, unsigned int);

int signed_division(unsigned int a, unsigned int b){
	unsigned int high_a = a & (0x80000000);
	unsigned int high_b = b & (0x80000000);
	unsigned int size_a;
	unsigned int size_b;
	if(high_a){
		size_a = ~a + 1; /* For negatives the magnitude is given from the two's complement */
	}else{
		size_a = a;
	}
	if(high_b){
		size_b = ~b + 1; /* For negatives the magnitude is given from the two's complement */
	}else{
		size_b = b;
	}
	if(high_a && high_b){
		return (int)(size_a / size_b);
	}else if(!high_a && !high_b){
		return (int)(size_a / size_b);
	}else if(!high_a && high_b){
		return -(int)(size_a / size_b);
	}else{
		return -(int)(size_a / size_b);
	}
}
