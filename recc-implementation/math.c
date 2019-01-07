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

#include "math.h"
#include <stdio.h>

void zero_abstract_integer(struct math_machine_info * mmi, unsigned int * in, unsigned int in_offset_bits, unsigned int int_size){
	unsigned int i;
	unsigned int * current_word_in = in;
	unsigned int current_offset_in = in_offset_bits;

	for(i = 0; i < int_size; i++){
		current_word_in = &in[(in_offset_bits + i) / mmi->bits_in_int];
		current_offset_in = (in_offset_bits + i) % mmi->bits_in_int;
		(*current_word_in) &= ~((0x1u) << current_offset_in);
	}
}

unsigned int lt_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * in_b, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int int_size){
	unsigned int i;
	unsigned int * current_word_in_a = in_a;
	unsigned int * current_word_in_b = in_b;
	unsigned int current_offset_in_a = in_a_offset_bits;
	unsigned int current_offset_in_b = in_b_offset_bits;

	for(i = int_size; i-- > 0;){
		unsigned int current_read_bit_a;
		unsigned int current_read_bit_b;

		current_word_in_a = &in_a[(in_a_offset_bits + i) / mmi->bits_in_int];
		current_word_in_b = &in_b[(in_b_offset_bits + i) / mmi->bits_in_int];
		current_offset_in_a = (in_a_offset_bits + i) % mmi->bits_in_int;
		current_offset_in_b = (in_b_offset_bits + i) % mmi->bits_in_int;

		current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;
		current_read_bit_b = (*current_word_in_b & ((0x1u) << current_offset_in_b)) >> current_offset_in_b;

		if(current_read_bit_a < current_read_bit_b){
			return 1;
		}else if(current_read_bit_a == current_read_bit_b){
			/*  Keep looking... */
		}else{
			return 0;
		}
	}
	return 0;
}

unsigned int gt_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * in_b, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int int_size){
	unsigned int i;
	unsigned int * current_word_in_a = in_a;
	unsigned int * current_word_in_b = in_b;
	unsigned int current_offset_in_a = in_a_offset_bits;
	unsigned int current_offset_in_b = in_b_offset_bits;

	for(i = int_size; i-- > 0;){
		unsigned int current_read_bit_a;
		unsigned int current_read_bit_b;

		current_word_in_a = &in_a[(in_a_offset_bits + i) / mmi->bits_in_int];
		current_word_in_b = &in_b[(in_b_offset_bits + i) / mmi->bits_in_int];
		current_offset_in_a = (in_a_offset_bits + i) % mmi->bits_in_int;
		current_offset_in_b = (in_b_offset_bits + i) % mmi->bits_in_int;

		current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;
		current_read_bit_b = (*current_word_in_b & ((0x1u) << current_offset_in_b)) >> current_offset_in_b;

		if(current_read_bit_a > current_read_bit_b){
			return 1;
		}else if(current_read_bit_a == current_read_bit_b){
			/*  Keep looking... */
		}else{
			return 0;
		}
	}
	return 0;
}

unsigned int eq_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * in_b, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int int_size){
	unsigned int i;
	unsigned int * current_word_in_a = in_a;
	unsigned int * current_word_in_b = in_b;
	unsigned int current_offset_in_a = in_a_offset_bits;
	unsigned int current_offset_in_b = in_b_offset_bits;
	for(i = 0; i < int_size; i++){
		unsigned int current_read_bit_a;
		unsigned int current_read_bit_b;

		current_word_in_a = &in_a[(in_a_offset_bits + i) / mmi->bits_in_int];
		current_word_in_b = &in_b[(in_b_offset_bits + i) / mmi->bits_in_int];
		current_offset_in_a = (in_a_offset_bits + i) % mmi->bits_in_int;
		current_offset_in_b = (in_b_offset_bits + i) % mmi->bits_in_int;

		current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;
		current_read_bit_b = (*current_word_in_b & ((0x1u) << current_offset_in_b)) >> current_offset_in_b;

		current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;
		current_read_bit_b = (*current_word_in_b & ((0x1u) << current_offset_in_b)) >> current_offset_in_b;

		if(current_read_bit_a != current_read_bit_b){
			return 0;
		}
	}
	return 1;
}

