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
#include <stdarg.h>
#include <stdio.h>
#include "../../data-structures/unsigned_int_merge_sort.h"
#include "../../data-structures/unsigned_int_binary_search.h"

extern unsigned int global_1;
extern unsigned int global_2;
extern unsigned int test_arr[3];

static unsigned int asdf;
static unsigned int asdf2 = 4;

unsigned int get_random(void);
unsigned int get_input_space_partitioned_random(void);

int return_signed_int(void);
unsigned int return_unsigned_int(void);
unsigned int coerce_return_to_unsigned(void);
int coerce_return_to_signed(void);
int many_math(void);

int return_signed_int(void){
	int i = 3;
	return i;
}

unsigned int return_unsigned_int(void){
	unsigned int i = 3;
	return i;
}

int coerce_return_to_signed(void){
	unsigned int i = 3;
	return (int)i;
}

unsigned int coerce_return_to_unsigned(void){
	int i = 3;
	return (unsigned int)i;
}

int many_math(void){
	int i = 3;
	unsigned int j = 3;
	int k = (int)((unsigned int)i - j);
	return k;
}

int many_math(void);

enum foo {type1, type2};

static const char * constant_strings[3] = {"asdf", "fdsa", "lol"};

int cmp_fcn(unsigned int *, unsigned int *);

int cmp_fcn(unsigned int * a, unsigned int *b){
	if(*a > *b){
		return 1;
	}else if(*a < *b){
		return -1;
	}else{
		return 0;
	}
}

int cmp_fcn(unsigned int *, unsigned int *); /*  Test declaring after definition */

