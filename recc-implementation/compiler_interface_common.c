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

#include "compiler_interface_common.h"

struct entity * make_new_entity(unsigned char * name){
	struct entity * e = (struct entity *)malloc(sizeof(struct entity));
	unsigned_char_ptr_to_unsigned_char_ptr_map_create(&e->attributes, struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_compare);
	e->satisfied = 0;
	e->registered = 0;
	e->name = name;
	return e;
}

unsigned char * copy_of_string(unsigned char * start){
	unsigned int length = (unsigned int)strlen((char *)start);
	unsigned char * rtn = (unsigned char *)malloc((sizeof(unsigned char) * length) + 1);
	unsigned int i = 0;
	for(i = 0; i < length; i++){
		rtn[i] = start[i];
	}
	rtn[length] = 0;
	return rtn;
}

void destroy_entity(struct entity * e){
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_unsigned_char_ptr_map_keys(&e->attributes);
	unsigned int i;
	for(i = 0; i < unsigned_char_ptr_list_size(&keys); i++){
		unsigned char * key = unsigned_char_ptr_list_get(&keys, i);
		unsigned char * value = unsigned_char_ptr_to_unsigned_char_ptr_map_get(&e->attributes, key);
		free(value);
	}
	/*  Go through list twice to avoid use after free of keys */
	for(i = 0; i < unsigned_char_ptr_list_size(&keys); i++){
		free(unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_list_destroy(&keys);
	unsigned_char_ptr_to_unsigned_char_ptr_map_destroy(&e->attributes);
	free(e);
}

void add_entity_attribute(struct build_state * state, const char * ent, const char * key, const char * value){
	if(unsigned_char_ptr_to_struct_entity_ptr_map_exists(&state->targets, (unsigned char *)ent)){
		struct entity * e = unsigned_char_ptr_to_struct_entity_ptr_map_get(&state->targets, (unsigned char *)ent);
		unsigned char * key_copy = copy_of_string((unsigned char *)key);
		unsigned char * value_copy = copy_of_string((unsigned char *)value);
		if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&e->attributes, key_copy)){
			printf("Entity %s already has attribute %s set and its value is %s.\n", ent, key, unsigned_char_ptr_to_unsigned_char_ptr_map_get(&e->attributes, key_copy));
			assert(0);
		}else{
			unsigned_char_ptr_to_unsigned_char_ptr_map_put(&e->attributes, key_copy, value_copy);
		}
	}else{
		printf("Entity %s was not found so no attribute can be added.\n", ent);
		assert(0);
	}
}

struct entity * get_entity(struct build_state * state, const char * name){
	if(unsigned_char_ptr_to_struct_entity_ptr_map_exists(&state->targets, (unsigned char *)name)){
		return unsigned_char_ptr_to_struct_entity_ptr_map_get(&state->targets, (unsigned char *)name);
	}
	return (struct entity *)0;
}

const char * register_entity(struct build_state * state, const char * name, enum entity_type type){
	struct entity * e;
	if(unsigned_char_ptr_to_struct_entity_ptr_map_exists(&state->targets, (unsigned char *)name)){
		e = unsigned_char_ptr_to_struct_entity_ptr_map_get(&state->targets, (unsigned char *)name);
		if(e->registered){
			printf("Item already registered in build process: %s\n", name);
			assert(0 && "Item already registered in build process.");
		}
	}else{
		unsigned char * key = copy_of_string((unsigned char *)name);
		e = make_new_entity(key);
		unsigned_char_ptr_to_struct_entity_ptr_map_put(&state->targets, key, e);
	}
	e->type = type;
	e->registered = 1;
	return name;
}

int struct_entity_cmp(struct entity * a, struct entity * b){
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}else{
		return 0;
	}
}

struct struct_entity_ptr_list * get_entity_list_for_relationship(struct build_state * state, struct entity * e, enum entity_relation_type type){
	struct entity_relationship r;
	r.entity = e;
	r.type = type;
	if(struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_exists(&state->relationships, r)){
		return struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_get(&state->relationships, r);
	}else{
		return (struct struct_entity_ptr_list *)0;
	}
}

struct unsigned_char_ptr_list get_entity_name_list_for_relationship(struct build_state * state, struct entity * e, enum entity_relation_type type){
	struct unsigned_char_ptr_list rtn_list;
	struct struct_entity_ptr_list * list = get_entity_list_for_relationship(state, e, type);
	unsigned_char_ptr_list_create(&rtn_list);
	if(list){
		unsigned int i;
		unsigned int size = struct_entity_ptr_list_size(list);
		for(i = 0; i < size; i++){
			unsigned_char_ptr_list_add_end(&rtn_list, struct_entity_ptr_list_get(list, i)->name);
		}
	}
	return rtn_list;
}