void lshift_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * out, unsigned int in_a_offset_bits, unsigned int out_offset_bits, unsigned int int_size, unsigned int shift_offset){
	unsigned int i;
	unsigned int * current_word_in_a = in_a;
	unsigned int * current_word_out = out;
	unsigned int current_offset_in_a = in_a_offset_bits;
	unsigned int current_offset_out = out_offset_bits;
	if(shift_offset >= int_size){
		shift_offset = shift_offset % int_size;
	}
	for(i = int_size; i-- > 0;){
		unsigned int current_read_bit_a;
		current_word_out = &out[(out_offset_bits + i) / mmi->bits_in_int];
		current_offset_out = (out_offset_bits + i) % mmi->bits_in_int;

		/*  Clear the bit that might be getting set. */
		(*current_word_out) &= ~((0x1u) << current_offset_out);

		if(i < shift_offset){
		}else{
			current_word_in_a = &in_a[(in_a_offset_bits + i - shift_offset) / mmi->bits_in_int];
			current_offset_in_a = (in_a_offset_bits + i - shift_offset) % mmi->bits_in_int;
                        
			current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;

			(*current_word_out) |= (current_read_bit_a << current_offset_out);
		}
	}
}

void rshift_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * out, unsigned int in_a_offset_bits, unsigned int out_offset_bits, unsigned int int_size, unsigned int shift_offset){
	unsigned int i;
	unsigned int * current_word_in_a = in_a;
	unsigned int * current_word_out = out;
	unsigned int current_offset_in_a = in_a_offset_bits;
	unsigned int current_offset_out = out_offset_bits;
	if(shift_offset >= int_size){
		shift_offset = shift_offset % int_size;
	}
	for(i = 0; i < int_size; i++){
		unsigned int current_read_bit_a;
		current_word_out = &out[(out_offset_bits + i) / mmi->bits_in_int];
		current_offset_out = (out_offset_bits + i) % mmi->bits_in_int;

		/*  Clear the bit that might be getting set. */
		(*current_word_out) &= ~((0x1u) << current_offset_out);

		if((i + shift_offset) < int_size){
			current_word_in_a = &in_a[(in_a_offset_bits + i + shift_offset) / mmi->bits_in_int];
			current_offset_in_a = (in_a_offset_bits + i + shift_offset) % mmi->bits_in_int;
                        
			current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;

			(*current_word_out) |= (current_read_bit_a << current_offset_out);
		}
	}
}

void logical_or_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * in_b, unsigned int * out, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int out_offset_bits, unsigned int int_size){
	unsigned int i;
	unsigned int * current_word_in_a = in_a;
	unsigned int * current_word_in_b = in_b;
	unsigned int * current_word_out = out;
	unsigned int current_offset_in_a = in_a_offset_bits;
	unsigned int current_offset_in_b = in_b_offset_bits;
	unsigned int current_offset_out = out_offset_bits;
	for(i = 0; i < int_size; i++){
		unsigned int current_read_bit_a;
		unsigned int current_read_bit_b;

		current_word_in_a = &in_a[(in_a_offset_bits + i) / mmi->bits_in_int];
		current_word_in_b = &in_b[(in_b_offset_bits + i) / mmi->bits_in_int];
		current_word_out = &out[(out_offset_bits + i) / mmi->bits_in_int];
		current_offset_in_a = (in_a_offset_bits + i) % mmi->bits_in_int;
		current_offset_in_b = (in_b_offset_bits + i) % mmi->bits_in_int;
		current_offset_out = (out_offset_bits + i) % mmi->bits_in_int;

		current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;
		current_read_bit_b = (*current_word_in_b & ((0x1u) << current_offset_in_b)) >> current_offset_in_b;

		/*  Clear the bit that might be getting set. */
		(*current_word_out) &= ~((0x1u) << current_offset_out);
		/*  Set the bit if it needs to be set.  */
		/*printf("Going to set bit %u to %u\n", current_offset_out, !current_read_bit);*/
		(*current_word_out) |= ((current_read_bit_a | current_read_bit_b) << current_offset_out);
	}
}

