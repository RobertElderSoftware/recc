#include "../compiler_interface.h"

void register_filesystem_objects(struct build_state *);

void register_filesystem_objects(struct build_state * state){
	/*  Implemention of emulated ext like filesystem */
	register_build_target(state, "filesystem/filesystem_impl.l2",         BUILD_TARGET_FILESYSTEM_IMPLEMENTATION);
}
