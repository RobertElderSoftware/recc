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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../recc-implementation/math.h"

#define NUM_WORDS 100
#define NUM_WORDS_RAND_TEST 1000
#define BITS_PER_WORD 32

void do_count_test(struct math_machine_info *);
void do_unary_op_test(struct math_machine_info *);
void do_binary_op_test(struct math_machine_info *, unsigned int);
void do_demorgan_test(struct math_machine_info *);
void do_add_subtract_test(struct math_machine_info *);
void do_multiply_divide_test(struct math_machine_info *);

unsigned int get_rand_in_range_and_exclude(unsigned int mod, unsigned int * exclude_list, unsigned int * exclude_list_length, unsigned int);

void do_count_test(struct math_machine_info * mmi){
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned char uchrs[(NUM_WORDS * BITS_PER_WORD) / 8] = {0};
	signed char schrs[(NUM_WORDS * BITS_PER_WORD) / 8] = {0};
	unsigned short ushorts[(NUM_WORDS * BITS_PER_WORD) / 16] = {0};
	signed short sshorts[(NUM_WORDS * BITS_PER_WORD) / 16] = {0};

	for(i = 1; i < 800; i++){
		unsigned int bits_per_int = i;
		unsigned int variable_bit_ints[NUM_WORDS] = {0};
		printf("^^^^^^^^^^^^^^\n");
		for(j = 0; (j < ((NUM_WORDS * BITS_PER_WORD) / bits_per_int)) && (i > 16 || (j < (0x1u << i))); j++){
			unsigned int input = j;
			unsigned int offset_input = 0;
			unsigned int offset_output = (j * bits_per_int) % BITS_PER_WORD;
			unsigned int index = (j * bits_per_int) / BITS_PER_WORD;
			copy_abstract_integer(mmi, &input, &variable_bit_ints[index], offset_input, offset_output, BITS_PER_WORD, bits_per_int);
			if(i == 8){
				copy_abstract_integer(mmi, &input, &(((unsigned int *)&uchrs[0])[index]), offset_input, offset_output, BITS_PER_WORD, bits_per_int);
				copy_abstract_integer(mmi, &input, &(((unsigned int *)&schrs[0])[index]), offset_input, offset_output, BITS_PER_WORD, bits_per_int);
			}
			if(i == 16){
				copy_abstract_integer(mmi, &input, &(((unsigned int *)&ushorts[0])[index]), offset_input, offset_output, BITS_PER_WORD, bits_per_int);
				copy_abstract_integer(mmi, &input, &(((unsigned int *)&sshorts[0])[index]), offset_input, offset_output, BITS_PER_WORD, bits_per_int);
			}
			printf("integer index=%u offset_output=%u #bits per int=%u j=%u num is 0x", index, offset_output, i, j);
			print_abstract_integer(mmi, &variable_bit_ints[index], offset_output, bits_per_int);
			printf("\n");
		}
		printf("--------------\n");
		for(j = 0; j < NUM_WORDS; j++){
			printf("0x%08X, ",variable_bit_ints[j]);
		}
		if(i == 8){
			for(j = 0; j < ((NUM_WORDS * BITS_PER_WORD) / 8); j++){
				printf("uchrs[%u]= %u\n", j, uchrs[j]);
				printf("schrs[%u]= %d\n", j, schrs[j]);
			}
		}
		if(i == 16){
			for(j = 0; j < ((NUM_WORDS * BITS_PER_WORD) / 16); j++){
				printf("ushorts[%u]= %u\n", j, ushorts[j]);
				printf("sshorts[%u]= %d\n", j, sshorts[j]);
			}
		}
		printf("\n");
		printf("vvvvvvvvvvvvvv\n");
	}
}

