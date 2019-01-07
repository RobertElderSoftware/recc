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

#include "../../recc-implementation/compiler_interface_phase_3.h"

void register_builtin_objects(struct build_state *);

void register_builtin_objects(struct build_state * state){
	/*  Builtin items */
	register_entity(state, "builtin/l2/memory_mapped_registers.l2", ENTITY_TYPE_L2_FILE);
	add_entity_attribute(state, "builtin/l2/memory_mapped_registers.l2", "terminal", "true");
	register_entity(state, "builtin/l2/mainstub.l2",                ENTITY_TYPE_L2_FILE);
	add_entity_attribute(state, "builtin/l2/mainstub.l2", "terminal", "true");

	register_entity(state, "builtin/signed_division.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/signed_division.c", "terminal", "true");
	register_entity(state, "builtin/signed_greater_than_or_equal_to.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/signed_greater_than_or_equal_to.c", "terminal", "true");
	register_entity(state, "builtin/signed_less_than_or_equal_to.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/signed_less_than_or_equal_to.c", "terminal", "true");
	register_entity(state, "builtin/signed_greater_than.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/signed_greater_than.c", "terminal", "true");
	register_entity(state, "builtin/signed_less_than.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/signed_less_than.c", "terminal", "true");
	register_entity(state, "builtin/signed_modulo.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/signed_modulo.c", "terminal", "true");
	register_entity(state, "builtin/print_decimal.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/print_decimal.c", "terminal", "true");
	register_entity(state, "builtin/print_hexadecimal.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/print_hexadecimal.c", "terminal", "true");
	register_entity(state, "builtin/print_string.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/print_string.c", "terminal", "true");
	register_entity(state, "builtin/print_buff_add.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/print_buff_add.c", "terminal", "true");
	register_entity(state, "builtin/print_bits.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/print_bits.c", "terminal", "true");
	register_entity(state, "builtin/stack.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/stack.c", "terminal", "true");
	register_entity(state, "builtin/c99libc_in_c89.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "builtin/c99libc_in_c89.c", "terminal", "true");

	register_entity(state, "builtin/signed_division.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/signed_greater_than_or_equal_to.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/signed_less_than_or_equal_to.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/signed_greater_than.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/signed_less_than.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/signed_modulo.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/print_decimal.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/print_hexadecimal.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/print_string.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/print_buff_add.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/print_bits.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/stack.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "builtin/c99libc_in_c89.i", ENTITY_TYPE_PREPROCESSED_FILE);

	register_dependency(state, "builtin/signed_division.i",                  "builtin/signed_division.c");
	register_dependency(state, "builtin/signed_greater_than_or_equal_to.i",  "builtin/signed_greater_than_or_equal_to.c");
	register_dependency(state, "builtin/signed_less_than_or_equal_to.i",     "builtin/signed_less_than_or_equal_to.c");
	register_dependency(state, "builtin/signed_greater_than.i",              "builtin/signed_greater_than.c");
	register_dependency(state, "builtin/signed_less_than.i",                 "builtin/signed_less_than.c");
	register_dependency(state, "builtin/signed_modulo.i",                    "builtin/signed_modulo.c");
	register_dependency(state, "builtin/print_decimal.i",                    "builtin/print_decimal.c");
	register_dependency(state, "builtin/print_hexadecimal.i",                "builtin/print_hexadecimal.c");
	register_dependency(state, "builtin/print_string.i",                     "builtin/print_string.c");
	register_dependency(state, "builtin/print_buff_add.i",                   "builtin/print_buff_add.c");
	register_dependency(state, "builtin/print_bits.i",                       "builtin/print_bits.c");
	register_dependency(state, "builtin/stack.i",                            "builtin/stack.c");
	register_dependency(state, "builtin/c99libc_in_c89.i",                   "builtin/c99libc_in_c89.c");

	register_entity(state, "builtin/signed_division.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/signed_greater_than_or_equal_to.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/signed_less_than_or_equal_to.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/signed_greater_than.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/signed_less_than.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/signed_modulo.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/print_decimal.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/print_hexadecimal.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/print_string.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/print_buff_add.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/print_bits.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/stack.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "builtin/c99libc_in_c89.l2", ENTITY_TYPE_L2_FILE);

	register_dependency(state, "builtin/signed_division.l2",                  "builtin/signed_division.i");
	register_dependency(state, "builtin/signed_greater_than_or_equal_to.l2",  "builtin/signed_greater_than_or_equal_to.i");
	register_dependency(state, "builtin/signed_less_than_or_equal_to.l2",     "builtin/signed_less_than_or_equal_to.i");
	register_dependency(state, "builtin/signed_greater_than.l2",              "builtin/signed_greater_than.i");
	register_dependency(state, "builtin/signed_less_than.l2",                 "builtin/signed_less_than.i");
	register_dependency(state, "builtin/signed_modulo.l2",                    "builtin/signed_modulo.i");
	register_dependency(state, "builtin/print_decimal.l2",                    "builtin/print_decimal.i");
	register_dependency(state, "builtin/print_hexadecimal.l2",                "builtin/print_hexadecimal.i");
	register_dependency(state, "builtin/print_string.l2",                     "builtin/print_string.i");
	register_dependency(state, "builtin/print_buff_add.l2",                   "builtin/print_buff_add.i");
	register_dependency(state, "builtin/print_bits.l2",                       "builtin/print_bits.i");
	register_dependency(state, "builtin/stack.l2",                            "builtin/stack.i");
	register_dependency(state, "builtin/c99libc_in_c89.l2",                   "builtin/c99libc_in_c89.i");

	register_entity(state, "builtin/libbuiltin.l2", ENTITY_TYPE_L2_FILE);

        register_dependency(state, "builtin/libbuiltin.l2", "builtin/c99libc_in_c89.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_division.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_greater_than_or_equal_to.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_less_than_or_equal_to.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_greater_than.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_less_than.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_modulo.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/print_decimal.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/print_hexadecimal.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/print_string.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/print_buff_add.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/print_bits.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/stack.l2");
}
