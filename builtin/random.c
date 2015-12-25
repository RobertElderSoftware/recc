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
/*  A very simple linear congruential generator */

static unsigned int x_i = 7;

unsigned int get_random(void);
unsigned int get_input_space_partitioned_random(void);

unsigned int get_random(void){
	unsigned int a = 1103515245;
	unsigned int c = 12345;
	unsigned int m = (unsigned int)0x1 << (unsigned int)31;
	x_i = (a * x_i + c) % m;
	return x_i;
}

unsigned int get_input_space_partitioned_random(void){
	unsigned int cl = get_random() % (unsigned int)5;
	switch (cl){
		case 0:{
			return get_random();
		}case 1:{
			return (unsigned int)0;
		}case 2:{
			return (unsigned int)1;
		}case 3:{
			return (unsigned int)4294967295;
		}case 4:{
			return get_random() % (unsigned int)300;
		}default:{
			return 0;
		}
	}
}