void do_unary_op_test(struct math_machine_info * mmi){
	unsigned int max_bit_size = 300;
	unsigned int i = 0;
	unsigned int j = 0;

	srand(123u);
	
	for(i = 1; i < 1000; i++){
		unsigned int bits_per_int = (rand() % max_bit_size) + 1;
		unsigned int num_ints_that_fit = (NUM_WORDS_RAND_TEST * BITS_PER_WORD) / bits_per_int;
		unsigned int vbi[NUM_WORDS_RAND_TEST] = {0};
		unsigned int int1 = rand() % num_ints_that_fit;
		unsigned int int2 = rand() % num_ints_that_fit;
		unsigned int index1 = (int1 * bits_per_int) / BITS_PER_WORD;
		unsigned int index2 = (int2 * bits_per_int) / BITS_PER_WORD;
		unsigned int offset1 = (int1 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset2 = (int2 * bits_per_int) % BITS_PER_WORD;
		unsigned int op_type;

		/*  Set up some random integers*/
		for(j = 0; j < (NUM_WORDS_RAND_TEST * BITS_PER_WORD); j++){
			unsigned int word_index = j / BITS_PER_WORD;
			unsigned int offset = j % BITS_PER_WORD;
			vbi[word_index] |= (rand() % 2) << offset;
		}

		op_type = 0;
		switch(op_type){
			case 0:{
				printf("~");
				break;
			}default:{
				assert(0);
			}
		}
		printf("0x");
		print_abstract_integer(mmi, &vbi[index1], offset1, bits_per_int);

		printf(" = ");

		switch(op_type){
			case 0:{
				printf("0x");
				logical_not_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int);
				print_abstract_integer(mmi, &vbi[index2], offset2, bits_per_int);
				break;
			}default:{
				assert(0);
			}
		}
		printf("\n");
	}
}