void logical_and_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * in_b, unsigned int * out, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int out_offset_bits, unsigned int int_size){
	unsigned int i;
	unsigned int * current_word_in_a = in_a;
	unsigned int * current_word_in_b = in_b;
	unsigned int * current_word_out = out;
	unsigned int current_offset_in_a = in_a_offset_bits;
	unsigned int current_offset_in_b = in_b_offset_bits;
	unsigned int current_offset_out = out_offset_bits;
	for(i = 0; i < int_size; i++){
		unsigned int current_read_bit_a;
		unsigned int current_read_bit_b;

		current_word_in_a = &in_a[(in_a_offset_bits + i) / mmi->bits_in_int];
		current_word_in_b = &in_b[(in_b_offset_bits + i) / mmi->bits_in_int];
		current_word_out = &out[(out_offset_bits + i) / mmi->bits_in_int];
		current_offset_in_a = (in_a_offset_bits + i) % mmi->bits_in_int;
		current_offset_in_b = (in_b_offset_bits + i) % mmi->bits_in_int;
		current_offset_out = (out_offset_bits + i) % mmi->bits_in_int;

		current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;
		current_read_bit_b = (*current_word_in_b & ((0x1u) << current_offset_in_b)) >> current_offset_in_b;

		/*  Clear the bit that might be getting set. */
		(*current_word_out) &= ~((0x1u) << current_offset_out);
		/*  Set the bit if it needs to be set.  */
		/*printf("Going to set bit %u to %u\n", current_offset_out, !current_read_bit);*/
		(*current_word_out) |= ((current_read_bit_a & current_read_bit_b) << current_offset_out);
	}
}

unsigned int increment_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * out, unsigned int in_a_offset_bits, unsigned int out_offset_bits, unsigned int int_size, unsigned int starting_bit){
	unsigned int i;
	unsigned int * current_word_in_a = in_a;
	unsigned int * current_word_out = out;
	unsigned int current_offset_in_a = in_a_offset_bits;
	unsigned int current_offset_out = out_offset_bits;
	unsigned int ha1_out = 0;
	unsigned int ha1_carry = 0;
	unsigned int ha2_out = 0;
	unsigned int ha2_carry = 1; /*  Assume that there is already a carry in before addition starts. */
	if(starting_bit > int_size){
		return 1;
	}
	for(i = starting_bit; i < int_size; i++){
		unsigned int current_read_bit_a;
		unsigned int current_read_bit_b;

		current_word_in_a = &in_a[(in_a_offset_bits + i) / mmi->bits_in_int];
		current_word_out = &out[(out_offset_bits + i) / mmi->bits_in_int];
		current_offset_in_a = (in_a_offset_bits + i) % mmi->bits_in_int;
		current_offset_out = (out_offset_bits + i) % mmi->bits_in_int;

		current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;
		current_read_bit_b = 0;

		/*  Clear the bit that might be getting set. */
		(*current_word_out) &= ~((0x1u) << current_offset_out);
		/*  Set the bit if it needs to be set.  */
		/*  The sum consists of two half adders: */
		ha1_out = current_read_bit_a ^ current_read_bit_b;
		ha1_carry = current_read_bit_a & current_read_bit_b;
		ha2_out = ha1_out ^ ha2_carry;
		ha2_carry = ha1_carry | (ha2_carry & ha1_out);
		(*current_word_out) |= ((ha2_out) << current_offset_out);
	}
	return ha2_carry;
}

unsigned int subtract_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * in_b, unsigned int * out, unsigned int * tmp, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int out_offset_bits, unsigned int tmp_offset_bits, unsigned int int_size){
	unsigned int inc_return1;
	unsigned int add_return;
	/*  Calculate the complement, and use addition instead: */
	logical_not_abstract_integer(mmi, in_b, tmp, in_b_offset_bits, tmp_offset_bits, int_size);
	/*  Increment only overflows when the subtrahend is all 0s, so discard overflow flag  */
	inc_return1 = increment_abstract_integer(mmi, tmp, tmp, tmp_offset_bits, tmp_offset_bits, int_size, 0);
	add_return = add_abstract_integer(mmi, in_a, tmp, out, in_a_offset_bits, tmp_offset_bits, out_offset_bits, int_size);
	if(!add_return){ /*  Result is a negative number, return the complement of the magnitude. */
		/*  To be consistent with C, return the complement, but we can do this to get the value:
		logical_not_abstract_integer(mmi, out, out, out_offset_bits, out_offset_bits, int_size);
		increment_abstract_integer(mmi, out, out, out_offset_bits, out_offset_bits, int_size, 0);*/
		if(inc_return1){ /*  If inc1 was overflow, we were subtracting zero for sure. */
			return !inc_return1;
		}else{
			return 1; /*  if !add_return, then there was underflow. */
		}
	}else{
		return 0;
	}
}

