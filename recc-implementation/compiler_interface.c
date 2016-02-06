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

#include "compiler_interface.h"

void make_target(struct build_state *, struct build_target*, struct unsigned_char_ptr_list *);
void register_group(struct build_state *, char *, unsigned int);
void register_to_l1(struct build_state *, char *, char *);
unsigned char * get_parent_symbol_file(struct build_state *, struct build_target *);

void register_l0_file(struct build_state * state, const char * l0_filename, const char * function_name, const char * language_string){
	unsigned char * function_name_copy = copy_null_terminated_string((unsigned char *)function_name, state->memory_pool_collection);
	unsigned char * language_copy = copy_null_terminated_string((unsigned char *)language_string, state->memory_pool_collection);
	struct l0_build_info * info = (struct l0_build_info *)malloc(sizeof(struct l0_build_info));
	info->language = language_copy;
	info->function_name = function_name_copy;
	register_build_target(state, l0_filename, BUILD_TARGET_L0_FILE);
	unsigned_char_ptr_to_struct_build_target_ptr_map_get(&state->targets, (unsigned char *)l0_filename)->l0_info = info;
}

void register_l1_file(struct build_state * state, const char * l1_filename, unsigned int page_align_permission_regions, unsigned int only_metadata){
	struct l1_build_info * info = (struct l1_build_info *)malloc(sizeof(struct l1_build_info));
	info->page_align_permission_regions = page_align_permission_regions;
	info->only_metadata = only_metadata;
	register_build_target(state, l1_filename, BUILD_TARGET_L1_FILE);
	unsigned_char_ptr_to_struct_build_target_ptr_map_get(&state->targets, (unsigned char *)l1_filename)->l1_info = info;
}

void register_l2_file(struct build_state * state, const char * l2_filename, const char * offset){
	struct l2_build_info * info = (struct l2_build_info *)malloc(sizeof(struct l2_build_info));
	info->offset = copy_null_terminated_string((unsigned char *)offset, state->memory_pool_collection);
	register_build_target(state, l2_filename, BUILD_TARGET_L2_FILE);
	unsigned_char_ptr_to_struct_build_target_ptr_map_get(&state->targets, (unsigned char *)l2_filename)->l2_info = info;
}

const char * register_build_target(struct build_state * state, const char * name, enum build_target_type type){
	struct build_target * new_target = (struct build_target *)malloc(sizeof(struct build_target));
	unsigned char * key = copy_null_terminated_string((unsigned char *)name, state->memory_pool_collection);
	new_target->satisfied = 0;
	new_target->l0_info = (struct l0_build_info*)0;
	new_target->l1_info = (struct l1_build_info*)0;
	new_target->l2_info = (struct l2_build_info*)0;
	new_target->type = type;
	new_target->name = key;
	unsigned_char_ptr_to_struct_build_target_ptr_map_create(&new_target->children);
	unsigned_char_ptr_to_struct_build_target_ptr_map_create(&new_target->parents);

	if(unsigned_char_ptr_to_struct_build_target_ptr_map_exists(&state->targets, key)){
		printf("Item already registered in build process: %s\n", key);
		assert(0 && "Item already registered in build process.");
	}else{
		unsigned_char_ptr_to_struct_build_target_ptr_map_put(&state->targets, key, new_target);
	}
	return name;
}

const char * register_dependency(struct build_state * state, const char * parent, const char * child){
	unsigned char * parent_key = copy_null_terminated_string((unsigned char *)parent, state->memory_pool_collection);
	unsigned char * child_key = copy_null_terminated_string((unsigned char *)child, state->memory_pool_collection);
	if(
		unsigned_char_ptr_to_struct_build_target_ptr_map_exists(&state->targets, parent_key) &&
		unsigned_char_ptr_to_struct_build_target_ptr_map_exists(&state->targets, child_key)
	){
		struct build_target * parent_target = unsigned_char_ptr_to_struct_build_target_ptr_map_get(&state->targets, parent_key);
		struct build_target * child_target = unsigned_char_ptr_to_struct_build_target_ptr_map_get(&state->targets, child_key);
		if(unsigned_char_ptr_to_struct_build_target_ptr_map_exists(&parent_target->children, child_key)){
			printf("%s is already a child of %s.\n", child_key, parent_key);
			assert(0 && "Redundant dependency rule.");
		}else{
			unsigned_char_ptr_to_struct_build_target_ptr_map_put(&parent_target->children, child_key, child_target);
		}

		if(!unsigned_char_ptr_to_struct_build_target_ptr_map_exists(&child_target->parents, parent_key)){
			unsigned_char_ptr_to_struct_build_target_ptr_map_put(&child_target->parents, parent_key, parent_target);
		}
	}else{
		if(!unsigned_char_ptr_to_struct_build_target_ptr_map_exists(&state->targets, parent_key)){
			printf("Dependency parent %s was not registered.\n", parent_key);
		}

		if(!unsigned_char_ptr_to_struct_build_target_ptr_map_exists(&state->targets, child_key)){
			printf("Dependency child %s was not registered.  Required by parent %s.\n", child_key, parent_key);
		}
		
		assert(0 && "Parent or child was not registered..");
	}
	return parent;
}