void do_binary_op_test(struct math_machine_info * mmi, unsigned int b){
	unsigned int max_bit_size = 300;
	unsigned int i = 0;
	unsigned int j = 0;

	srand(123u);
	
	for(i = 1; i < 10000; i++){
		unsigned int exclude_list[10];/*  Up to 10...*/
		unsigned int exclude_list_length = 0;
		unsigned int bits_per_int = b == 0 ? (rand() % max_bit_size) + 1 : b;
		unsigned int num_ints_that_fit = (NUM_WORDS_RAND_TEST * BITS_PER_WORD) / bits_per_int;
		unsigned int vbi[NUM_WORDS_RAND_TEST] = {0};
		unsigned int tmp1 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int tmp2 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int tmp3 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int int1 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 0);
		unsigned int int2 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 0);
		unsigned int int3 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 0);
		unsigned int tmp1_index = (tmp1 * bits_per_int) / BITS_PER_WORD;
		unsigned int tmp2_index = (tmp2 * bits_per_int) / BITS_PER_WORD;
		unsigned int tmp3_index = (tmp3 * bits_per_int) / BITS_PER_WORD;
		unsigned int index1 = (int1 * bits_per_int) / BITS_PER_WORD;
		unsigned int index2 = (int2 * bits_per_int) / BITS_PER_WORD;
		unsigned int index3 = (int3 * bits_per_int) / BITS_PER_WORD;
		unsigned int tmp1_offset = (tmp1 * bits_per_int) % BITS_PER_WORD;
		unsigned int tmp2_offset = (tmp2 * bits_per_int) % BITS_PER_WORD;
		unsigned int tmp3_offset = (tmp3 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset1 = (int1 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset2 = (int2 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset3 = (int3 * bits_per_int) % BITS_PER_WORD;
		unsigned int shift_amount = rand() % (bits_per_int * 2);
		unsigned int op_type;

		/*  Set up some random integers*/
		for(j = 0; j < (NUM_WORDS_RAND_TEST * BITS_PER_WORD); j++){
			unsigned int word_index = j / BITS_PER_WORD;
			unsigned int offset = j % BITS_PER_WORD;
			vbi[word_index] |= (rand() % 2) << offset;
		}

		op_type = rand() % 12;
		printf("0x");
		print_abstract_integer(mmi, &vbi[index1], offset1, bits_per_int);
		switch(op_type){
			case 0:{
				printf(" | ");
				break;
			}case 1:{
				printf(" & ");
				break;
			}case 2:{
				printf(" == ");
				break;
			}case 3:{
				printf(" > ");
				break;
			}case 4:{
				printf(" < ");
				break;
			}case 5:{
				printf(" >> ");
				break;
			}case 6:{
				printf(" << ");
				break;
			}case 7:{
				printf(" * ");
				break;
			}case 8:{
				printf(" / ");
				break;
			}case 9:{
				printf(" %% ");
				break;
			}case 10:{
				printf(" + ");
				break;
			}case 11:{
				printf(" - ");
				break;
			}default:{
				assert(0);
			}
		}

		if(op_type == 5 || op_type == 6){
			printf("%u = ", shift_amount);
		}else{
			printf("0x");
			print_abstract_integer(mmi, &vbi[index2], offset2, bits_per_int);
			printf(" = ");
		}

		switch(op_type){
			case 0:{
				unsigned int word_result = vbi[index1] | vbi[index2];
				printf("0x");
				logical_or_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], offset1, offset2, offset3, bits_per_int);
				print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
				if(bits_per_int == mmi->bits_in_int){ assert(word_result == vbi[index3]); }
				break;
			}case 1:{
				unsigned int word_result = vbi[index1] & vbi[index2];
				printf("0x");
				logical_and_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], offset1, offset2, offset3, bits_per_int);
				print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
				if(bits_per_int == mmi->bits_in_int){ assert(word_result == vbi[index3]); }
				break;
			}case 2:{
				unsigned int word_result = vbi[index1] == vbi[index2];
				unsigned int rtn = eq_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int);
				printf("%u", rtn);
				if(bits_per_int == mmi->bits_in_int){ assert(word_result == rtn); }
				break;
			}case 3:{
				unsigned int word_result = vbi[index1] > vbi[index2];
				unsigned int rtn = gt_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int);
				printf("%u", rtn);
				if(bits_per_int == mmi->bits_in_int){ assert(word_result == rtn); }
				break;
			}case 4:{
				unsigned int word_result = vbi[index1] < vbi[index2];
				unsigned int rtn = lt_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int);
				printf("%u", rtn);
				if(bits_per_int == mmi->bits_in_int){ assert(word_result == rtn); }
				break;
			}case 5:{
				unsigned int word_result = vbi[index1] >> shift_amount;
				rshift_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int, shift_amount);
				print_abstract_integer(mmi, &vbi[index2], offset2, bits_per_int);
				if(bits_per_int == mmi->bits_in_int){ printf("\n%u %u %u %u\n", vbi[index1], shift_amount, word_result, vbi[index3]); assert(word_result == vbi[index2]); }
				break;
			}case 6:{
				unsigned int word_result = vbi[index1] << shift_amount;
				lshift_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int, shift_amount);
				print_abstract_integer(mmi, &vbi[index2], offset2, bits_per_int);
				if(bits_per_int == mmi->bits_in_int){ assert(word_result == vbi[index2]); }
				break;
			}case 7:{
				unsigned int word_result = vbi[index1] * vbi[index2];
				printf("0x");
				multiply_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], &vbi[tmp1_index], offset1, offset2, offset3, tmp1_offset, bits_per_int);
				print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
				if(bits_per_int == mmi->bits_in_int){ assert(word_result == vbi[index3]); }
				break;
			}case 8:{
				unsigned int word_result = vbi[index2] == 0 ? 0 : vbi[index1] / vbi[index2];
				unsigned int div_rtn;
				printf("0x");
				div_rtn = divide_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], &vbi[tmp1_index], &vbi[tmp2_index], &vbi[tmp3_index], offset1, offset2, offset3, tmp1_offset, tmp2_offset, tmp3_offset, bits_per_int);
				print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
				if(div_rtn){
				}else{
					if(bits_per_int == mmi->bits_in_int){ assert(word_result == vbi[index3]); }
				}
				break;
			}case 9:{
				unsigned int word_result = vbi[index2] == 0 ? 0 : vbi[index1] % vbi[index2];
				unsigned int mod_rtn;
				printf("0x");
				mod_rtn = modulo_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], &vbi[tmp1_index], &vbi[tmp2_index], &vbi[tmp3_index], offset1, offset2, offset3, tmp1_offset, tmp2_offset, tmp3_offset, bits_per_int);
				print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
				if(mod_rtn){
				}else{
					if(bits_per_int == mmi->bits_in_int){ assert(word_result == vbi[index3]); }
				}
				break;
			}case 10:{
				unsigned int word_result = vbi[index1] + vbi[index2];
				printf("0x");
				add_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], offset1, offset2, offset3, bits_per_int);
				print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
				if(bits_per_int == mmi->bits_in_int){ assert(word_result == vbi[index3]); }
				break;
			}case 11:{
				unsigned int word_result = vbi[index1] - vbi[index2];
				printf("0x");
				subtract_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], &vbi[tmp1_index], offset1, offset2, offset3, tmp1_offset, bits_per_int);
				print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
				if(bits_per_int == mmi->bits_in_int){ printf("\n%u %u\n", word_result, vbi[index3]); assert(word_result == vbi[index3]); }
				break;
			}default:{
				assert(0);
			}
		}
		printf("\n");
	}
}

