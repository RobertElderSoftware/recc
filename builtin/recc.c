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

#include "../recc-implementation/compiler_interface.h"

void register_builtin_objects(struct build_state *);

void register_builtin_objects(struct build_state * state){
	/*  Builtin items */
	register_l2_file(state, "builtin/l2/memory_mapped_registers.l2", "RELOCATABLE");
	register_l2_file(state, "builtin/l2/mainstub.l2",                "RELOCATABLE");

	register_c_to_l2(state, "builtin/signed_division");
	register_c_to_l2(state, "builtin/signed_greater_than_or_equal_to");
	register_c_to_l2(state, "builtin/signed_less_than_or_equal_to");
	register_c_to_l2(state, "builtin/signed_greater_than");
	register_c_to_l2(state, "builtin/signed_less_than");
	register_c_to_l2(state, "builtin/signed_modulo");
	register_c_to_l2(state, "builtin/print_decimal");
	register_c_to_l2(state, "builtin/print_hexadecimal");
	register_c_to_l2(state, "builtin/print_string");
	register_c_to_l2(state, "builtin/print_buff_add");
	register_c_to_l2(state, "builtin/print_bits");
	register_c_to_l2(state, "builtin/stack");
	register_c_to_l2(state, "builtin/c99libc_in_c89");

	register_l2_file(state, "builtin/libbuiltin.l2", "RELOCATABLE");

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
