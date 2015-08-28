#include "../../compiler_interface.h"

void build_tests(void);

int main(void){
	struct build_state * state = create_build_state();
	register_build_target(state, "demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h",  BUILD_TARGET_C_FILE);
	register_build_target(state, "demos/brainfuck-cpp/result.i",  BUILD_TARGET_PREPROCESSED_FILE);
        register_dependency(state, "demos/brainfuck-cpp/result.i", "demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h");
	construct_build_target(state, "demos/brainfuck-cpp/result.i");
	destroy_build_state(state);
	return 0;
}