unsigned int list_contains(unsigned int x, unsigned int * exclude_list, unsigned int exclude_list_length);
unsigned int list_contains(unsigned int x, unsigned int * exclude_list, unsigned int exclude_list_length){
	unsigned int i;
	for(i = 0; i < exclude_list_length; i++){
		if(exclude_list[i] == x){
			return 1;
		}
	}
	return 0;
}

unsigned int get_rand_in_range_and_exclude(unsigned int mod, unsigned int * exclude_list, unsigned int * exclude_list_length, unsigned int add_exclusion){
	/*  Get a random number we haven't already used. */
	/*  Don't let mod >= exclude_list_length. */
	unsigned int candidate;
	do{
		candidate = rand() % mod;
	}while(list_contains(candidate, exclude_list, *exclude_list_length));
	if(add_exclusion){
		/*  Add to list. */
		exclude_list[*exclude_list_length] = candidate;
		*exclude_list_length = *exclude_list_length + 1;
	}
	return candidate;
}

void do_demorgan_test(struct math_machine_info * mmi){
	unsigned int max_bit_size = 300;
	unsigned int i = 0;
	unsigned int j = 0;

	printf("Begin do_demorgan_test.\n");
	srand(123u);
	
	for(i = 1; i < 1000; i++){
		unsigned int exclude_list[10];/*  Up to 10...*/
		unsigned int exclude_list_length = 0;
		unsigned int bits_per_int = (rand() % max_bit_size) + 1;
		unsigned int num_ints_that_fit = (NUM_WORDS_RAND_TEST * BITS_PER_WORD) / bits_per_int;
		unsigned int vbi[NUM_WORDS_RAND_TEST] = {0};
		unsigned int int1 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int int2 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int int3 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int int4 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int index1 = (int1 * bits_per_int) / BITS_PER_WORD;
		unsigned int index2 = (int2 * bits_per_int) / BITS_PER_WORD;
		unsigned int index3 = (int3 * bits_per_int) / BITS_PER_WORD;
		unsigned int index4 = (int4 * bits_per_int) / BITS_PER_WORD;
		unsigned int offset1 = (int1 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset2 = (int2 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset3 = (int3 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset4 = (int4 * bits_per_int) % BITS_PER_WORD;
		unsigned int rtn;

		/*  Set up some random integers. */
		for(j = 0; j < (NUM_WORDS_RAND_TEST * BITS_PER_WORD); j++){
			unsigned int word_index = j / BITS_PER_WORD;
			unsigned int offset = j % BITS_PER_WORD;
			vbi[word_index] |= (rand() % 2) << offset;
		}

		/* Now, compute ~(A | B) = ~A & ~B */


		/* Now, compute ~(A | B) */


		/*
		printf("A = 0x");
		print_abstract_integer(&vbi[index1], offset1, bits_per_int);
		printf("\n");
		*/

		/*
		printf("B = 0x");
		print_abstract_integer(&vbi[index2], offset2, bits_per_int);
		printf("\n");
		*/

		logical_or_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], offset1, offset2, offset3, bits_per_int);

		/*
		printf("A | B = 0x");
		print_abstract_integer(&vbi[index3], offset3, bits_per_int);
		printf("\n");
		*/

		logical_not_abstract_integer(mmi, &vbi[index3], &vbi[index3], offset3, offset3, bits_per_int);

		/*
		printf("~(A | B) = 0x");
		print_abstract_integer(&vbi[index3], offset3, bits_per_int);
		printf("\n");
		*/

		/* And ~A & ~B */
		logical_not_abstract_integer(mmi, &vbi[index1], &vbi[index1], offset1, offset1, bits_per_int);

		/*
		printf("~A = 0x");
		print_abstract_integer(&vbi[index1], offset1, bits_per_int);
		printf("\n");
		*/

		logical_not_abstract_integer(mmi, &vbi[index2], &vbi[index2], offset2, offset2, bits_per_int);

		/*
		printf("~B = 0x");
		print_abstract_integer(&vbi[index2], offset2, bits_per_int);
		printf("\n");
		*/

		logical_and_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index4], offset1, offset2, offset4, bits_per_int);

		/*
		printf("~A & ~B = 0x");
		print_abstract_integer(&vbi[index4], offset4, bits_per_int);
		printf("\n");
		*/

		rtn = eq_abstract_integer(mmi, &vbi[index3], &vbi[index4], offset3, offset4, bits_per_int);
		printf("%u", rtn);
		assert(rtn);
	}
	printf("\nComplete do_demorgan_test.\n");
}