unsigned char * get_parent_symbol_file(struct build_state * state, struct build_target * target){
	struct unsigned_char_ptr_list parent_keys = unsigned_char_ptr_to_struct_build_target_ptr_map_keys(&target->parents);
	unsigned int i;
	unsigned char * rtn = (unsigned char *)0;
	(void)state;
	for(i = 0; i < unsigned_char_ptr_list_size(&parent_keys); i++){
		struct build_target * p = unsigned_char_ptr_to_struct_build_target_ptr_map_get(&state->targets, unsigned_char_ptr_list_get(&parent_keys, i));
		if(p->type == BUILD_TARGET_SYMBOL_FILE){
			assert(!rtn && "Two symbol files?");
			rtn = unsigned_char_ptr_list_get(&parent_keys, i);
		}
	}
	unsigned_char_ptr_list_destroy(&parent_keys);
	return rtn;
}

void make_target(struct build_state * state, struct build_target * target, struct unsigned_char_ptr_list * child_keys){
	switch(target->type){
		case BUILD_TARGET_L1_FILE:{
			unsigned char * symbol_file = get_parent_symbol_file(state, target);
			printf("Build process creating L1 file '%s'", target->name);
			if(symbol_file){
				printf(" with symbol file %s.\n", symbol_file);
			}else{
				printf(".\n");
			}
			do_link(state->memory_pool_collection, child_keys, target->name, symbol_file, BUILD_TARGET_L1_FILE, (unsigned char *)"0x0", target->l1_info->page_align_permission_regions, target->l1_info->only_metadata);
			break;
		}case BUILD_TARGET_L2_FILE:{
			/*  If it has a single child that is a preprocessed C file, then we're just building a regular l2 file. */
			if(unsigned_char_ptr_list_size(child_keys) == 1 && unsigned_char_ptr_to_struct_build_target_ptr_map_get(&target->children, unsigned_char_ptr_list_get(child_keys, 0))->type == BUILD_TARGET_PREPROCESSED_FILE){
				printf("Build process creating L2 file '%s' from preprocessed file '%s'.\n", target->name, unsigned_char_ptr_list_get(child_keys,0));
				do_code_generation(state->memory_pool_collection, unsigned_char_ptr_list_get(child_keys, 0), target->name, target->l2_info->offset);
			}else{
				/*  Otherwise, we must be linking together multiple l2 files into a bit one. */
				unsigned char * symbol_file = get_parent_symbol_file(state, target);
				do_link(state->memory_pool_collection, child_keys, target->name, symbol_file, BUILD_TARGET_L2_FILE, target->l2_info->offset, 0, 0);
			}
			break;
		}case BUILD_TARGET_L0_FILE:{
			struct l0_generator_state * l0_generator_state;
			printf("Build process creating L0 file '%s' from l1 file '%s'.\n", target->name, unsigned_char_ptr_list_get(child_keys,0));
			l0_generator_state = l0_generator_state_create(state->memory_pool_collection, target->l0_info->function_name, unsigned_char_ptr_list_get(child_keys,0), target->name, target->l0_info->language);
			l0_generator_state_destroy(l0_generator_state);
			break;
		}case BUILD_TARGET_FILESYSTEM_IMPLEMENTATION:{
			printf("Build process creating filesystem implementation '%s'.\n", target->name);
			create_filesystem_impl(target->name);
			break;
		}case BUILD_TARGET_PREPROCESSED_FILE:{
			printf("Build process creating preprocessed file '%s' from C file '%s'.\n", target->name, unsigned_char_ptr_list_get(child_keys,0));
			do_preprocess(state->memory_pool_collection, unsigned_char_ptr_list_get(child_keys,0), target->name);
			break;
		}case BUILD_TARGET_C_FILE:{
			assert(0 && "Not supported.");
			break;
		}default:{
			assert(0 && "Not expected.");
			break;
		}
	}
	target->satisfied = 1;
}

void construct_build_target(struct build_state * state, const char * target){
	if(unsigned_char_ptr_to_struct_build_target_ptr_map_exists(&state->targets, (unsigned char *)target)){
		struct build_target * t = unsigned_char_ptr_to_struct_build_target_ptr_map_get(&state->targets, (unsigned char *)target);
		struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_build_target_ptr_map_keys(&t->children);
		unsigned int i;
		unsigned int num_keys = unsigned_char_ptr_list_size(&keys);
		for(i = 0; i < num_keys; i++){
			construct_build_target(state, (char*)unsigned_char_ptr_list_get(&keys, i));
		}

		/*  Hack for now, filesystem implementation doesn't specify dependencies. */
		if((num_keys && !t->satisfied) || (t->type == BUILD_TARGET_FILESYSTEM_IMPLEMENTATION)){
			make_target(state, t, &keys);
		}
		unsigned_char_ptr_list_destroy(&keys);
	}else{
		printf("Can't build:  Target '%s' does not exist.", target);
		assert(0 && "Unsatisfied dependency.");
	}
}

