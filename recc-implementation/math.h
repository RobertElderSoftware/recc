#ifndef MATH_H_
#define MATH_H_
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

#include <limits.h>

enum math_machine_info_type {
	MATH_LITTLE_ENDIAN,
	MATH_BIG_ENDIAN
};

struct math_machine_info {
	unsigned int bits_in_int;
	enum math_machine_info_type type;
};


void zero_abstract_integer(struct math_machine_info *, unsigned int * in, unsigned int in_offset_bits, unsigned int int_size);
unsigned int gt_abstract_integer(struct math_machine_info *, unsigned int * in_a, unsigned int * in_b, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int int_size);
unsigned int lt_abstract_integer(struct math_machine_info *, unsigned int * in_a, unsigned int * in_b, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int int_size);
unsigned int eq_abstract_integer(struct math_machine_info *, unsigned int * in_a, unsigned int * in_b, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int int_size);
void logical_or_abstract_integer(struct math_machine_info *, unsigned int * in_a, unsigned int * in_b, unsigned int * out, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int out_offset_bits, unsigned int int_size);
void logical_and_abstract_integer(struct math_machine_info *, unsigned int * in_a, unsigned int * in_b, unsigned int * out, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int out_offset_bits, unsigned int int_size);
void logical_not_abstract_integer(struct math_machine_info *, unsigned int * in, unsigned int * out, unsigned int in_offset_bits, unsigned int out_offset_bits, unsigned int int_size);
unsigned int increment_abstract_integer(struct math_machine_info *, unsigned int * in, unsigned int * out, unsigned int in_offset_bits, unsigned int out_offset_bits, unsigned int int_size, unsigned int starting_position);
unsigned int add_abstract_integer(struct math_machine_info *, unsigned int * in_a, unsigned int * in_b, unsigned int * out, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int out_offset_bits, unsigned int int_size);
unsigned int subtract_abstract_integer(struct math_machine_info *, unsigned int * in_a, unsigned int * in_b, unsigned int * out, unsigned int * tmp, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int out_offset_bits, unsigned int tmp_offset_bits, unsigned int int_size);
unsigned int multiply_abstract_integer(struct math_machine_info *, unsigned int * in_a, unsigned int * in_b, unsigned int * out, unsigned int * partial_result, unsigned int in_a_offset_bits, unsigned int in_b_offset_bits, unsigned int out_offset_bits, unsigned int partial_result_offset_bits, unsigned int int_size);
unsigned int divide_abstract_integer(struct math_machine_info *, unsigned int * numerator, unsigned int * denominator, unsigned int * out, unsigned int * partial_remainder, unsigned int * sub_tmp, unsigned int * partial_result, unsigned int numerator_offset_bits, unsigned int denominator_offset_bits, unsigned int out_offset_bits, unsigned int partial_remainder_offset_bits, unsigned int sub_tmp_offset_bits, unsigned int partial_result_offset_bits, unsigned int int_size);

unsigned int divide_and_modulo_abstract_integer(struct math_machine_info *, unsigned int * numerator, unsigned int * denominator, unsigned int * out, unsigned int * partial_remainder, unsigned int * sub_tmp, unsigned int * partial_result, unsigned int numerator_offset_bits, unsigned int denominator_offset_bits, unsigned int out_offset_bits, unsigned int partial_remainder_offset_bits, unsigned int sub_tmp_offset_bits, unsigned int partial_result_offset_bits, unsigned int int_size, unsigned int divide_or_modulo);
unsigned int modulo_abstract_integer(struct math_machine_info *, unsigned int * numerator, unsigned int * denominator, unsigned int * out, unsigned int * partial_remainder, unsigned int * sub_tmp, unsigned int * partial_result, unsigned int numerator_offset_bits, unsigned int denominator_offset_bits, unsigned int out_offset_bits, unsigned int partial_remainder_offset_bits, unsigned int sub_tmp_offset_bits, unsigned int partial_result_offset_bits, unsigned int int_size);

void lshift_abstract_integer(struct math_machine_info *, unsigned int * in_a, unsigned int * out, unsigned int in_a_offset_bits, unsigned int out_offset_bits, unsigned int int_size, unsigned int shift_offset);
void rshift_abstract_integer(struct math_machine_info *, unsigned int * in_a, unsigned int * out, unsigned int in_a_offset_bits, unsigned int out_offset_bits, unsigned int int_size, unsigned int shift_offset);

void print_abstract_integer(struct math_machine_info *, unsigned int * in, unsigned int in_offset_bits, unsigned int in_bit_size);
unsigned int copy_abstract_integer(struct math_machine_info *, unsigned int * in, unsigned int * out, unsigned int in_offset_bits, unsigned int out_offset_bits, unsigned int in_bit_size, unsigned int out_bit_size);
unsigned int add_abstract_integers(struct math_machine_info *, unsigned int * a, unsigned int * b, unsigned int a_offset_bits, unsigned int b_offset_bits, unsigned int bit_size);

#endif