void do_add_subtract_test(struct math_machine_info * mmi){
	unsigned int max_bit_size = 300;
	unsigned int i = 0;
	unsigned int j = 0;

	printf("Begin do_add_subtract_test.\n");
	srand(123u);
	
	for(i = 1; i < 10000; i++){
		unsigned int exclude_list[10];/*  Up to 10...*/
		unsigned int exclude_list_length = 0;
		unsigned int bits_per_int = (rand() % max_bit_size) + 1;
		unsigned int num_ints_that_fit = (NUM_WORDS_RAND_TEST * BITS_PER_WORD) / bits_per_int;
		unsigned int vbi[NUM_WORDS_RAND_TEST] = {0};
		unsigned int tmp = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int int1 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 0);
		unsigned int int2 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 0);
		unsigned int int3 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 0);
		unsigned int tmp_index = (tmp * bits_per_int) / BITS_PER_WORD;
		unsigned int index1 = (int1 * bits_per_int) / BITS_PER_WORD;
		unsigned int index2 = (int2 * bits_per_int) / BITS_PER_WORD;
		unsigned int index3 = (int3 * bits_per_int) / BITS_PER_WORD;
		unsigned int tmp_offset = (tmp * bits_per_int) % BITS_PER_WORD;
		unsigned int offset1 = (int1 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset2 = (int2 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset3 = (int3 * bits_per_int) % BITS_PER_WORD;
		unsigned int rtn;
		unsigned int a_gt_b;
		unsigned int a_lt_b;
		unsigned int a_eq_b;

		unsigned int add_or_subtract = rand() % 2;
		/*  Set up some random integers. */
		for(j = 0; j < (NUM_WORDS_RAND_TEST * BITS_PER_WORD); j++){
			unsigned int word_index = j / BITS_PER_WORD;
			unsigned int offset = j % BITS_PER_WORD;
			vbi[word_index] |= (rand() % 2) << offset;
		}

		printf("(bpi=%u) 0x", bits_per_int);
		print_abstract_integer(mmi, &vbi[index1], offset1, bits_per_int);

		if(add_or_subtract){
			printf(" + ");
		}else{
			printf(" - ");
		}

		printf("0x");
		print_abstract_integer(mmi, &vbi[index2], offset2, bits_per_int);
		printf(" = ");

		a_gt_b = gt_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int);
		a_lt_b = lt_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int);
		a_eq_b = eq_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int);

		if(add_or_subtract){
			rtn = add_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], offset1, offset2, offset3, bits_per_int);
		}else{
			rtn = subtract_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], &vbi[tmp_index], offset1, offset2, offset3, tmp_offset, bits_per_int);
			if(a_gt_b){
				assert(!rtn); /*  Should be no underflow if a > b */
			}
			if(a_lt_b){
				fflush(stdout);
				assert(rtn); /*  Should be underflow if a > b */
			}
			if(a_eq_b){
				assert(!rtn); /*  Should be no underflow if a == b */
			}
		}

		printf("0x");
		print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
		printf(" ");

		printf("Overflow: %u\n", rtn);
	}
	printf("\nComplete do_add_subtract_test.\n");
}

