#ifndef FILESYSTEM_COMPILER_H_
#define FILESYSTEM_COMPILER_H_
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

#ifndef ASSERT_H_DEFINED_
#include <assert.h>
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif
#ifndef STDLIB_FILESYSTEM_H_
#include "../libc/filesystem.h"
#endif
#ifndef unsigned_char_ptr_to_unsigned_char_ptr_map_H_DEFINED_
#include "../generated/unsigned_char_ptr_to_unsigned_char_ptr_map.h"
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../generated/unsigned_char_list.h"
#endif
#ifndef IO_H_
#include "../recc-implementation/io.h"
#endif
#ifndef unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_compare_H_DEFINED_
#include "../generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_compare.h"
#endif

void output_filesystem_impl(unsigned char *);
void create_directories(struct unsigned_char_ptr_list *, struct memory_pool_collection *);
void create_files(struct unsigned_char_ptr_to_unsigned_char_ptr_map *, struct memory_pool_collection *);
void load_file(unsigned char *, FILE *, struct memory_pool_collection *);
void load_file_given_new_inode(unsigned int, FILE *);
void create_filesystem_impl(unsigned char *);

#endif