unsigned int add_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * in_b, unsigned int * out, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int out_offset_bits, unsigned int int_size){
	unsigned int i;
	unsigned int * current_word_in_a = in_a;
	unsigned int * current_word_in_b = in_b;
	unsigned int * current_word_out = out;
	unsigned int current_offset_in_a = in_a_offset_bits;
	unsigned int current_offset_in_b = in_b_offset_bits;
	unsigned int current_offset_out = out_offset_bits;
	unsigned int ha1_out = 0;
	unsigned int ha1_carry = 0;
	unsigned int ha2_out = 0;
	unsigned int ha2_carry = 0;
	for(i = 0; i < int_size; i++){
		unsigned int current_read_bit_a;
		unsigned int current_read_bit_b;

		current_word_in_a = &in_a[(in_a_offset_bits + i) / mmi->bits_in_int];
		current_word_in_b = &in_b[(in_b_offset_bits + i) / mmi->bits_in_int];
		current_word_out = &out[(out_offset_bits + i) / mmi->bits_in_int];
		current_offset_in_a = (in_a_offset_bits + i) % mmi->bits_in_int;
		current_offset_in_b = (in_b_offset_bits + i) % mmi->bits_in_int;
		current_offset_out = (out_offset_bits + i) % mmi->bits_in_int;

		current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;
		current_read_bit_b = (*current_word_in_b & ((0x1u) << current_offset_in_b)) >> current_offset_in_b;

		/*  Clear the bit that might be getting set. */
		(*current_word_out) &= ~((0x1u) << current_offset_out);
		/*  Set the bit if it needs to be set.  */
		/*  The sum consists of two half adders: */
		ha1_out = current_read_bit_a ^ current_read_bit_b;
		ha1_carry = current_read_bit_a & current_read_bit_b;
		ha2_out = ha1_out ^ ha2_carry;
		ha2_carry = ha1_carry | (ha2_carry & ha1_out);
		(*current_word_out) |= ((ha2_out) << current_offset_out);

	}
	return ha2_carry;
}

unsigned int modulo_abstract_integer(struct math_machine_info * mmi, unsigned int * numerator, unsigned int * denominator, unsigned int * out, unsigned int * partial_remainder, unsigned int * sub_tmp, unsigned int * partial_result, unsigned int numerator_offset_bits, unsigned int denominator_offset_bits, unsigned int out_offset_bits, unsigned int partial_remainder_offset_bits, unsigned int sub_tmp_offset_bits, unsigned int partial_result_offset_bits, unsigned int int_size){
	return divide_and_modulo_abstract_integer(mmi, numerator, denominator, out, partial_remainder, sub_tmp, partial_result, numerator_offset_bits, denominator_offset_bits, out_offset_bits, partial_remainder_offset_bits, sub_tmp_offset_bits, partial_result_offset_bits, int_size, 1);
}

unsigned int divide_abstract_integer(struct math_machine_info * mmi, unsigned int * numerator, unsigned int * denominator, unsigned int * out, unsigned int * partial_remainder, unsigned int * sub_tmp, unsigned int * partial_result, unsigned int numerator_offset_bits, unsigned int denominator_offset_bits, unsigned int out_offset_bits, unsigned int partial_remainder_offset_bits, unsigned int sub_tmp_offset_bits, unsigned int partial_result_offset_bits, unsigned int int_size){
	return divide_and_modulo_abstract_integer(mmi, numerator, denominator, out, partial_remainder, sub_tmp, partial_result, numerator_offset_bits, denominator_offset_bits, out_offset_bits, partial_remainder_offset_bits, sub_tmp_offset_bits, partial_result_offset_bits, int_size, 0);
	
}

