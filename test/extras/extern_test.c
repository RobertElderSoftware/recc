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

extern unsigned int global_1;
extern unsigned int global_2;

extern unsigned int test_arr[3];

unsigned int global_1;
unsigned int global_2 = 3;

unsigned int test_arr[3];

struct bootest{
	unsigned int arr[4];
	unsigned int k;	
};

extern struct bootest global_struct_test;

struct bootest global_struct_test;
