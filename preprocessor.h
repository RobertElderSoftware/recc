/*
	Copyright 2015 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#ifndef __PREPROCESSOR_H__
#define __PREPROCESSOR_H__ 

#include <assert.h>
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lexer.h"
#include "io.h"
#include "data-structures/unsigned_char_list.h"
#include "data-structures/struct_c_lexer_token_ptr_list.h"
#include "data-structures/unsigned_char_ptr_to_unsigned_char_ptr_map.h"

int preprocess_tokens(struct memory_pooler_collection *, struct c_lexer_state *, struct unsigned_char_list *, unsigned char *, struct unsigned_char_ptr_to_unsigned_char_ptr_map *);
int tokcmp(const char *, const char *, unsigned int);
unsigned int does_token_match(struct c_lexer_state *, enum c_token_type, unsigned int);
void skip_whitespace(struct c_lexer_state *, unsigned int *);
int do_preprocess(struct memory_pooler_collection *, unsigned char *, unsigned char *);
int get_preprocessed_output_from_file(struct memory_pooler_collection *, unsigned char *, struct unsigned_char_list *, struct unsigned_char_ptr_to_unsigned_char_ptr_map *);
unsigned char * convert_filename_to_directory(unsigned char *);
void free_define_map(struct unsigned_char_ptr_to_unsigned_char_ptr_map *);

#endif
