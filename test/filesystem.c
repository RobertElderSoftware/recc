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

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "../data-structures/unsigned_char_list.h"
#include "../data-structures/unsigned_char_ptr_list.h"
#include "../io.h"

#define BUFFER_LEN 1000

int main(int argc, char * argv[]){
	const char * file_to_open = "filesystem.c";
	char cwd[BUFFER_LEN];
	char c = 0;
	unsigned int i;
	struct unsigned_char_list list;
	struct unsigned_char_ptr_list components_list;
	FILE * f;
	unsigned_char_list_create(&list);
	unsigned_char_ptr_list_create(&components_list);

	/*  Get current working directory. */
	getcwd(cwd, BUFFER_LEN);

	/*  Add current working directory to target file path */
	for(i = 0; ; i++){
		if(!cwd[i]){
			break;
		}
		unsigned_char_list_add_end(&list, cwd[i]);
	}

	if(unsigned_char_list_get(&list, unsigned_char_list_size(&list) -1) != '/'){
		unsigned_char_list_add_end(&list, '/');
	}

	/*  Make sure executable name was passed in */
	assert(argc == 1);
	/*  Add current working directory to full file name */
	resolve_path_components((unsigned char *)argv[0], &components_list);
	/*  Remove last component because it represents the current executable name */
	unsigned_char_ptr_list_remove_all(&components_list, unsigned_char_ptr_list_get(&components_list, unsigned_char_ptr_list_size(&components_list) -1));
	/*  Add current executable path */
	for(i = 0; i < unsigned_char_ptr_list_size(&components_list); i++){
		unsigned char * current_component = unsigned_char_ptr_list_get(&components_list, i);
		unsigned int j;
		for(j = 0; ; j++){
			if(!current_component[j]){
				break;
			}
			unsigned_char_list_add_end(&list, current_component[j]);
		}
		unsigned_char_list_add_end(&list, '/');
	}

	/*  Add file we want to open */
	for(i = 0; ; i++){
		if(!file_to_open[i]){
			break;
		}
		unsigned_char_list_add_end(&list, file_to_open[i]);
	}

	/*  Null Terminator */
	unsigned_char_list_add_end(&list, 0);

	f = fopen((char *)unsigned_char_list_data(&list), "rb");

	/*  Output the file */
	while (c != EOF) {
		c = getc(f);
		if(c == EOF)
			break;
		printf("%c", c);
	}


	unsigned_char_ptr_list_destroy(&components_list);
	unsigned_char_list_destroy(&list);
	fclose(f);
	return 0;
}