struct build_state * create_build_state(void){
	struct build_state * state = (struct build_state *)malloc(sizeof(struct build_state));
	unsigned_char_ptr_to_struct_build_target_ptr_map_create(&state->targets);
	state->memory_pool_collection = (struct memory_pool_collection*)malloc(sizeof(struct memory_pool_collection));
	memory_pool_collection_create(state->memory_pool_collection);
	return state;
}

void destroy_build_state(struct build_state * state){
	unsigned int i;
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_build_target_ptr_map_keys(&state->targets);
	unsigned int num_targets = unsigned_char_ptr_list_size(&keys);
	for(i = 0; i < num_targets; i++){
		unsigned int j;
		unsigned char * k = unsigned_char_ptr_list_get(&keys, i);
		struct build_target * target = unsigned_char_ptr_to_struct_build_target_ptr_map_get(&state->targets, k);
		struct unsigned_char_ptr_list parent_keys = unsigned_char_ptr_to_struct_build_target_ptr_map_keys(&target->parents);
		struct unsigned_char_ptr_list child_keys = unsigned_char_ptr_to_struct_build_target_ptr_map_keys(&target->children);
		unsigned int num_parent_keys = unsigned_char_ptr_list_size(&parent_keys);
		unsigned int num_child_keys = unsigned_char_ptr_list_size(&child_keys);
		for(j = 0; j < num_parent_keys; j++){
			heap_memory_pool_free(state->memory_pool_collection->heap_pool, unsigned_char_ptr_list_get(&parent_keys, j));
		}
		for(j = 0; j < num_child_keys; j++){
			heap_memory_pool_free(state->memory_pool_collection->heap_pool, unsigned_char_ptr_list_get(&child_keys, j));
		}
		
		unsigned_char_ptr_list_destroy(&parent_keys);
		unsigned_char_ptr_list_destroy(&child_keys);
		unsigned_char_ptr_to_struct_build_target_ptr_map_destroy(&target->parents);
		unsigned_char_ptr_to_struct_build_target_ptr_map_destroy(&target->children);

		if(target->l0_info){
			heap_memory_pool_free(state->memory_pool_collection->heap_pool, target->l0_info->language);
			heap_memory_pool_free(state->memory_pool_collection->heap_pool, target->l0_info->function_name);
		}
		if(target->l2_info){
			heap_memory_pool_free(state->memory_pool_collection->heap_pool, target->l2_info->offset);
		}
		free(target->l0_info);
		free(target->l1_info);
		free(target->l2_info);
		free(target);
	}

	/*  Delete keys after map contents are freed */
	for(i = 0; i < num_targets; i++){
		heap_memory_pool_free(state->memory_pool_collection->heap_pool, unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_list_destroy(&keys);
	unsigned_char_ptr_to_struct_build_target_ptr_map_destroy(&state->targets);
	memory_pool_collection_destroy(state->memory_pool_collection);
	free(state->memory_pool_collection);
	free(state);
}

void register_to_l1(struct build_state * state, char * l2_item, char * item){
	char * l1_item = (char *)create_formatted_string(state->memory_pool_collection, 20, "%s.l1", item);
	register_l1_file(state, l1_item, 0, 0);
	register_dependency(state, l1_item, l2_item);
	heap_memory_pool_free(state->memory_pool_collection->heap_pool, l1_item);
}

void register_group(struct build_state * state, char * item, unsigned int include_l1){
	/*  A common set of dependency rules around one name */
	char * l2_item = (char *)create_formatted_string(state->memory_pool_collection, 20, "%s.l2", item);
	char * i_item = (char *)create_formatted_string(state->memory_pool_collection, 20, "%s.i", item);
	char * c_item = (char *)create_formatted_string(state->memory_pool_collection, 20, "%s.c", item);

	register_l2_file(state, l2_item, "RELOCATABLE");
	register_build_target(state, i_item,  BUILD_TARGET_PREPROCESSED_FILE);
	register_build_target(state, c_item,  BUILD_TARGET_C_FILE);
	register_dependency(state, i_item, c_item);
	register_dependency(state, l2_item, i_item);

	if(include_l1){
		register_to_l1(state, l2_item, item);
	}

	heap_memory_pool_free(state->memory_pool_collection->heap_pool, l2_item);
	heap_memory_pool_free(state->memory_pool_collection->heap_pool, i_item);
	heap_memory_pool_free(state->memory_pool_collection->heap_pool, c_item);
}

void register_c_to_l2(struct build_state * state, const char * item){
        register_group(state, (char*)item, 0);
}

void register_c_to_l1(struct build_state * state, const char * item){
        register_group(state, (char*)item, 1);
}
