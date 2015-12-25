/*
    Copyright 2015 Robert Elder Software Inc.
    
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
#include "replace_tool.h"
#include <stdio.h>

void validate_replacement_strings(struct binary_exponential_buffer * searches, struct binary_exponential_buffer * replaces){
	unsigned int i;
	unsigned int j;
	unsigned char ** search_strs = (unsigned char **)binary_exponential_buffer_data(searches);
	/*  Make sure there number of searches and replaces match. */
	assert(binary_exponential_buffer_size(searches) == binary_exponential_buffer_size(replaces));
	/*  Make sure that no search string is a proper prefix of another search string. */
	for(i = 0; i < binary_exponential_buffer_size(searches); i++){
		for(j = (i + 1); j < binary_exponential_buffer_size(searches); j++){
			unsigned int chr_pos = 0;
			while(search_strs[i][chr_pos] == search_strs[j][chr_pos] || (search_strs[i][chr_pos] == 0 || search_strs[j][chr_pos] == 0)){
				if(search_strs[i][chr_pos] == 0 || search_strs[j][chr_pos] == 0){
					printf("The strings %s and %s have a common prefix", search_strs[i], search_strs[j]);
					assert(0 && "One of the search strings is a prefix of another.  This will lead to non-deterministic resultsed based on list ordering.");
				}
				chr_pos++;
			}
		}
	}
}

void reset_candidates(struct binary_exponential_buffer * candidates){
	unsigned int * p = (unsigned int *)binary_exponential_buffer_data(candidates);
	unsigned int num_candidates = binary_exponential_buffer_size(candidates);
	unsigned int i;
	for(i = 0; i < num_candidates; i++){
		p[i] = 1;
	}
}

unsigned int count_candidates(struct binary_exponential_buffer * candidates){
	unsigned int * p = (unsigned int *)binary_exponential_buffer_data(candidates);
	unsigned int num_candidates = binary_exponential_buffer_size(candidates);
	unsigned int i;
	unsigned int num_active = 0;
	for(i = 0; i < num_candidates; i++){
		if(p[i]){
			num_active++;
		}
	}
	return num_active;
}

unsigned int get_length(unsigned char * c){
	unsigned int i = 0;
	while(*c){
		c++;
		i++;
	}
	return i;
}

unsigned int set_lengths(struct binary_exponential_buffer * lengths, struct binary_exponential_buffer * searches){
	unsigned char ** search_strs = (unsigned char **)binary_exponential_buffer_data(searches);
	unsigned int * p = (unsigned int *)binary_exponential_buffer_data(lengths);
	unsigned int num_lengths = binary_exponential_buffer_size(lengths);
	unsigned int i;
	unsigned int longest = 0;
	for(i = 0; i < num_lengths; i++){
		unsigned int l = get_length(search_strs[i]);
		assert(l != 0);
		p[i] = l;
		if(l > longest){
			longest = l;
		}
	}
	return longest;
}

void do_string_replacements(struct binary_exponential_buffer * searches, struct binary_exponential_buffer * replaces, struct binary_exponential_buffer * in_characters, struct binary_exponential_buffer * out_characters){
	unsigned char * in = (unsigned char *)binary_exponential_buffer_data(in_characters);
	unsigned char ** search_strs = (unsigned char **)binary_exponential_buffer_data(searches);
	unsigned char ** replace_strs = (unsigned char **)binary_exponential_buffer_data(replaces);
	unsigned int num_searches = binary_exponential_buffer_size(searches);
	unsigned int num_characters = binary_exponential_buffer_size(in_characters);
	unsigned int in_chr_pos;
	unsigned int current_search;
	unsigned int longest_search_len;
	unsigned int * candidates;
	unsigned int * search_lengths;
	unsigned int * replace_lengths;
	struct binary_exponential_buffer candidates_buffer;
	struct binary_exponential_buffer search_lengths_buffer;
	struct binary_exponential_buffer replace_lengths_buffer;

	validate_replacement_strings(searches, replaces);

	binary_exponential_buffer_create(&candidates_buffer, sizeof(unsigned int));
	binary_exponential_buffer_increment(&candidates_buffer, num_searches);
	reset_candidates(&candidates_buffer);
	binary_exponential_buffer_create(&search_lengths_buffer, sizeof(unsigned int));
	binary_exponential_buffer_increment(&search_lengths_buffer, num_searches);
	longest_search_len = set_lengths(&search_lengths_buffer, searches);
	binary_exponential_buffer_create(&replace_lengths_buffer, sizeof(unsigned int));
	binary_exponential_buffer_increment(&replace_lengths_buffer, num_searches);
	set_lengths(&replace_lengths_buffer, replaces);

	candidates = (unsigned int *)binary_exponential_buffer_data(&candidates_buffer);
	search_lengths = (unsigned int *)binary_exponential_buffer_data(&search_lengths_buffer);
	replace_lengths = (unsigned int *)binary_exponential_buffer_data(&replace_lengths_buffer);

	/*  For every character of input */
	for(in_chr_pos = 0; in_chr_pos < num_characters; in_chr_pos++){
		unsigned int compare_index;
		unsigned match_obtained = 0;
		for(compare_index = 0; compare_index < longest_search_len; compare_index++){
			/*  For each string we're searching for */
			for(current_search = 0; current_search < num_searches; current_search++){
				/*  If this string is still a candidate match */
				if(candidates[current_search] && search_lengths[current_search] > compare_index){
					if((in_chr_pos + compare_index) < num_characters && in[in_chr_pos + compare_index] == search_strs[current_search][compare_index]){
						if((compare_index + 1) == search_lengths[current_search]){
							unsigned int size_before = binary_exponential_buffer_size(out_characters);
							unsigned int chars_copied = 0;
							unsigned char * copy_location;
							match_obtained = 1;
							/*  Resizing the buffer can change the location of data. */
							binary_exponential_buffer_increment(out_characters, replace_lengths[current_search]);
							copy_location = &(((unsigned char *)binary_exponential_buffer_data(out_characters))[size_before]);
							while(chars_copied < replace_lengths[current_search]){
								copy_location[chars_copied] = replace_strs[current_search][chars_copied];
								chars_copied++;
							}
							in_chr_pos += compare_index;
							compare_index = longest_search_len; /*  Forces us out of the outer loop */
							reset_candidates(&candidates_buffer);
							break;
						}
					}else{
						/*  This string failed to match, so it is no longer a candidate match */
						candidates[current_search] = 0;
					}
				}
			}
		}
		reset_candidates(&candidates_buffer);
		if(!match_obtained){
			unsigned int size_before = binary_exponential_buffer_size(out_characters);
			/*  Ourput the non-matching regular character */
			binary_exponential_buffer_increment(out_characters, 1);
			((unsigned char *)binary_exponential_buffer_data(out_characters))[size_before] = in[in_chr_pos];
		}
	}

	binary_exponential_buffer_destroy(&candidates_buffer);
	binary_exponential_buffer_destroy(&search_lengths_buffer);
	binary_exponential_buffer_destroy(&replace_lengths_buffer);
}