unsigned int divide_and_modulo_abstract_integer(struct math_machine_info * mmi, unsigned int * numerator, unsigned int * denominator, unsigned int * out, unsigned int * partial_remainder, unsigned int * sub_tmp, unsigned int * partial_result, unsigned int numerator_offset_bits, unsigned int denominator_offset_bits, unsigned int out_offset_bits, unsigned int partial_remainder_offset_bits, unsigned int sub_tmp_offset_bits, unsigned int partial_result_offset_bits, unsigned int int_size, unsigned int divide_or_modulo){
	/*  This function implements long division:  */
	/*            ______________________________ */
	/*  ex.  1010 ) 101110101010101010100010111  */
	/*                                           */
	unsigned int i;
	unsigned int * current_word_numerator = numerator;
	unsigned int current_offset_numerator = numerator_offset_bits;
	zero_abstract_integer(mmi, partial_result, partial_result_offset_bits, int_size);
	zero_abstract_integer(mmi, partial_remainder, partial_remainder_offset_bits, int_size);
	if(gt_abstract_integer(mmi, denominator, partial_remainder, denominator_offset_bits, partial_remainder_offset_bits, int_size)){
		for(i = int_size; i-- > 0;){
			unsigned int current_read_bit_numerator;

			lshift_abstract_integer(mmi, partial_remainder, partial_remainder, partial_remainder_offset_bits, partial_remainder_offset_bits, int_size, 1);
			current_word_numerator = &numerator[(numerator_offset_bits + i) / mmi->bits_in_int];
			current_offset_numerator = (numerator_offset_bits + i) % mmi->bits_in_int;

			/*  Extract the next digit from the numerator and add it to our 'partial_remainder': */
			current_read_bit_numerator = (*current_word_numerator & ((0x1u) << current_offset_numerator)) >> current_offset_numerator;
			/*  If it was non-zero make it the next digit of our 'partial_remainder' numerator. */
			if(current_read_bit_numerator){
				increment_abstract_integer(mmi, partial_remainder, partial_remainder, partial_remainder_offset_bits, partial_remainder_offset_bits, int_size, 0);
			}

			if(!gt_abstract_integer(mmi, denominator, partial_remainder, denominator_offset_bits, partial_remainder_offset_bits, int_size)){
				/*  Therefore, it would divide at least once, so subtract the result so far: */
				subtract_abstract_integer(mmi, partial_remainder, denominator, partial_remainder, sub_tmp, partial_remainder_offset_bits, denominator_offset_bits, partial_remainder_offset_bits, sub_tmp_offset_bits, int_size);
				increment_abstract_integer(mmi, partial_result, partial_result, partial_result_offset_bits, partial_result_offset_bits, int_size, i);
			}

		}
		if(divide_or_modulo){
			/*  Modulo */
			copy_abstract_integer(mmi, partial_remainder, out, partial_remainder_offset_bits, out_offset_bits, int_size, int_size);
		}else{
			/*  Divide */
			copy_abstract_integer(mmi, partial_result, out, partial_result_offset_bits, out_offset_bits, int_size, int_size);
		}
		return 0;
	}else{
		/*  Division by zero, make the result as large as possible.  */
		logical_not_abstract_integer(mmi, partial_result, partial_result, partial_result_offset_bits, partial_result_offset_bits, int_size);
		if(divide_or_modulo){
			/*  Modulo */
			zero_abstract_integer(mmi, out, out_offset_bits, int_size);
		}else{
			/*  Divide */
			copy_abstract_integer(mmi, partial_result, out, partial_result_offset_bits, out_offset_bits, int_size, int_size);
		}
		return 1;
	}
}

unsigned int multiply_abstract_integer(struct math_machine_info * mmi, unsigned int * in_a, unsigned int * in_b, unsigned int * out, unsigned int * partial_result, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int out_offset_bits, unsigned int partial_result_offset_bits, unsigned int int_size){
	unsigned int i;
	unsigned int j;
	unsigned int * current_word_in_a = in_a;
	unsigned int * current_word_in_b = in_b;
	unsigned int current_offset_in_a = in_a_offset_bits;
	unsigned int current_offset_in_b = in_b_offset_bits;
	unsigned int has_overflow = 0;
	zero_abstract_integer(mmi, partial_result, partial_result_offset_bits, int_size);
	for(i = 0; i < int_size; i++){
		unsigned int current_read_bit_a;
		current_offset_in_a = (in_a_offset_bits + i) % mmi->bits_in_int;
		current_word_in_a = &in_a[(in_a_offset_bits + i) / mmi->bits_in_int];
		current_read_bit_a = (*current_word_in_a & ((0x1u) << current_offset_in_a)) >> current_offset_in_a;
		for(j = 0; j < int_size; j++){
			unsigned int current_read_bit_b;
			current_word_in_b = &in_b[(in_b_offset_bits + j) / mmi->bits_in_int];
			current_offset_in_b = (in_b_offset_bits + j) % mmi->bits_in_int;
			current_read_bit_b = (*current_word_in_b & ((0x1u) << current_offset_in_b)) >> current_offset_in_b;
                        
			if(current_read_bit_a && current_read_bit_b){
				has_overflow += increment_abstract_integer(mmi, partial_result, partial_result, partial_result_offset_bits, partial_result_offset_bits, int_size, i + j);
			}
		}

	}
	copy_abstract_integer(mmi, partial_result, out, partial_result_offset_bits, out_offset_bits, int_size, int_size);
	return has_overflow > 0;
}