void register_relationship(struct build_state * state, unsigned char * a , unsigned char * b, enum entity_relation_type type){
	struct struct_entity_ptr_list * list;
	struct entity * a_target;
	struct entity * b_target;
	if(unsigned_char_ptr_to_struct_entity_ptr_map_exists(&state->targets, (unsigned char *)a)){
		a_target = unsigned_char_ptr_to_struct_entity_ptr_map_get(&state->targets, (unsigned char *)a);
	}else{
		a_target = make_new_entity(copy_of_string((unsigned char *)a));
		unsigned_char_ptr_to_struct_entity_ptr_map_put(&state->targets, a_target->name, a_target);
	}

	if(unsigned_char_ptr_to_struct_entity_ptr_map_exists(&state->targets, (unsigned char *)b)){
		b_target = unsigned_char_ptr_to_struct_entity_ptr_map_get(&state->targets, (unsigned char *)b);
	}else{
		b_target = make_new_entity(copy_of_string((unsigned char *)b));
		unsigned_char_ptr_to_struct_entity_ptr_map_put(&state->targets, b_target->name, b_target);
	}
		
	if(a_target == b_target){
		printf("%s should not relate to itself.\n", a_target->name);
		assert((a_target != b_target) && "Entity should not be related to itself.");
	}
	if((list = get_entity_list_for_relationship(state, a_target, type))){
		/*  Relationships of that type already exist */
		if(struct_entity_ptr_list_occurrences(list, b_target, struct_entity_cmp)){
			printf("%s already relates to %s with relationship id %d\n", a_target->name, b_target->name, type);
			assert(0 && "This relationship already exists.");
		}else{
			struct_entity_ptr_list_add_end(list, b_target);
		}
	}else{
		struct entity_relationship r;
		r.entity = a_target;
		r.type = type;
		/*  No relationships of that type already exist, create a new entry  */
		list = (struct struct_entity_ptr_list *)malloc(sizeof(struct struct_entity_ptr_list));
		struct_entity_ptr_list_create(list);
		struct_entity_ptr_list_add_end(list, b_target);
		struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_put(&state->relationships, r, list);
	}
}

const char * register_dependency(struct build_state * state, const char * a , const char * b){
	register_relationship(state, (unsigned char *)a, (unsigned char *)b, RELATION_TYPE_DEPENDS_ON_AT_BUILD_TIME);
	register_relationship(state, (unsigned char *)b, (unsigned char *)a, RELATION_TYPE_IS_DEPENDED_ON_AT_BUILD_TIME_BY);
	return a;
}

const char * register_composition(struct build_state * state, const char * a , const char * b){
	register_relationship(state, (unsigned char *)a, (unsigned char *)b, RELATION_TYPE_DEPENDS_ON_AT_BUILD_TIME);
	register_relationship(state, (unsigned char *)b, (unsigned char *)a, RELATION_TYPE_IS_DEPENDED_ON_AT_BUILD_TIME_BY);
	register_relationship(state, (unsigned char *)a, (unsigned char *)b, RELATION_TYPE_CONTAINS);
	register_relationship(state, (unsigned char *)b, (unsigned char *)a, RELATION_TYPE_IS_CONTAINED_BY);
	return a;
}

const char * register_inclusion(struct build_state * state, const char * a , const char * b){
	register_relationship(state, (unsigned char *)a, (unsigned char *)b, RELATION_TYPE_INCLUDES);
	register_relationship(state, (unsigned char *)b, (unsigned char *)a, RELATION_TYPE_IS_INCLUDED_BY);
	return a;
}