void do_multiply_divide_test(struct math_machine_info * mmi){
	unsigned int max_bit_size = 300;
	unsigned int i = 0;
	unsigned int j = 0;

	printf("Begin do_multiply_divide_test.\n");
	srand(123u);
	
	for(i = 1; i < 1000; i++){
		unsigned int exclude_list[10];/*  Up to 10...*/
		unsigned int exclude_list_length = 0;
		unsigned int bits_per_int = (rand() % max_bit_size) + 1;
		unsigned int num_ints_that_fit = (NUM_WORDS_RAND_TEST * BITS_PER_WORD) / bits_per_int;
		unsigned int vbi[NUM_WORDS_RAND_TEST] = {0};
		unsigned int tmp1 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int tmp2 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int tmp3 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 1);
		unsigned int int1 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 0);
		unsigned int int2 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 0);
		unsigned int int3 = get_rand_in_range_and_exclude(num_ints_that_fit, &exclude_list[0], &exclude_list_length, 0);
		unsigned int tmp1_index = (tmp1 * bits_per_int) / BITS_PER_WORD;
		unsigned int tmp2_index = (tmp2 * bits_per_int) / BITS_PER_WORD;
		unsigned int tmp3_index = (tmp3 * bits_per_int) / BITS_PER_WORD;
		unsigned int index1 = (int1 * bits_per_int) / BITS_PER_WORD;
		unsigned int index2 = (int2 * bits_per_int) / BITS_PER_WORD;
		unsigned int index3 = (int3 * bits_per_int) / BITS_PER_WORD;
		unsigned int tmp1_offset = (tmp1 * bits_per_int) % BITS_PER_WORD;
		unsigned int tmp2_offset = (tmp2 * bits_per_int) % BITS_PER_WORD;
		unsigned int tmp3_offset = (tmp3 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset1 = (int1 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset2 = (int2 * bits_per_int) % BITS_PER_WORD;
		unsigned int offset3 = (int3 * bits_per_int) % BITS_PER_WORD;
		unsigned int rtn;
		unsigned int multiply_or_divide = rand() % 3;


		unsigned int a_gt_b;
		unsigned int a_lt_b;
		unsigned int a_eq_b;

		/*  Set up some random integers. */
		for(j = 0; j < (NUM_WORDS_RAND_TEST * BITS_PER_WORD); j++){
			unsigned int word_index = j / BITS_PER_WORD;
			unsigned int offset = j % BITS_PER_WORD;
			vbi[word_index] |= (rand() % 2) << offset;
		}

		/*  Randomly divide the integers by a bit so all the calculations don't overflow. */
		for(j = 0; j < num_ints_that_fit; j++){
			unsigned int word_index = (j * bits_per_int) / BITS_PER_WORD;
			unsigned int bit_offset = (j * bits_per_int) % BITS_PER_WORD;
			unsigned int shift_amount = (rand() % bits_per_int) + 1;
			rshift_abstract_integer(mmi, &vbi[word_index], &vbi[word_index], bit_offset, bit_offset, bits_per_int, shift_amount);
		}

		printf("(bpi=%u) 0x", bits_per_int);
		print_abstract_integer(mmi, &vbi[index1], offset1, bits_per_int);

		if(multiply_or_divide == 0){
			printf(" / ");
		}else if(multiply_or_divide == 1){
			printf(" %% ");
		}else{
			printf(" * ");
		}

		printf("0x");
		print_abstract_integer(mmi, &vbi[index2], offset2, bits_per_int);
		printf(" = ");

		a_gt_b = gt_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int);
		a_lt_b = lt_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int);
		a_eq_b = eq_abstract_integer(mmi, &vbi[index1], &vbi[index2], offset1, offset2, bits_per_int);

		if(multiply_or_divide == 0){
			rtn = divide_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], &vbi[tmp1_index], &vbi[tmp2_index], &vbi[tmp3_index], offset1, offset2, offset3, tmp1_offset, tmp2_offset, tmp3_offset, bits_per_int);
			printf("0x");
			print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
			printf(" ");
			printf("Division by zero: %u\n", rtn);
			zero_abstract_integer(mmi, &vbi[tmp1_index], tmp1_offset, bits_per_int);
			if(a_gt_b){
				/*  It should have divided at least once. */
				assert(gt_abstract_integer(mmi, &vbi[index3], &vbi[tmp1_index], offset3, tmp1_offset, bits_per_int));
			}
			if(a_lt_b){
				/*  It should not divide at all. */
				assert(eq_abstract_integer(mmi, &vbi[index3], &vbi[tmp1_index], offset3, tmp1_offset, bits_per_int));
			}
			if(a_eq_b){
				/*  It should at least once. */
				assert(gt_abstract_integer(mmi, &vbi[index3], &vbi[tmp1_index], offset3, tmp1_offset, bits_per_int));
			}
		}else if(multiply_or_divide == 1){
			rtn = modulo_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], &vbi[tmp1_index], &vbi[tmp2_index], &vbi[tmp3_index], offset1, offset2, offset3, tmp1_offset, tmp2_offset, tmp3_offset, bits_per_int);
			printf("0x");
			print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
			printf(" ");
			printf("Modulo return: %u\n", rtn);
		}else{
			rtn = multiply_abstract_integer(mmi, &vbi[index1], &vbi[index2], &vbi[index3], &vbi[tmp1_index], offset1, offset2, offset3, tmp3_offset, bits_per_int);
			printf("0x");
			print_abstract_integer(mmi, &vbi[index3], offset3, bits_per_int);
			printf(" ");
			printf("Overflow: %u\n", rtn);
		}
	}
	printf("\nComplete do_multiply_divide_test.\n");
}

int main(void){
	struct math_machine_info mmi;
	mmi.type = MATH_LITTLE_ENDIAN;
	mmi.bits_in_int = 32;

	do_count_test(&mmi);
	do_unary_op_test(&mmi);
	do_binary_op_test(&mmi, mmi.bits_in_int);
	do_binary_op_test(&mmi, 0);
	do_demorgan_test(&mmi);
	do_add_subtract_test(&mmi);
	do_multiply_divide_test(&mmi);
	return 0;
}
