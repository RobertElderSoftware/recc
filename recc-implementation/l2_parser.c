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

#include "l2_parser.h"

void * push_l2_operation(struct l2_parser_state *, enum l2_parser_operation_type, void *);
void pop_l2_operation(struct l2_parser_state *);
void l2_backtrack(struct l2_parser_state *, unsigned int);
struct l2_parser_node * l2_accept(enum l2_token_type, struct l2_parser_state *);
unsigned int count_newlines_in_l2_comment(struct l2_lexer_token *);
struct l2_parser_node * create_l2_parser_node(struct l2_parser_state *, struct l2_parser_node *, struct l2_parser_node *, struct l2_lexer_token *, enum l2_node_type);
void l2_parser_progress(const char*, ...);

struct l2_parser_node * make_l2_epsilon(struct l2_parser_state *);
struct l2_parser_node * l2_after_statement(struct l2_parser_state *);
struct l2_parser_node * l2_statement(struct l2_parser_state * state);
struct l2_parser_node * l2_statement_list(struct l2_parser_state *);
struct l2_parser_node * instruction_statement(struct l2_parser_state *);
struct l2_parser_node * l2_file(struct l2_parser_state *);
struct l2_parser_node * label_statement(struct l2_parser_state * state);
struct l2_parser_node * directive_statement(struct l2_parser_state * state);
struct l2_parser_node * define_word_directive(struct l2_parser_state * state);
struct l2_parser_node * skip_words_directive(struct l2_parser_state * state);
struct l2_parser_node * offset_directive(struct l2_parser_state * state);
struct l2_parser_node * symbol_linkage_declaration(struct l2_parser_state *);
struct l2_parser_node * linkage_domain(struct l2_parser_state *);
struct l2_parser_node * interface_specifier(struct l2_parser_state *);
struct l2_parser_node * branch_distance(struct l2_parser_state *);
struct l2_parser_node * function_directive(struct l2_parser_state *);
struct l2_parser_node * variable_directive(struct l2_parser_state *);
struct l2_parser_node * constant_directive(struct l2_parser_state *);
struct l2_parser_node * string_directive(struct l2_parser_state *);
struct l2_parser_node * implemented_directive(struct l2_parser_state *);
struct l2_parser_node * required_directive(struct l2_parser_state *);
struct l2_parser_node * unresolved_directive(struct l2_parser_state *);
struct l2_parser_node * linkage_directive(struct l2_parser_state *);
struct l2_parser_node * region_directive(struct l2_parser_state *);
struct l2_parser_node * start_directive(struct l2_parser_state *);
struct l2_parser_node * end_directive(struct l2_parser_state *);
struct l2_parser_node * permission_directive(struct l2_parser_state *);

static const char * l2_node_type_names[29] = {
	"L2_TERMINAL",
	"L2_EPSILON",
	"L2_INSTRUCTION",
	"L2_FILE",
	"L2_AFTER_STATEMENT",
	"L2_STATEMENT_LIST",
	"L2_STATEMENT",
	"L2_OFFSET_DIRECTIVE",
	"L2_DEFINE_WORD_DIRECTIVE",
	"L2_SKIP_WORDS_DIRECTIVE",
	"L2_INSTRUCTION_STATEMENT",
	"L2_DIRECTIVE_STATEMENT",
	"L2_LABEL_STATEMENT",
	"L2_LINKAGE_DOMAIN",
	"L2_INTERFACE_SPECIFIER",
	"L2_BRANCH_DISTANCE",
	"L2_SYMBOL_LINKAGE_DECLARATION",
	"L2_FUNCTION_DIRECTIVE",
	"L2_VARIABLE_DIRECTIVE",
	"L2_CONSTANT_DIRECTIVE",
	"L2_STRING_DIRECTIVE",
	"L2_IMPLEMENTED_DIRECTIVE",
	"L2_REQUIRED_DIRECTIVE",
	"L2_UNRESOLVED_DIRECTIVE",
	"L2_LINKAGE_DIRECTIVE",
	"L2_REGION_DIRECTIVE",
	"L2_START_DIRECTIVE",
	"L2_END_DIRECTIVE",
	"L2_PERMISSION_DIRECTIVE"
};

void l2_parser_progress(const char* format, ...){
	va_list arglist;
	va_start( arglist, format );
	/*
	if(1){
		printf(";");
		vprintf( format, arglist );
	}
	*/
	va_end( arglist );
}