unsigned int construct_entity(struct build_state * state, const char * target){
	if(unsigned_char_ptr_to_struct_entity_ptr_map_exists(&state->targets, (unsigned char *)target)){
		struct entity * t = unsigned_char_ptr_to_struct_entity_ptr_map_get(&state->targets, (unsigned char *)target);
		struct unsigned_char_ptr_list depends_on = get_entity_name_list_for_relationship(state, t, RELATION_TYPE_DEPENDS_ON_AT_BUILD_TIME);
		unsigned int num_build_time_dependencies = unsigned_char_ptr_list_size(&depends_on);
		unsigned int i;
		unsigned int requires_build = 1;
		if(!(t->registered)){
			printf("Target %s has not been registered yet, so it can't be built.\n", t->name);
			assert(0);
		}
		for(i = 0; i < num_build_time_dependencies; i++){
			if(construct_entity(state, (char*)unsigned_char_ptr_list_get(&depends_on, i))){
				return 1;
			}
		}

		/*  If this is specified to be a terminal */
		if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&t->attributes, (unsigned char *)"terminal")){
			const char * attr = (const char *)unsigned_char_ptr_to_unsigned_char_ptr_map_get(&t->attributes, (unsigned char *)"terminal");
			requires_build = !(strcmp(attr, "true") == 0);
		}

		if(requires_build){
			if(!t->satisfied){
				if(make_target(state, t)){
					return 1;
				}
			}
		}else{
			/*  If it does not need to be built, it should not have build time dependencies */
			if(num_build_time_dependencies > 0){
				printf("Can't build:  Target '%s' has build time dependencies, but is supposed to be a terminal dependency.", target);
				assert(0);
			}
		}
		unsigned_char_ptr_list_destroy(&depends_on);
	}else{
		printf("Can't build:  Target '%s' does not exist.", target);
		assert(0 && "Unsatisfied dependency.");
	}
	return 0;
}

struct files_for_type * make_build_rules_for_typed_files(struct bootstrap_state * b, struct build_state * state, struct files_for_type * files_for_type){
	unsigned int i;
	struct templated_file ** data = (struct templated_file **)binary_exponential_buffer_data(&files_for_type->files);
	unsigned int num_files = binary_exponential_buffer_size(&files_for_type->files);
	for(i = 0; i < num_files; i++){
		struct templated_file * f = data[i];
		unsigned int j;
		unsigned int num_inclusions = binary_exponential_buffer_size(&f->header_files);
		const char * infile = (const char *)binary_exponential_buffer_data(&f->in_file);
		const char * outfile = (const char *)binary_exponential_buffer_data(&f->out_file);
		const char ** headers = (const char **)binary_exponential_buffer_data(&f->header_files);
		struct entity * source_entity = get_entity(state, infile);
		struct entity * out_entity = get_entity(state, outfile);

		if(!(out_entity && out_entity->registered)){ /*  Only attempt to register things for this type if the outfile has not been registered */
			switch(f->type){
				case TEMPLATED_FILE_TYPE_C_SOURCE:{
					if(!source_entity || !source_entity->registered){  /*  If the source file hasn't been registered, do it now. */
						register_entity(state, infile, ENTITY_TYPE_SOURCE);
						add_entity_attribute(state, infile, "terminal", "true");
					}
					register_composition(state, outfile, infile);

					for(j = 0; j < num_inclusions; j++){
						register_inclusion(state, outfile, headers[j]);
					}

					register_entity(state, outfile, ENTITY_TYPE_GENERATED_C_CODE);
					add_entity_attribute(state, outfile, "is_header", f->is_header ? "true" : "false");
					add_entity_attribute(state, outfile, "type1", (const char *)f->type_1_literal);
					if(f->type_2_literal){
						add_entity_attribute(state, outfile, "type2", (const char *)f->type_2_literal);
					}
					break;
				}case TEMPLATED_FILE_TYPE_I_FILE:{
					register_entity(state, outfile, ENTITY_TYPE_PREPROCESSED_FILE);
					register_dependency(state, outfile, infile);
					break;
				}case TEMPLATED_FILE_TYPE_I_TO_L2:{
					register_entity(state, outfile, ENTITY_TYPE_L2_FILE);
					register_dependency(state, outfile, infile);
					register_dependency(state, "recc-implementation/libdata-structures.l2", outfile);
					break;
				}case TEMPLATED_FILE_TYPE_C_TO_O:{
					register_entity(state, outfile, ENTITY_TYPE_OBJECT_FILE);
					make_build_rules(b, outfile, infile);
					break;
				}default:{
					assert(0 && "Not expected");
				}
			}
		}else{	
			printf("Skipping registeration for %s file because it has already been referenced.\n", outfile);
		}
	}
	return files_for_type;
}

void construct_generated_c_entities(struct build_state * state){
	struct struct_entity_ptr_list entities = unsigned_char_ptr_to_struct_entity_ptr_map_values(&state->targets);
	unsigned int size = struct_entity_ptr_list_size(&entities);
	unsigned int i;
	for(i = 0; i < size; i++){
		struct entity * e = struct_entity_ptr_list_get(&entities, i);
		if(e->registered && e->type == ENTITY_TYPE_GENERATED_C_CODE){
			construct_entity(state, (const char *)e->name);
		}
	}
	struct_entity_ptr_list_destroy(&entities);
}