void logical_not_abstract_integer(struct math_machine_info * mmi, unsigned int * in, unsigned int * out, unsigned int in_offset_bits, unsigned int out_offset_bits, unsigned int int_size){
	unsigned int i;
	unsigned int * current_word_in = in;
	unsigned int * current_word_out = out;
	unsigned int current_offset_in = in_offset_bits;
	unsigned int current_offset_out = out_offset_bits;
	for(i = 0; i < int_size; i++){
		unsigned int current_read_bit;

		current_word_in = &in[(in_offset_bits + i) / mmi->bits_in_int];
		current_word_out = &out[(out_offset_bits + i) / mmi->bits_in_int];
		current_offset_in = (in_offset_bits + i) % mmi->bits_in_int;
		current_offset_out = (out_offset_bits + i) % mmi->bits_in_int;

		current_read_bit = (*current_word_in & ((0x1u) << current_offset_in)) >> current_offset_in;

		/*  Clear the bit that might be getting set. */
		(*current_word_out) &= ~((0x1u) << current_offset_out);
		/*  Set the bit if it needs to be set.  */
		/*printf("Going to set bit %u to %u\n", current_offset_out, !current_read_bit);*/
		(*current_word_out) |= ((!current_read_bit) << current_offset_out);
	}
}

void print_abstract_integer(struct math_machine_info * mmi, unsigned int * in, unsigned int in_offset_bits, unsigned int in_bit_size){
	unsigned int remainder_bits = in_bit_size % 4;
	unsigned int current_bits_in_digit = remainder_bits == 0 ? 4 : remainder_bits;
	unsigned int bits_remaining = in_bit_size;

	char c[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	unsigned int word_bits = 32;
	(void)mmi;
	while(bits_remaining > 0){
		unsigned int current_digit = 0;
		unsigned int bits_consumed_current_digit = 0;
		while(bits_consumed_current_digit < current_bits_in_digit){
			unsigned int word_index = (in_offset_bits + bits_remaining -1) / word_bits;
			{/*printf("word_index is %u, in_offset_bits=%u, bits_remaining=%u, bits_consumed_current_digit=%u \n", word_index, in_offset_bits, bits_remaining, bits_consumed_current_digit);*/
			{
			unsigned int last_word = in[word_index];
			unsigned int current_bit_index = (in_offset_bits + bits_remaining -1) % word_bits;
			unsigned int current_bit_shift_value = current_bits_in_digit - bits_consumed_current_digit -1;
			current_digit |= ((last_word & (0x1u << current_bit_index)) >> current_bit_index) << current_bit_shift_value;
			/*printf("thebit=%08X, current_bit_index=%08X, bit_value=%08X\n", ((last_word & (0x1u << current_bit_index)) >> current_bit_index) << current_bit_shift_value, current_bit_index, (0x1u) << current_bit_shift_value);*/
			bits_consumed_current_digit += 1;
			bits_remaining -= 1;
			/*printf("last_word=%08X\n", last_word);*/

			}}
		}
		/*printf("digit index=%08X\n", current_digit);*/
		printf("%c", c[current_digit]);
		/*  All remaining digits consist of 4 bits. */
		current_bits_in_digit = 4;
		/*printf("At end bits remaining = %u\n", bits_remaining);*/
	}
}

unsigned int copy_abstract_integer(struct math_machine_info * mmi, unsigned int * in, unsigned int * out, unsigned int in_offset_bits, unsigned int out_offset_bits, unsigned int in_bit_size, unsigned int out_bit_size){
	unsigned int total_bits_to_copy = in_bit_size > out_bit_size ? out_bit_size : in_bit_size; /*  in will be truncated to the size of out. */
	unsigned int bits_left_to_copy = total_bits_to_copy;
	unsigned int * current_word_in = in;
	unsigned int * current_word_out = out;
	unsigned int current_offset_in = in_offset_bits;
	unsigned int current_offset_out = out_offset_bits;


	while(bits_left_to_copy > 0){
		unsigned int bits_after_offset_in = mmi->bits_in_int - current_offset_in;
		unsigned int bits_after_offset_out = mmi->bits_in_int - current_offset_out;
		unsigned int bits_to_consume = bits_left_to_copy;
		unsigned int high_bits_omitted;
		unsigned int read_mask;
		unsigned int write_mask;
		unsigned int read_bits;
		unsigned int write_bits;

		if(current_offset_in >= mmi->bits_in_int){
			current_word_in++;
			current_offset_in -= mmi->bits_in_int;
			/*printf("Advanced in.\n");*/
			continue;
		}

		if(current_offset_out >= mmi->bits_in_int){
			current_word_out++;
			current_offset_out -= mmi->bits_in_int;
			/*printf("Advanced out.\n");*/
			continue;
		}

		/*  Copy at most min(bits of space in copy destination word, bits left available to copy from source word, number of bits left overall) */
		if(bits_to_consume > bits_after_offset_in){
			bits_to_consume = bits_after_offset_in;
		}

		if(bits_to_consume > bits_after_offset_out){
			bits_to_consume = bits_after_offset_out;
		}

		/*printf("current_offset_in = %d, current_offset_out=%d\n", current_offset_in, current_offset_out);*/
		/*printf("bits_to_consume = %u\n", bits_to_consume);*/

		/*  Mask of 1s required to grab the bits we want to copy. */
		high_bits_omitted = mmi->bits_in_int - (current_offset_in + bits_to_consume);
		read_mask = ((((~0x0u) << current_offset_in) << high_bits_omitted) >> high_bits_omitted);
		read_bits = read_mask & (*current_word_in);
		/*printf("high_bits_omitted = %u, read_mask=%08X, read_bits=%08X\n", high_bits_omitted, read_mask, read_bits);*/
		if(current_offset_in > current_offset_out){
			/*  Shift the mask back to align with output */
			write_mask = read_mask >> (current_offset_in - current_offset_out);
			write_bits = read_bits >> (current_offset_in - current_offset_out);
			/*printf("current_offset_in > current_offset_out, write_mask=%08X, shifted write_bits=%08X\n", write_mask, write_bits);*/
		}else if(current_offset_in < current_offset_out){
			/*  Shift the mask forward to align with output */
			write_mask = read_mask << (current_offset_out - current_offset_in);
			write_bits = read_bits << (current_offset_out - current_offset_in);
			/*printf("current_offset_in < current_offset_out, write_mask=%08X, shifted write_bits=%08X\n", write_mask, write_bits);*/
		}else{
			write_mask = read_mask;
			write_bits = read_bits;
			/*printf("current_offset_in == current_offset_out, write_mask=%08X, shifted write_bits=%08X\n", write_mask, write_bits);*/
		}
		*current_word_out = *current_word_out & (~write_mask); /*  Clear all bits in the mask area, but leave the rest alone. */
		/*printf("Zeroing out mask, & with ~write_mask=%08X\n", ~write_mask);*/
		*current_word_out = *current_word_out | write_bits; /*  Make the assignment. */
		/*printf("Writing bits, | with write_bits=%08X\n", write_bits);*/
		bits_left_to_copy -= bits_to_consume;
		current_offset_in += bits_to_consume;
		current_offset_out += bits_to_consume;
	}

	while(current_offset_out >= mmi->bits_in_int){
		current_word_out++;
		current_offset_out -= mmi->bits_in_int;
		/*printf("Advanced after writing.\n");*/
	}

	if(out_bit_size > in_bit_size){
		/*  If the input size is smaller than the output size, then zero-fill the high bits to initialize them. */
		unsigned int num_zero_fill = out_bit_size - in_bit_size;
		/*printf("Need to zero fill %u bits\n", num_zero_fill);*/
		while(num_zero_fill > 0){
			/*printf("Zeroing bit, & with ~write_mask=%08X\n", (~(0x1u << current_offset_out)));*/
			*current_word_out &= (~(0x1u << current_offset_out));
			current_offset_out++;

			if(current_offset_out >= mmi->bits_in_int){
				current_word_out++;
				current_offset_out -= mmi->bits_in_int;
				/*printf("Advanced out by zeroing.\n");*/
				break;
			}

			num_zero_fill--;
		}
	}
	/*printf("Exiting copy...\n");*/
	/*  Return 1 if the input was truncated, 0 otherwise. */
	return in_bit_size > out_bit_size;
}
