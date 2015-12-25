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
#include <stdlib.h>
#include <stdio.h>

unsigned int get_input_space_partitioned_random(void);

int main(void){
	unsigned int num_items = 20;
	unsigned int i;
	unsigned int j;
	unsigned int ** p = (unsigned int **)malloc(sizeof(unsigned int *) * num_items);
	char * str = (char *)malloc(sizeof(char) * 8);
	char * str2 = (char *)malloc(sizeof(char) * 256);
	char * cpy = str2;
	for(i = 0; i < 256; i++){
		cpy[0] = (char)i;
		cpy++;
	}
	cpy[0] = 0;
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	str[3] = 'd';
	str[4] = 'e';
	str[5] = 'f';
	str[6] = 'g';
	str[7] = 0;
	printf("%s", str2);
	for(i = 0; i < num_items; i++){
		p[i] = (unsigned int*)malloc(sizeof(unsigned int) * num_items);
		for(j = 0; j < num_items; j++){
			p[i][j] = get_input_space_partitioned_random();
		}
	}
	for(i = 0; i < num_items; i++){
		for(j = 0; j < num_items; j++){
			printf("%X\n", p[i][j]);
		}
		free((void*)p[i]);
	}
	free((void*)p);
	free((void*)str);
	free((void*)str2);
	return 0;
}
