#include "compiler_interface.h"

void register_compiler_objects(struct build_state *);

void register_compiler_objects(struct build_state * state){
	/*  Parts of the compiler */
	register_c_to_l2(state, "io");
	register_c_to_l2(state, "preprocessor");
	register_c_to_l2(state, "code_generator");
	register_c_to_l2(state, "parser");
	register_c_to_l2(state, "linker");
	register_c_to_l2(state, "lexer");
	register_c_to_l2(state, "heap_memory_pool");
	register_c_to_l2(state, "memory_pool_collection");
}