unsigned int count_newlines_in_l2_comment(struct l2_lexer_token * t){
	unsigned int count = 0;
	unsigned char * c = t->first_byte;
	while(c != t->last_byte){
		if(*c == '\n'){
			count++;
		}
		c++;
	}
	return count;
}

struct l2_parser_node * offset_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build offset_directive\n");
	if((n = l2_accept(L2_OFFSET, state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			l2_parser_progress("built offset hex\n");
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_OFFSET_DIRECTIVE);
		}else if((n->next = l2_accept(L2_RELOCATABLE, state))){
			l2_parser_progress("built offset reloc\n");
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_OFFSET_DIRECTIVE);
		}else if((n->next = l2_accept(L2_END, state))){
			l2_parser_progress("built offset reloc\n");
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_OFFSET_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber or relocatable.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * skip_words_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build skip_words_directive\n");
	if((n = l2_accept(L2_SW, state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_SKIP_WORDS_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * define_word_directive(struct l2_parser_state * state){
        struct l2_parser_node * n;
        l2_parser_progress("Attempting to build define_word_directive\n");
        if((n = l2_accept(L2_DW, state))){
                if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
                        return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DEFINE_WORD_DIRECTIVE);
                }else if((n->next = l2_accept(L2_IDENTIFIER, state))){
                        return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DEFINE_WORD_DIRECTIVE);
                }else{
                        assert(0 && "Expected hexnumber.");
                }
        }else{
                return (struct l2_parser_node *)0;
        }
}

struct l2_parser_node * linkage_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build linkage_directive\n");
	if((n = linkage_domain(state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_LINKAGE_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * string_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build string_directive\n");
	if((n = l2_accept(L2_STRING, state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_STRING_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * implemented_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build implemented_directive\n");
	if((n = l2_accept(L2_IMPLEMENTED, state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_IMPLEMENTED_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * required_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build required_directive\n");
	if((n = l2_accept(L2_REQUIRED, state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_REQUIRED_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * unresolved_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build unresolved_directive\n");
	if((n = l2_accept(L2_UNRESOLVED, state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_UNRESOLVED_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * region_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build region_directive\n");
	if((n = l2_accept(L2_REGION, state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_REGION_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * start_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build start_directive\n");
	if((n = l2_accept(L2_START, state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_START_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * end_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build end_directive\n");
	if((n = l2_accept(L2_END, state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_END_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * permission_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build permission_directive\n");
	if((n = l2_accept(L2_PERMISSION, state))){
		if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_PERMISSION_DIRECTIVE);
		}else{
			assert(0 && "Expected hexnumber.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * interface_specifier(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build interface_specifier\n");
	if((n = l2_accept(L2_IMPLEMENTS, state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_INTERFACE_SPECIFIER);
	}else if((n = l2_accept(L2_REQUIRES, state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_INTERFACE_SPECIFIER);
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * linkage_domain(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build linkage_domain\n");
	if((n = l2_accept(L2_INTERNAL, state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_LINKAGE_DOMAIN);
	}else if((n = l2_accept(L2_EXTERNAL, state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_LINKAGE_DOMAIN);
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * function_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build function_directive\n");
	if((n = l2_accept(L2_FUNCTION, state))){
		if((n->next = l2_accept(L2_IDENTIFIER, state))){
			if((n->next->next = l2_accept(L2_IDENTIFIER, state))){
				return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_FUNCTION_DIRECTIVE);
			}else{
				assert(0 && "Expected identifier.");
			}
		}else if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_FUNCTION_DIRECTIVE);
		}else{
			assert(0 && "Expected identifier or hex constant.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * variable_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build variable_directive\n");
	if((n = l2_accept(L2_VARIABLE, state))){
		if((n->next = l2_accept(L2_IDENTIFIER, state))){
			if((n->next->next = l2_accept(L2_IDENTIFIER, state))){
				return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_VARIABLE_DIRECTIVE);
			}else{
				assert(0 && "Expected identifier.");
			}
		}else if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_VARIABLE_DIRECTIVE);
		}else{
			assert(0 && "Expected identifier or hex constant.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * constant_directive(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build constant_directive\n");
	if((n = l2_accept(L2_CONSTANT, state))){
		if((n->next = l2_accept(L2_IDENTIFIER, state))){
			if((n->next->next = l2_accept(L2_IDENTIFIER, state))){
				return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_CONSTANT_DIRECTIVE);
			}else{
				assert(0 && "Expected identifier.");
			}
		}else if((n->next = l2_accept(L2_CONSTANT_HEX, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_CONSTANT_DIRECTIVE);
		}else{
			assert(0 && "Expected identifier or hex constant.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * symbol_linkage_declaration(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build symbol_linkage_declaration\n");
	if((n = interface_specifier(state))){
		if((n->next = linkage_domain(state))){
			if((n->next->next = l2_accept(L2_IDENTIFIER, state))){
				return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_SYMBOL_LINKAGE_DECLARATION);
			}else{
				assert(0 && "Expected identifier.");
			}
		}else if((n->next = l2_accept(L2_COMMA_CHAR, state))){
			if((n->next->next = interface_specifier(state))){
				if((n->next->next->next = linkage_domain(state))){
					if((n->next->next->next->next = l2_accept(L2_IDENTIFIER, state))){
						return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_SYMBOL_LINKAGE_DECLARATION);
					}else{
						assert(0 && "Expected identifier.");
					}
				}else{
					assert(0 && "Expected linkage domain.");
				}
			}else{
				assert(0 && "Expected interface specifier.");
			}
		}else{
			assert(0 && "Expected comma or linkage domain.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * directive_statement(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build directive_statement\n");
	if((n = offset_directive(state))){
		l2_parser_progress("made directive\n");
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = define_word_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = skip_words_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = function_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = variable_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = constant_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = string_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = implemented_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = required_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = unresolved_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = linkage_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = region_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = start_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = end_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = permission_directive(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else if((n = symbol_linkage_declaration(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_DIRECTIVE_STATEMENT);
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * branch_distance(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build branch_distance\n");
	if((n = l2_accept(L2_IDENTIFIER, state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_BRANCH_DISTANCE);
	}else if((n = l2_accept(L2_CONSTANT_DECIMAL, state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_BRANCH_DISTANCE);
	}else if((n = l2_accept(L2_MINUS_CHAR, state))){
		if((n->next = l2_accept(L2_CONSTANT_DECIMAL, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_BRANCH_DISTANCE);
		}else{
			printf("Parser could not continue on line %d of file %s\n", state->line_number, state->l2_lexer_state->c.filename);
			assert(0 && "Expected constant decimal.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * instruction_statement(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build instruction_statement\n");
	if((n = l2_accept(L2_ADD, state)) || (n = l2_accept(L2_SUB, state)) || (n = l2_accept(L2_MUL, state)) || (n = l2_accept(L2_DIV, state)) || (n = l2_accept(L2_AND, state)) || (n = l2_accept(L2_OR, state))){
		if((n->next = l2_accept(L2_REGISTER, state))){
			if((n->next->next = l2_accept(L2_REGISTER, state))){
				if((n->next->next->next = l2_accept(L2_REGISTER, state))){
					return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_INSTRUCTION_STATEMENT);
				}else{
					assert(0 && "Expected register.");
				}
			}else{
				assert(0 && "Expected register.");
			}
		}else{
			assert(0 && "Expected register.");
		}
	}else if((n = l2_accept(L2_NOT, state)) || (n = l2_accept(L2_LOA, state)) || (n = l2_accept(L2_STO, state)) || (n = l2_accept(L2_SHR, state)) || (n = l2_accept(L2_SHL, state))){
		if((n->next = l2_accept(L2_REGISTER, state))){
			if((n->next->next = l2_accept(L2_REGISTER, state))){
				return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_INSTRUCTION_STATEMENT);
			}else{
				assert(0 && "Expected register.");
			}
		}else{
			assert(0 && "Expected register.");
		}
	}else if((n = l2_accept(L2_BEQ, state)) || (n = l2_accept(L2_BLT, state))){
		if((n->next = l2_accept(L2_REGISTER, state))){
			if((n->next->next = l2_accept(L2_REGISTER, state))){
				if((n->next->next->next = branch_distance(state))){
					return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_INSTRUCTION_STATEMENT);
				}else{
					printf("Parser could not continue on line %d of file %s\n", state->line_number, state->l2_lexer_state->c.filename);
					assert(0 && "Expected constant decimal.");
				}
			}else{
				assert(0 && "Expected register.");
			}
		}else{
			assert(0 && "Expected register.");
		}
	}else if((n = l2_accept(L2_LL, state))){
		if((n->next = l2_accept(L2_REGISTER, state))){
			if((n->next->next = l2_accept(L2_CONSTANT_HEX, state))){
				return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_INSTRUCTION_STATEMENT);
			}else{
				assert(0 && "Expected constant hex.");
			}
		}else{
			assert(0 && "Expected register.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * label_statement(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build label_statement\n");
	if((n = l2_accept(L2_IDENTIFIER, state))){
		if((n->next = l2_accept(L2_COLON_CHAR, state))){
			return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_LABEL_STATEMENT);
		}else{
			printf("Parser could not continue on line %d of file %s\n", state->line_number, state->l2_lexer_state->c.filename);
			assert(0 && "Expected colon.");
		}
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * l2_statement(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build l2_statement\n");
	if((n = instruction_statement(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_STATEMENT);
	}else if((n = directive_statement(state))){
		l2_parser_progress("made statement\n");
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_STATEMENT);
	}else if((n = label_statement(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token *)0, L2_STATEMENT);
	}else{
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * l2_statement_list(struct l2_parser_state * state){
	struct l2_parser_node * result;
	struct l2_parser_node ** old_prev = &result;
	struct l2_parser_node ** new_prev = (struct l2_parser_node **)0;
	l2_parser_progress("Attempting to build l2_statement_list\n");

	do{
		struct l2_parser_node * statement;
		struct l2_parser_node * after;
		struct l2_parser_node * new_node;
		statement = l2_statement(state);

		if((after = l2_accept(L2_NEWLINE, state))){
			new_prev = &after->next;
		}else if((after = l2_accept(L2_COMMENT, state))){
			if((after->next = l2_accept(L2_NEWLINE, state))){
				new_prev = &after->next->next;
			}
		}

		if(statement){
			assert(after && "No after statement.");
			statement->next = create_l2_parser_node(state, (struct l2_parser_node *)0, after, (struct l2_lexer_token *)0, L2_AFTER_STATEMENT);
			new_node = statement;
		}else if(after){
			new_node = create_l2_parser_node(state, (struct l2_parser_node *)0, after, (struct l2_lexer_token *)0, L2_AFTER_STATEMENT);
		}else{
			new_node = make_l2_epsilon(state);
			new_prev = (struct l2_parser_node **)0;
		}
		*old_prev = create_l2_parser_node(state, (struct l2_parser_node *)0, new_node, (struct l2_lexer_token *)0, L2_STATEMENT_LIST);
		old_prev = new_prev;
	}while(new_prev);
	return result;
}

struct l2_parser_node * make_l2_epsilon(struct l2_parser_state * state){
	l2_parser_progress("Attempting to build l2 epsilon\n");
	return create_l2_parser_node(state, (struct l2_parser_node *)0, (struct l2_parser_node *)0, (struct l2_lexer_token *)0, L2_EPSILON);
}

struct l2_parser_node * l2_file(struct l2_parser_state * state){
	struct l2_parser_node * n;
	l2_parser_progress("Attempting to build l2_file\n");
	if((n = l2_statement_list(state))){
		return create_l2_parser_node(state, (struct l2_parser_node *)0, n, (struct l2_lexer_token*)0, L2_FILE);
	}else{
		printf("Parser could not continue on line %d of file %s\n", state->line_number, state->l2_lexer_state->c.filename);
		assert(0 && "Expected statement_list.\n");
		return (struct l2_parser_node *)0;
	}
}

struct l2_parser_node * create_l2_parser_node(struct l2_parser_state * parser_state, struct l2_parser_node * n, struct l2_parser_node * f, struct l2_lexer_token * l, enum l2_node_type t){
	struct l2_parser_node new_node;
	new_node.next = n;
	new_node.first_child = f;
	new_node.l2_lexer_token = l;
	new_node.type = t;
	return (struct l2_parser_node *)push_l2_operation(parser_state, L2_ADVANCE_PARSER_POSITION, (void*)&new_node);
}

struct l2_parser_node * l2_accept(enum l2_token_type t, struct l2_parser_state * parser_state){
	unsigned int num_tokens = struct_l2_lexer_token_ptr_list_size(&parser_state->l2_lexer_state->tokens);
	struct l2_lexer_token ** tokens = struct_l2_lexer_token_ptr_list_data(&parser_state->l2_lexer_state->tokens);
	while(
		(parser_state->tokens_position < num_tokens) &&
		(
			tokens[parser_state->tokens_position]->type == L2_SPACE
		)
	){
		push_l2_operation(parser_state, L2_ADVANCE_TOKEN_POSITION, (void*)tokens[parser_state->tokens_position]);
	}

	if(parser_state->tokens_position < num_tokens){
		if(tokens[parser_state->tokens_position]->type == L2_NEWLINE){
			push_l2_operation(parser_state, L2_INCREMENT_NEWLINE_COUNT, (void*)0);
		}else if(tokens[parser_state->tokens_position]->type == L2_COMMENT){
			unsigned int num_newlines = count_newlines_in_l2_comment(tokens[parser_state->tokens_position]);
			unsigned int i;
			for(i = 0; i < num_newlines; i++){
				push_l2_operation(parser_state, L2_INCREMENT_NEWLINE_COUNT, (void*)0);
			}
		}

		if(tokens[parser_state->tokens_position]->type == t){
			struct l2_lexer_token * lt;
			lt = tokens[parser_state->tokens_position];
		        push_l2_operation(parser_state, L2_ADVANCE_TOKEN_POSITION, (void*)tokens[parser_state->tokens_position]);
			return create_l2_parser_node(parser_state, (struct l2_parser_node *)0, (struct l2_parser_node *)0, lt, L2_TERMINAL);
		}
	}
	return (struct l2_parser_node *)0;
}

void * push_l2_operation(struct l2_parser_state * parser_state, enum l2_parser_operation_type t, void * data){
	struct l2_parser_operation new_operation;
	new_operation.type = t;
	new_operation.data = t == L2_ADVANCE_PARSER_POSITION ? struct_l2_parser_node_memory_pool_malloc(parser_state->memory_pool_collection->struct_l2_parser_node_pool) : data;
	struct_l2_parser_operation_stack_push(&parser_state->operation_stack, new_operation);
	switch(t){
		case L2_ADVANCE_TOKEN_POSITION:{
			parser_state->tokens_position = parser_state->tokens_position + 1;
			return (void *)0;
		}case L2_ADVANCE_PARSER_POSITION:{
			*((struct l2_parser_node *)new_operation.data) = *((struct l2_parser_node *)data);
			return new_operation.data;
		}case L2_INCREMENT_NEWLINE_COUNT:{
			parser_state->line_number = parser_state->line_number + 1;
			return (void *)0;
		}default:{
			assert(0 && "This should never happen.\n");
			return (void *)0;
		}
	}
}

void pop_l2_operation(struct l2_parser_state * parser_state){
	struct l2_parser_operation poped_operation;
	if(struct_l2_parser_operation_stack_size(&parser_state->operation_stack) == 0){
		assert(0 && "Trying to pop empty operation stack.\n");
		return;
	}
	poped_operation = struct_l2_parser_operation_stack_pop(&parser_state->operation_stack);
	
	switch(poped_operation.type){
		case L2_ADVANCE_TOKEN_POSITION:{
			parser_state->tokens_position = parser_state->tokens_position - 1;
			break;
		}case L2_ADVANCE_PARSER_POSITION:{
			struct_l2_parser_node_memory_pool_free(parser_state->memory_pool_collection->struct_l2_parser_node_pool, (struct l2_parser_node*)poped_operation.data);
			break;
		}case L2_INCREMENT_NEWLINE_COUNT:{
			parser_state->line_number = parser_state->line_number - 1;
			break;
		}default:{
			assert(0 && "This should never happen.\n");
		}
	}
}

void l2_backtrack(struct l2_parser_state * parser_state, unsigned int target){
	while(struct_l2_parser_operation_stack_size(&parser_state->operation_stack) != target){
		pop_l2_operation(parser_state);
	}
}

int parse_l2(struct l2_parser_state * state){
	state->top_node = l2_file(state);
	if(state->tokens_position == struct_l2_lexer_token_ptr_list_size(&state->l2_lexer_state->tokens)){
	}else{
		printf("L2 Parser could not continue on line %d of file %s\n", state->line_number, state->l2_lexer_state->c.filename);
		assert(0 && "Parsing failed.");
	}
	return 0;
}

void create_l2_parser_state(struct l2_parser_state * state, struct memory_pool_collection * memory_pool_collection, struct l2_lexer_state * l2_lexer_state, struct unsigned_char_list * buffered_output, unsigned char * buffer){
	state->buffered_output = buffered_output;
	state->memory_pool_collection = memory_pool_collection;
	state->l2_lexer_state = l2_lexer_state;
	state->line_number = 1;
	state->tokens_position = 0;
	state->buff = buffer;
	struct_l2_parser_operation_stack_create(&state->operation_stack);
}

void destroy_l2_parser_state(struct l2_parser_state * state){
	l2_backtrack(state, 0);
	struct_l2_parser_operation_stack_destroy(&state->operation_stack);
}

const char ** get_l2_node_type_names(void){
	return (const char **)l2_node_type_names;
}
