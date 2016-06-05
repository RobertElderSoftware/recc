#ifndef REGEX_ENGINE_H_
#define REGEX_ENGINE_H_
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

#ifndef TYPES_REGEX_ENGINE_struct_regex_compiler_state_H_
#include "../generated/struct_regex_compiler_state.h"
#endif
#ifndef TYPES_REGEX_ENGINE_struct_regex_matcher_state_H_
#include "../generated/struct_regex_matcher_state.h"
#endif
#ifndef MEMORY_POOL_COLLECTION_H_
#include "memory_pool_collection.h"
#endif

/*
	NOTE:  There are probably subtle bugs in edge cases for this regex engine, as it hasn't been well tested on every regex it supports.

	This regex engine supports the following standard syntax features:

	CHARACTER CLASSES AND RANGES

	Ex: [a-zA-Z0-9]

	NEGATIVE CHARACTER CLASSES 

	Ex: [^a-zA-Z0-9]

	REPETITION 

	Ex:

	a? - Zero or once
	b* - Zero or more
	c+ - One or more

	GROUPING

	Ex: (a[b]+)

	OPTIONS

	Ex: (cat|dog)*

	SPECIAL CHARACTERS THAT REQUIRE ESCAPING

	\ ( ) [ ] + ?  * .  - ^

	OTHER CHARACTERS

	\n - escaped newline
	\t - escaped tab
	\xHH - Any hex character from 00-FF
	. -  Any non-newline character

	Backreferenes are currently not supported.  All matches are based on the longest match of the entire regex.

	This regex engine allows the user to associate multiple regexes (a 'struct regex_compiler_state')
	with a single regex matcher (a 'struct regex_matcher_state').  This is useful for tokenizing source
	code because a user can simply specify the regexes of each token, then add that regex to the matcher
	with a unique id, then see which ids matched.

	To use this, first create a struct regex_compiler_state, and a struct regex_matcher_state;

	struct regex_matcher_state m;
	struct regex_compiler_state c;
	struct regex_compiler_state * arr[1];

	Initialize the first regex:

	regex_compiler_state_create(c, m, "(abc)*", 0);

	Then add it to the matcher

	regex_matcher_state_create(c, &arr[0], 1);

	Now you can run it

	regex_matcher_state_run(c, "abc", 12345);

	See struct regex_matcher_state for details on how to access the results.
*/

/*
	void regex_compiler_state_create(struct regex_compiler_state * compiler, struct memory_pool_collection * m, unsigned char * r, unsigned int id);

	compiler:  The regex compiler state struct that is waiting to be initialized.

	m:  A pointer to a 'memory_pool_collection' which is used to indirectly call malloc and free.  You could pass
	in null for this parameter, if you replace any memory pool malloc/free calls with regular malloc/free.

	r:  A character pointer to the regex you can to compile.

	id:  A unique id from 0 to the number of compiler states, used to identify which regexes matched.

	Side effects:  If compillation of the regex fails, compiler->errors will be non-null, then use regex_compiler_state_explain.
*/
void regex_compiler_state_create(struct regex_compiler_state *, struct memory_pool_collection *, unsigned char *, unsigned int);


/*
	void regex_compiler_state_explain(struct regex_compiler_state * compiler);

	compiler:  The regex compiler state struct that was previously initialized.

	This function prints out a description of the compiled regex and lists details about each node in the regex computation.
*/
void regex_compiler_state_explain(struct regex_compiler_state *);


/*
	void regex_compiler_state_compile(struct regex_compiler_state * compiler);

	compiler:  The regex compiler state struct that was previously initialized.
*/
void regex_compiler_state_compile(struct regex_compiler_state *);


/*
	void regex_compiler_state_destroy(struct regex_compiler_state * compiler);

	compiler:  The regex compiler state struct that was previously initialized, and needs to be destroyed.
*/
void regex_compiler_state_destroy(struct regex_compiler_state *);


/*
	void regex_matcher_state_create(struct regex_matcher_state * matcher, struct regex_compiler_state ** states, unsigned int len);

	matcher:  A pointer to a empty matcher struct waiting to be initialized.

	states: An array of pointers to regex compiler states, all of which have been compiled successfully previously.

	len:  The number of regex compiler states pointed to by the 'states' pointer.
*/
void regex_matcher_state_create(struct regex_matcher_state *, struct regex_compiler_state **, unsigned int);


/*
	void regex_matcher_state_destroy(struct regex_matcher_state * matcher);

	matcher:  A pointer to a matcher struct waiting to be destroyed.
*/
void regex_matcher_state_destroy(struct regex_matcher_state *);


/*
	void regex_matcher_state_run(struct regex_matcher_state * matcher, unsigned char * str, unsigned int max_len);

	matcher:  A pointer to a matcher struct that has been initialized.

	str:  A pointer to the character array you wish to run the list of regexes against.

	max_len:  The maximum number of characters that can safely been read from the buffer.
*/
void regex_matcher_state_run(struct regex_matcher_state *, unsigned char *, unsigned int);

#endif