int main(void){
	unsigned int i;
	unsigned int j;
	unsigned int k;
	unsigned int b;
	unsigned int c;
	unsigned int array1[26];
	unsigned int array2[26];
	unsigned int num_sort = 100;
	unsigned int * sort_array = (unsigned int *)malloc(num_sort * sizeof(unsigned int));
	test_arr[1] = 7;
	putchar('0' + (int)test_arr[1]);
	asdf = 4;
	asdf2 = 'Z';
	putchar('0' + (int)asdf);
	putchar((int)asdf2);
	asdf = 5;
	global_1 = 5;
	putchar('0' + (int)global_1);
	putchar('0' + (int)global_2);
	putchar('0' + (int)asdf);
	putchar('0' + type1);
	putchar('0' + (unsigned int)type2);
	array1[0] = 0x0;
	array1[1] = 0x1;
	array1[2] = (unsigned int)-1;
	array1[3] = 2147483644;
	array1[4] = 2147483645;
	array1[5] = 2147483646;
	array1[6] = 2147483647;
	array1[7] = 214743648;
	array1[8] = 217483649;
	array1[9] = 214748350;
	array1[10] = (unsigned int)-214743650;
	array1[11] = (unsigned int)-147483649;
	array1[12] = (unsigned int)-2147483648;
	array1[13] = (unsigned int)-2147483647;
	array1[14] = (unsigned int)-2147483646;
	array1[15] = 100000;
	array1[16] = 7;
	array1[17] = 77;
	array1[18] = 777;
	array1[19] = 7777;
	array1[20] = 77777;
	array1[21] = 777777;
	array1[22] = 7777777;
	array1[23] = 77777777;
	array1[24] = 777777777;
	array1[25] = 1777777777;

	array2[0] = 0x0;
	array2[1] = 0x1;
	array2[2] = (unsigned int)-1;
	array2[3] = 2147483644;
	array2[4] = 2147483645;
	array2[5] = 2147483646;
	array2[6] = 2147483647;
	array2[7] = 214743648;
	array2[8] = 217483649;
	array2[9] = 214748350;
	array2[10] = (unsigned int)-214743650;
	array2[11] = (unsigned int)-147483649;
	array2[12] = (unsigned int)-2147483648;
	array2[13] = (unsigned int)-2147483647;
	array2[14] = (unsigned int)-2147483646;
	array2[15] = 100000;
	array2[16] = 7;
	array2[17] = 77;
	array2[18] = 777;
	array2[19] = 7777;
	array2[20] = 77777;
	array2[21] = 777777;
	array2[22] = 7777777;
	array2[23] = 77777777;
	array2[24] = 777777777;
	array2[25] = 1777777777;
	(void)i;
	(void)j;
	(void)array1;
	(void)array2;

	printf("%s", "testing");

	for(k = 0; k < 3; k++){
		printf("%s", constant_strings[k]);
	}

	for(k = 0; k < 2; k++){
		b = 0;
		b += get_input_space_partitioned_random();
		b += get_input_space_partitioned_random();

		c = 0;
		c += get_input_space_partitioned_random();
		c += get_input_space_partitioned_random();
		printf(
			"RND:%X %X %X %X %X %X %X %X %X %X %X %X %X %X\n",
			b * c,
			b + c,
			b - c,
			b && c,
			b || c,
			b & c,
			b | c,
			b ^ c,
			b == c,
			b >= c,
			b > c,
			b <= c,
			b < c,
			b != c
		);

		if(c != 0){
			printf("%X %X\n", b / c, b % c);
		}

		printf(
			"RND:b:%d c:%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
			(int)b,
			(int)c,
			(int)b * (int)c,
			(int)b + (int)c,
			(int)b - (int)c,
			(int)b && (int)c,
			(int)b || (int)c,
			(int)b & (int)c,
			(int)b | (int)c,
			(int)b ^ (int)c,
			(int)b == (int)c,
			(int)b >= (int)c,
			(int)b > (int)c,
			(int)b <= (int)c, (int)b < (int)c,
			(int)b != (int)c
		);

		if(c != 0){
			printf("%d %d\n", (int)b % (int)c, (int)b / (int)c);
		}
	}

	for(k = 0; k < 2; k++){
		b = 0;
		b += get_input_space_partitioned_random();
		b += get_input_space_partitioned_random();

		c = 0;
		c += get_input_space_partitioned_random();
		c += get_input_space_partitioned_random();

		printf("%X\n", b *= c);
		printf("%X\n", b += c);
		printf("%X\n", b -= c);
		printf("%X\n", b &= c);
		printf("%X\n", b |= c);
		printf("%X\n", b ^= c);
		printf("%X\n", b >>= c);
		printf("%X\n", b <<= c);

		if(c != 0){
			printf("%X\n", b %= c);
			printf("%X\n", b /= c);
		}

		/*
		TODO:  This generates compiler warnings
		printf("%d\n", b *= (int)c);
		printf("%d\n", b += (int)c);
		printf("%d\n", b -= (int)c);
		printf("%d\n", b &= (int)c);
		printf("%d\n", b |= (int)c);
		printf("%d\n", b ^= (int)c);
		printf("%d\n", b >>= (int)c);
		printf("%d\n", b <<= (int)c);

		if(c != 0){
			printf("%d\n", b /= (int)c);
			printf("%d\n", b %= (int)c);
		}
		*/
	}

	printf("%d %d\n", ((int)-97) / ((int)97), (int)-97);
	
	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++){


			printf("%X %X %X\n", array1[i], array2[j], array1[i] * array2[j]);
			if(array2[j] != 0){
				printf("%X\n", array2[j]);
			}
			printf("%X\n", array1[i] - array2[j]);
			printf("%X\n", array1[i] - array2[j]);
			printf("%X\n", array2[j]);
		}
	}

	printf("%X\n", return_signed_int());
	printf("%X\n", return_unsigned_int());
	printf("%X\n", coerce_return_to_unsigned());
	printf("%X\n", many_math());

	printf("%d\n", 2147483647);
	printf("%i\n", 2147483647);
	printf("%i\n", 1234);
	printf("%i\n", -1234);

	printf("%d", 1234);
	printf("\n");

	printf("%d", -1234);
	printf("\n");
	printf("%d", 1 * 4);
	printf("\n");
	printf("%d", 1 * 0x99);
	printf("\n");
	printf("%d", 1 * (int)0xFFFFFFFF);
	printf("\n");
	printf("%d", 0xFFFFFFFF * 0xFFFFFFFF);
	printf("\n");
	printf("%d", 0xFFFFFFFE * 0xFFFFFFFF);
	printf("\n");
	printf("%d", 0xFFFFFFFE * 0xFFFFFFFE);
	printf("\n");

	{
		enum food{u,r,s,t};
		putchar('0' + u);
	}

	{
		enum food{q,w,u,r,s,t};
		putchar('0' + u);
	}
	#include "../../builtin/includetest1.h"

	for(k = 0; k < num_sort; k++){
		sort_array[k] = get_input_space_partitioned_random();
		printf("%X\n", sort_array[k]);
	}
	unsigned_int_merge_sort(sort_array, num_sort, cmp_fcn);
	for(k = 0; k < num_sort; k++){
		printf("%X\n", sort_array[k]);
	}
	printf("Go through sorts:\n");
	for(k = 0; k < num_sort; k++){
		int index = unsigned_int_binary_search(sort_array, sort_array[k], (int)num_sort, cmp_fcn, unsigned_int_BINARY_SEARCH_RETURN_NEG_ONE);
		if(index != -1){
			printf("%X\n", sort_array[index]);
		}else{
			printf("not found\n");
		}
	}
	printf("Done\n");
	free(sort_array);
	{
		char cc = 'a';
		char dd = 'b';
		char ii = cc - dd;
		int iii = ii;
		printf("%d", iii);
	}
	return 0;
}
