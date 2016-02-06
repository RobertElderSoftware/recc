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
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "regex_engine.h"

/*

LL grammar for the regexes that the regex compiler supports:

hex_digit
	0-9
	A-F
	a-f

escapable_character
	'\'
	'n'
	't'
	's'
	'('
	')'
	'['
	']'
	'+'
	'?'
	'*'
	'.'
	'-'
	'^'
	'x' hex_digit hex_digit

escaped_character
	'\' escapable_character

character_range
	non_class_character '-' non_class_character

non_class_character
	escaped_character
	' ' to '''
	','
	'/' to '>'
	'@' to 'Z'
	'_' to '{'
	'}' to '~'

character_or_range
	character_range
	non_class_character
	'.'

character_or_class
	character_class
	non_class_character
	'.'

character_class_rest
	character_or_range character_class_rest
	']'

character_class
	'[' character_or_range character_class_rest
	'[' '^' character_or_range character_class_rest

character_sequence_rest
	character_or_class character_sequence_rest
	epsilon

character_sequence
	character_or_class character_sequence_rest

postfix_operator
	'?'
	'+'
	'*'

infix_expression_rest
	'|' regular_expression infix_expression_rest
	')'

infix_expression
	'(' regular_expression infix_expression_rest

postfix_expression
	character_sequence postfix_operation
	character_sequence
	infix_expression postfix_operation
	infix_expression

regular_expression
	postfix_expression regular_expression
	epsilon

*/

static struct regex_parser_node * postfix_operator(struct regex_compiler_state *);
static struct regex_parser_node * regex_postfix_expression(struct regex_compiler_state *);
static struct regex_parser_node * infix_expression_rest(struct regex_compiler_state *);
static struct regex_parser_node * infix_expression(struct regex_compiler_state *);
static struct regex_parser_node * regular_expression(struct regex_compiler_state *);
static struct regex_parser_node * character_sequence(struct regex_compiler_state *);
static struct regex_parser_node * character_or_range(struct regex_compiler_state *);
static struct regex_parser_node * character_or_class(struct regex_compiler_state *);
static struct regex_parser_node * escapable_character(struct regex_compiler_state *);
static struct regex_parser_node * escaped_character(struct regex_compiler_state *);
static struct regex_parser_node * character_range(struct regex_compiler_state *);
static struct regex_parser_node * character_class(struct regex_compiler_state *);
static struct regex_parser_node * non_class_character(struct regex_compiler_state *);


static struct regex_computation_node * reg_postfix_expression(struct regex_compiler_state *, struct regex_parser_node *, struct regex_computation_node *);
static struct regex_computation_node * reg_infix_expression(struct regex_compiler_state *, struct regex_parser_node *, struct regex_computation_node *);
static struct regex_computation_node * reg_regular_expression(struct regex_compiler_state *, struct regex_parser_node *, struct regex_computation_node *);
static void explain_error(struct regex_compiler_state *);
static void regex_matcher_state_optimize(struct regex_matcher_state *, unsigned char);
static void regex_compiler_state_parse(struct regex_compiler_state *);


static void destroy_errors(struct regex_compiler_error * n);
static void destroy_regex_computation_nodes(struct regex_compiler_state *, struct regex_computation_node *);

void regex_compiler_state_create(struct regex_compiler_state * state, struct memory_pool_collection * m, unsigned char * buffer, unsigned int id){
	state->m = m;
	state->position = 0;
	state->buffer = buffer;
	state->top_node = (struct regex_parser_node *)0;
	state->buffer_length = (unsigned int)strlen((char *)buffer);
	state->errors = (struct regex_compiler_error *)0;
	state->match_id = id;
}

static struct regex_parser_node * create_regex_parser_node(struct regex_compiler_state * state, struct regex_parser_node * n, struct regex_parser_node * f, unsigned char c, enum regex_parser_node_type t){
	struct regex_parser_node * new_node = struct_regex_parser_node_memory_pool_malloc(state->m->struct_regex_parser_node_pool);
	new_node->next = n;
	new_node->first_child = f;
	new_node->type = t;
	new_node->c = c;
	return new_node;
}

static void destroy_regex_parser_node_tree(struct regex_compiler_state * state, struct regex_parser_node * n){
	if(n){
		destroy_regex_parser_node_tree(state, n->next);
		destroy_regex_parser_node_tree(state, n->first_child);
		struct_regex_parser_node_memory_pool_free(state->m->struct_regex_parser_node_pool, n);
	}
}

void regex_compiler_state_destroy(struct regex_compiler_state * state){
	destroy_regex_parser_node_tree(state, state->top_node);
	destroy_regex_computation_nodes(state, state->first_computation_node);
	destroy_errors(state->errors);
}

static struct regex_parser_node * regex_accept_range(struct regex_compiler_state * state, unsigned char low, unsigned char high){
	if(state->position < state->buffer_length){
		if((state->buffer[state->position] >= low) && (state->buffer[state->position] <= high)){
			unsigned char c = state->buffer[state->position];
			state->position += 1;
			return create_regex_parser_node(state, (struct regex_parser_node *)0, (struct regex_parser_node *)0, c, REGEX_TERMINAL);
		}
	}
	return (struct regex_parser_node *)0;
}


static void set_error_state(struct regex_compiler_state * state, enum regex_compiler_error_type t){
	struct regex_compiler_error * er = (struct regex_compiler_error *)malloc(sizeof(struct regex_compiler_error));
	er->next = (struct regex_compiler_error *)0;
	er->type = t;
	er->character_index_of_error = state->position;
	if(state->errors){
		struct regex_compiler_error * c = state->errors;
		while(c->next) {
			c = c->next;
		}
		c->next = er; /*  Add it to the end */
	}else{
		state->errors = er; /*  First one */
	}
}

static struct regex_parser_node * regex_accept(struct regex_compiler_state * state, unsigned char c){
	return regex_accept_range(state, c, c);
}

static struct regex_parser_node * regex_make_epsilon(struct regex_compiler_state * state){
	return create_regex_parser_node(state, (struct regex_parser_node *)0, (struct regex_parser_node *)0, 0, REGEX_EPSILON);
}

static struct regex_parser_node * regex_hex_digit(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = regex_accept_range(state, '0', '9'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_HEX_DIGIT);
	}else if((n = regex_accept_range(state, 'A', 'F'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_HEX_DIGIT);
	}else if((n = regex_accept_range(state, 'a', 'f'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_HEX_DIGIT);
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * escapable_character(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = regex_accept(state, 'n'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, 't'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, '('))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, ')'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, '['))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, ']'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, '+'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, '?'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, '*'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, '.'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, '-'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, '^'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, '|'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, '\\'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
	}else if((n = regex_accept(state, 'x'))){ /*  Escaped hex character */
		if((n->next = regex_hex_digit(state))){
			if((n->next->next = regex_hex_digit(state))){
				return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPABLE_CHARACTER);
			}else{
				set_error_state(state, REGEX_ERROR_EXPECTED_HEX_DIGIT);
				destroy_regex_parser_node_tree(state, n);
				return (struct regex_parser_node *)0;
			}
		}else{
			set_error_state(state, REGEX_ERROR_EXPECTED_HEX_DIGIT);
			destroy_regex_parser_node_tree(state, n);
			return (struct regex_parser_node *)0;
		}
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * escaped_character(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = regex_accept(state, '\\'))){
		if((n->next = escapable_character(state))){
			return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_ESCAPED_CHARACTER);
		}else{
			set_error_state(state, REGEX_ERROR_NOT_ESCAPABLE);
			destroy_regex_parser_node_tree(state, n);
			return (struct regex_parser_node *)0;
		}
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * character_range(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	unsigned int previous_position = state->position;
	if(state->errors){ return n; }
	if((n = non_class_character(state))){
		if((n->next = regex_accept(state, '-'))){
			if((n->next->next = non_class_character(state))){
				return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_RANGE);
			}else{
				set_error_state(state, REGEX_ERROR_INVALID_CHARACTER_RANGE);
				destroy_regex_parser_node_tree(state, n);
				return (struct regex_parser_node *)0;
			}
		}else{
			/*  Need to backtrack... */
			state->position = previous_position;
			destroy_regex_parser_node_tree(state, n);
			return (struct regex_parser_node *)0;
		}
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * character_or_range(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = character_range(state))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_OR_RANGE);
	}else if((n = non_class_character(state))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_OR_RANGE);
	}else if((n = regex_accept(state, '.'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_OR_RANGE);
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * character_or_class(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = character_class(state))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_OR_CLASS);
	}else if((n = non_class_character(state))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_OR_CLASS);
	}else if((n = regex_accept(state, '.'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_OR_CLASS);
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * character_class_rest(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = character_or_range(state))){
		if((n->next = character_class_rest(state))){
			return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_CLASS_REST);
		}else{
			set_error_state(state, REGEX_ERROR_INVALID_CHARACTER_CLASS);
			destroy_regex_parser_node_tree(state, n);
			return (struct regex_parser_node *)0;
		}
	}else if((n = regex_accept(state, ']'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_CLASS_REST);
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * character_class(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = regex_accept(state, '['))){
		if((n->next = regex_accept(state, '^'))){
			if((n->next->next = character_or_range(state))){
				if((n->next->next->next = character_class_rest(state))){
					return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_CLASS);
				}else{
					set_error_state(state, REGEX_ERROR_INVALID_CHARACTER_CLASS);
					destroy_regex_parser_node_tree(state, n);
					return (struct regex_parser_node *)0;
				}
			}else{
				set_error_state(state, REGEX_ERROR_INVALID_CHARACTER_CLASS);
				destroy_regex_parser_node_tree(state, n);
				return (struct regex_parser_node *)0;
			}
		}else if((n->next = character_or_range(state))){
			if((n->next->next = character_class_rest(state))){
				return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_CLASS);
			}else{
				set_error_state(state, REGEX_ERROR_INVALID_CHARACTER_CLASS);
				destroy_regex_parser_node_tree(state, n);
				return (struct regex_parser_node *)0;
			}
		}else{
			set_error_state(state, REGEX_ERROR_INVALID_CHARACTER_CLASS);
			destroy_regex_parser_node_tree(state, n);
			return (struct regex_parser_node *)0;
		}
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * non_class_character(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = escaped_character(state))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_NON_CLASS_CHARACTER);
	/*  Normal characters:  Ranges carefully created to avoid special control characters */
	}else if((n = regex_accept_range(state, ' ', '\''))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_NON_CLASS_CHARACTER);
	}else if((n = regex_accept(state, ','))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_NON_CLASS_CHARACTER);
	}else if((n = regex_accept_range(state, '/', '>'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_NON_CLASS_CHARACTER);
	}else if((n = regex_accept_range(state, '@', 'Z'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_NON_CLASS_CHARACTER);
	}else if((n = regex_accept_range(state, '_', '{'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_NON_CLASS_CHARACTER);
	}else if((n = regex_accept_range(state, '}', '~'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_NON_CLASS_CHARACTER);
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * character_sequence_rest(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = character_or_class(state))){
		if((n->next = character_sequence_rest(state))){
			return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_SEQUENCE_REST);
		}else{
			set_error_state(state, REGEX_ERROR_INVALID_CHARACTER_SEQUENCE);
			destroy_regex_parser_node_tree(state, n);
			return (struct regex_parser_node *)0;
		}
	}else{
		if(state->errors){ return n; }
		return create_regex_parser_node(state, (struct regex_parser_node *)0, regex_make_epsilon(state), 0, REGEX_CHARACTER_SEQUENCE_REST);
	}
}

static struct regex_parser_node * character_sequence(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = character_or_class(state))){
		if((n->next = character_sequence_rest(state))){
			return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_CHARACTER_SEQUENCE);
		}else{
			set_error_state(state, REGEX_ERROR_INVALID_CHARACTER_SEQUENCE);
			destroy_regex_parser_node_tree(state, n);
			return (struct regex_parser_node *)0;
		}
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * postfix_operator(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = regex_accept(state, '+'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_POSTFIX_OPERATOR);
	}else if((n = regex_accept(state, '?'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_POSTFIX_OPERATOR);
	}else if((n = regex_accept(state, '*'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_POSTFIX_OPERATOR);
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * infix_expression_rest(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = regex_accept(state, '|'))){
		if((n->next = regular_expression(state))){
			if((n->next->next = infix_expression_rest(state))){
				return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_INFIX_EXPRESSION_REST);
			}else{
				set_error_state(state, REGEX_ERROR_EXPECTED_INFIX_EXPRESSION_REST);
				destroy_regex_parser_node_tree(state, n);
				return (struct regex_parser_node *)0;
			}
		}else{
			set_error_state(state, REGEX_ERROR_INVALID_REGULAR_EXPRESSION);
			destroy_regex_parser_node_tree(state, n);
			return (struct regex_parser_node *)0;
		}
	}else if((n = regex_accept(state, ')'))){
		return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_INFIX_EXPRESSION_REST);
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * infix_expression(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = regex_accept(state, '('))){
		if((n->next = regular_expression(state))){
			if((n->next->next = infix_expression_rest(state))){
				return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_INFIX_EXPRESSION);
			}else{
				set_error_state(state, REGEX_ERROR_EXPECTED_INFIX_EXPRESSION_REST);
				destroy_regex_parser_node_tree(state, n);
				return (struct regex_parser_node *)0;
			}
		}else{
			set_error_state(state, REGEX_ERROR_INVALID_REGULAR_EXPRESSION);
			destroy_regex_parser_node_tree(state, n);
			return (struct regex_parser_node *)0;
		}
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * regex_postfix_expression(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = character_sequence(state))){
		if((n->next = postfix_operator(state))){
			return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_POSTFIX_EXPRESSION);
		}else{
			return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_POSTFIX_EXPRESSION);
		}
	}else if((n = infix_expression(state))){
		if((n->next = postfix_operator(state))){
			return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_POSTFIX_EXPRESSION);
		}else{
			return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_POSTFIX_EXPRESSION);
		}
	}else{
		return (struct regex_parser_node *)0;
	}
}

static struct regex_parser_node * regular_expression(struct regex_compiler_state * state){
	struct regex_parser_node * n = (struct regex_parser_node *)0;
	if(state->errors){ return n; }
	if((n = regex_postfix_expression(state))){
		if((n->next = regular_expression(state))){
			return create_regex_parser_node(state, (struct regex_parser_node *)0, n, 0, REGEX_REGULAR_EXPRESSION);
		}else{
			set_error_state(state, REGEX_ERROR_INVALID_REGULAR_EXPRESSION);
			destroy_regex_parser_node_tree(state, n);
			return (struct regex_parser_node *)0;
		}
	}else{
		if(state->errors){ return n; }
		return create_regex_parser_node(state, (struct regex_parser_node *)0, regex_make_epsilon(state), 0, REGEX_REGULAR_EXPRESSION);
	}
}

static void regex_compiler_state_parse(struct regex_compiler_state * state){
	if((state->top_node = regular_expression(state))){
		/*  Should have consumed all characters. */
		if(state->position != state->buffer_length){
			set_error_state(state, REGEX_ERROR_INCOMPLETE_PARSING);
		}
	}else{
		set_error_state(state, REGEX_ERROR_PARSING_FAILED);
	}
}


static struct regex_parser_node * reg_first_child(struct regex_parser_node * p){
	return p->first_child;
}

static struct regex_parser_node * reg_second_child(struct regex_parser_node * p){
	return p->first_child->next;
}

static struct regex_parser_node * reg_third_child(struct regex_parser_node * p){
	return p->first_child->next->next;
}

static struct regex_parser_node * reg_fourth_child(struct regex_parser_node * p){
	return p->first_child->next->next->next;
}

static struct regex_parser_node * reg_is_first_child_type(struct regex_parser_node * p, enum regex_parser_node_type t){
	return (
		reg_first_child(p) &&
		reg_first_child(p)->type == t
	) ? reg_first_child(p) : (struct regex_parser_node *)0;
}

static struct regex_parser_node * reg_is_second_child_type(struct regex_parser_node * p, enum regex_parser_node_type t){
	return (
		reg_first_child(p) &&
		reg_second_child(p) &&
		reg_second_child(p)->type == t
	) ? reg_second_child(p) : (struct regex_parser_node *)0;
}

static struct regex_parser_node * reg_is_third_child_type(struct regex_parser_node * p, enum regex_parser_node_type t){
	return (
		reg_first_child(p) &&
		reg_second_child(p) &&
		reg_third_child(p) &&
		reg_third_child(p)->type == t
	) ? reg_third_child(p) : (struct regex_parser_node *)0;
}

static struct regex_parser_node * reg_is_fourth_child_type(struct regex_parser_node * p, enum regex_parser_node_type t){
	return (
		reg_first_child(p) &&
		reg_second_child(p) &&
		reg_third_child(p) &&
		reg_fourth_child(p) &&
		reg_fourth_child(p)->type == t
	) ? reg_fourth_child(p) : (struct regex_parser_node *)0;
}

static int reg_check_one_child(struct regex_parser_node * p, enum regex_parser_node_type t1){
	return (reg_is_first_child_type(p, t1) && 1);
}

static int reg_check_two_children(struct regex_parser_node * p, enum regex_parser_node_type t1, enum regex_parser_node_type t2){
	return (
		reg_is_first_child_type(p, t1) &&
		reg_is_second_child_type(p, t2)
	);
}

static int reg_check_three_children(struct regex_parser_node * p, enum regex_parser_node_type t1, enum regex_parser_node_type t2, enum regex_parser_node_type t3){
	return (
		reg_is_first_child_type(p, t1) &&
		reg_is_second_child_type(p, t2) &&
		reg_is_third_child_type(p, t3)
	);
}

static int reg_check_four_children(struct regex_parser_node * p, enum regex_parser_node_type t1, enum regex_parser_node_type t2, enum regex_parser_node_type t3, enum regex_parser_node_type t4){
	return (
		reg_is_first_child_type(p, t1) &&
		reg_is_second_child_type(p, t2) &&
		reg_is_third_child_type(p, t3) &&
		reg_is_fourth_child_type(p, t4)
	);
}

static unsigned char process_hex_digit(struct regex_parser_node * p){
	unsigned char c;
	assert(p->type == REGEX_HEX_DIGIT);
	c = p->first_child->c;
	if(c >= '0' && c <= '9'){
		return c - '0';
	}else if(c >= 'a' && c <= 'f'){
		return (c - 'a') + 10;
	}else if(c >= 'A' && c <= 'F'){
		return (c - 'A') + 10;
	}else{
		assert(0 && "Internal error: Should not happen.");
	}
}

static unsigned char process_non_class_character(struct regex_parser_node * p){
	assert(p->type = REGEX_NON_CLASS_CHARACTER);
	p = p->first_child;
	if(p->type == REGEX_ESCAPED_CHARACTER){
		unsigned char c = p->first_child->next->first_child->c;
		switch(c){
			case 'n':{
				return '\n';
			}case 't':{
				return '\t';
			}case '(':{
				return '(';
			}case ')':{
				return ')';
			}case '[':{
				return '[';
			}case ']':{
				return ']';
			}case '*':{
				return '*';
			}case '.':{
				return '.';
			}case '-':{
				return '-';
			}case '|':{
				return '|';
			}case '+':{
				return '+';
			}case '?':{
				return '?';
			}case '^':{
				return '^';
			}case '\\':{
				return '\\';
			}case 'x':{
				struct regex_parser_node * first_digit = p->first_child->next->first_child->next;
				return (process_hex_digit(first_digit) * 16) + process_hex_digit(first_digit->next);
			}default:{
				printf("Character was '%c'.\n", c);
				assert(0 && "Internal error: Unknown escaped character.");
			}
		}
	}else if(p->type == REGEX_TERMINAL){ /*  Any other character */
		return p->c;
	}else{
		assert(0 && "Should not happen.");
	}
}

static struct regex_computation_node * create_new_regex_computation_node(struct regex_compiler_state * state){
	struct regex_computation_node * n = struct_regex_computation_node_memory_pool_malloc(state->m->struct_regex_computation_node_pool);
	n->path1 = (struct regex_computation_node *)0;
	n->path2 = (struct regex_computation_node *)0;
	n->after = (struct regex_computation_node *)0;
	n->ranges = (struct character_class_range *)0;
	n->type = REGEX_COMP_UNUSED;
	n->c = 0;
	return n;
}

static struct regex_computation_node * get_last_node(struct regex_computation_node * p){
	assert(p);
	while(p->after){
		p = p->after;
	}
	return p;
}

static struct character_class_range * make_character_range(struct regex_parser_node * p){
	struct character_class_range * r = (struct character_class_range *)malloc(sizeof(struct character_class_range));
	r->next = (struct character_class_range *)0;
	assert(p->type == REGEX_CHARACTER_OR_RANGE);
	p = p->first_child;
	if(p->type == REGEX_CHARACTER_RANGE){
		r->low = process_non_class_character(p->first_child);
		r->high = process_non_class_character(p->first_child->next->next);
		r->is_dot = 0;
	}else if(p->type == REGEX_NON_CLASS_CHARACTER){
		r->low = process_non_class_character(p);
		r->high = process_non_class_character(p);
		r->is_dot = 0;
	}else if(p->type == REGEX_TERMINAL){/*  This is for a '.' control character. */
		r->is_dot = 1;
	}else{
		assert(0 && "If shouldn't happen.");
	}
	return r;
}

static struct regex_computation_node * make_character_class_node(struct regex_compiler_state * state, struct regex_parser_node * p1, struct regex_parser_node * p2, unsigned int negative){
	/*  This function is responsible for creating a linked list of all the possible options that can be checked against inside a character
	 ie. a [...]  pattern. */
	struct regex_computation_node * new_node = create_new_regex_computation_node(state);
	struct character_class_range * current;
	assert(p1->type == REGEX_CHARACTER_OR_RANGE);
	assert(p2->type == REGEX_CHARACTER_CLASS_REST);
	new_node->type = negative ? REGEX_COMP_NEG_CLASS : REGEX_COMP_CLASS;
	new_node->ranges = make_character_range(p1);
	current = new_node->ranges; /*  Remember where to link the rest of the ranges from */
	while(p2->type != REGEX_TERMINAL){
		p2 = p2->first_child;
		if(p2->type == REGEX_CHARACTER_OR_RANGE){
			current->next = make_character_range(p2);
			current = current->next;
			p2 = p2->next; /*  Move to character_class_rest */
		}else{
			assert(p2->type == REGEX_TERMINAL);
		}
	}
	return new_node;
}

static struct regex_computation_node * reg_character_class(struct regex_compiler_state * state, struct regex_parser_node * p){
	if(state->errors){ return (struct regex_computation_node *)0; }
	if(reg_check_three_children(p, REGEX_TERMINAL, REGEX_CHARACTER_OR_RANGE, REGEX_CHARACTER_CLASS_REST)){
		struct regex_parser_node * n = reg_second_child(p);
		struct regex_parser_node * o = reg_third_child(p);
		return make_character_class_node(state, n, o, 0);
	}else if(reg_check_four_children(p, REGEX_TERMINAL, REGEX_TERMINAL, REGEX_CHARACTER_OR_RANGE, REGEX_CHARACTER_CLASS_REST)){
		struct regex_parser_node * n = reg_third_child(p);
		struct regex_parser_node * o = reg_fourth_child(p);
		return make_character_class_node(state, n, o, 1);
	}else{
		assert(0 && "Should not happen.");
		return (struct regex_computation_node *)0;
	}
}

static struct regex_computation_node * reg_non_class_character(struct regex_compiler_state * state, struct regex_parser_node * p){
	struct regex_computation_node * n = create_new_regex_computation_node(state);
	if(state->errors){ return (struct regex_computation_node *)0; }
	n->c = process_non_class_character(p);
	n->type = REGEX_COMP_CHAR;
	return n;
}

static struct regex_computation_node * make_dot_character(struct regex_compiler_state * state){
	struct regex_computation_node * n = create_new_regex_computation_node(state);
	if(state->errors){ return (struct regex_computation_node *)0; }
	n->type = REGEX_COMP_DOT;
	return n;
}

static struct regex_computation_node * reg_character_or_class(struct regex_compiler_state * state, struct regex_parser_node * p){
	if(state->errors){ return (struct regex_computation_node *)0; }
	if(reg_check_one_child(p, REGEX_CHARACTER_CLASS)){
		return reg_character_class(state, reg_first_child(p));
	}else if(reg_check_one_child(p, REGEX_NON_CLASS_CHARACTER)){
		return reg_non_class_character(state, reg_first_child(p));
	}else if(reg_check_one_child(p, REGEX_TERMINAL)){
		return make_dot_character(state);
	}else{
		assert(0 && "Should not happen.");
		return (struct regex_computation_node *)0;
	}
}

static struct regex_computation_node * reg_character_sequence_rest(struct regex_compiler_state * state, struct regex_parser_node * p){
	if(state->errors){ return (struct regex_computation_node *)0; }
	if(reg_check_two_children(p, REGEX_CHARACTER_OR_CLASS, REGEX_CHARACTER_SEQUENCE_REST)){
		struct regex_computation_node * n = reg_character_or_class(state, reg_first_child(p));
		if(n){
			get_last_node(n)->after = reg_character_sequence_rest(state, reg_second_child(p));
		}
		return n;
	}else if(reg_check_one_child(p, REGEX_EPSILON)){
		return (struct regex_computation_node *)0;
	}else{
		assert(0 && "Should not happen.");
		return (struct regex_computation_node *)0;
	}
}

static struct regex_computation_node * reg_character_sequence(struct regex_compiler_state * state, struct regex_parser_node * p){
	if(state->errors){ return (struct regex_computation_node *)0; }
	if(reg_check_two_children(p, REGEX_CHARACTER_OR_CLASS, REGEX_CHARACTER_SEQUENCE_REST)){
		struct regex_computation_node * n = reg_character_or_class(state, reg_first_child(p));
		if(n){
			get_last_node(n)->after = reg_character_sequence_rest(state, reg_second_child(p));
			return n;
		}else{
			struct_regex_computation_node_memory_pool_free(state->m->struct_regex_computation_node_pool, n);
			return (struct regex_computation_node *)0;
		}
	}else{
		assert(0 && "Should not happen.");
		return (struct regex_computation_node *)0;
	}
}

static struct regex_computation_node * apply_postfix_operator(struct regex_compiler_state * state, struct regex_computation_node * n, struct regex_parser_node * p, struct regex_computation_node * p_after){
	if(reg_check_one_child(p, REGEX_TERMINAL)){
		struct regex_computation_node * last_n = n;
		struct regex_computation_node * follow_n = n;
		struct regex_computation_node * after_n;
		struct regex_computation_node * rtn;
		switch(reg_first_child(p)->c){
			case '?':{
				/*  Split into two threads, one that jumps over the pattern, and one that requires it. */
				struct regex_computation_node * optional = create_new_regex_computation_node(state);
				optional->type = REGEX_COMP_SPLIT;
				optional->after = n;
				optional->path1 = n;
				optional->path2 = p_after;
				after_n = p_after;
				rtn = optional;
				break;
			}case '*':{
				if(n->after){
					/*  Split into two threads, one that jumps over the pattern, and one that requires it. */
					struct regex_computation_node * any = create_new_regex_computation_node(state);
					struct regex_computation_node * again = create_new_regex_computation_node(state);
					again->type = REGEX_COMP_JUMP;
					again->after = p_after;
					again->path1 = any;

					any->type = REGEX_COMP_SPLIT;
					any->after = n; 
					any->path1 = n;  /*  See if we can match the pattern */
					any->path2 = p_after;  /*  Or just give up */
					after_n = again; /*  After the pattern, we can go back and match it again */
					rtn = any;
				}else{/*  Optimization to avoid unncessary splits */
					switch(n->type){
						case REGEX_COMP_CHAR:{
							n->type = REGEX_COMP_CHAR_STAR;
							break;
						}case REGEX_COMP_CLASS:{
							n->type = REGEX_COMP_CLASS_STAR;
							break;
						}case REGEX_COMP_NEG_CLASS:{
							n->type = REGEX_COMP_NEG_CLASS_STAR;
							break;
						}case REGEX_COMP_DOT:{
							n->type = REGEX_COMP_DOT_STAR;
							break;
						}default:{
							assert(0 && "Not considered.");
						}
					}
					n->after = p_after;
					n->path1 = n; /*  Just re-queue itself */
					return n;
				}
				break;
			}case '+':{
				/*  After pattern, split into two threads, one that jumps back over the pattern, and just continues. */
				struct regex_computation_node * again = create_new_regex_computation_node(state);
				again->type = REGEX_COMP_SPLIT;
				again->after = p_after;
				again->path1 = p_after;
				again->path2 = n;
				after_n = again; /*  After the pattern, we can go back and match it again */
				rtn = n;
				break;
			}default:{
				assert(0 && "Should not happen.");
			}
		}
		/*  If any special paths are null, assume they should be set to the next node */
		while(follow_n){
			switch(follow_n->type){
				case REGEX_COMP_SPLIT:{
					if(!follow_n->path1){
						follow_n->path1 = after_n;
					}
					if(!follow_n->path2){
						follow_n->path2 = after_n;
					}
					break;
				}case REGEX_COMP_JUMP:{
					if(!follow_n->path1){
						follow_n->path1 = after_n;
					}
					break;
				}default:{
				}
			}
			last_n = follow_n;
			follow_n = follow_n->after;
		}
		/*  Last node needs after set */
		last_n->after = after_n;
		return rtn;
	}else{
		assert(0 && "Should not happen.");
		return (struct regex_computation_node *)0;
	}
}

static struct regex_computation_node * reg_infix_expression_rest(struct regex_compiler_state * state, struct regex_parser_node * p, struct regex_computation_node * p_after){
	if(state->errors){ return (struct regex_computation_node *)0; }
	if(reg_check_three_children(p, REGEX_TERMINAL, REGEX_REGULAR_EXPRESSION, REGEX_INFIX_EXPRESSION_REST)){
		if(reg_first_child(p)->c == '|'){
			if(reg_first_child(reg_third_child(p))->c == ')'){/*  If this is the last option don't do a split */
				return reg_regular_expression(state, reg_second_child(p), p_after);
			}else{
				struct regex_computation_node * second_option = reg_infix_expression_rest(state, reg_third_child(p), p_after);
				struct regex_computation_node * jump = create_new_regex_computation_node(state);
				struct regex_computation_node * option = create_new_regex_computation_node(state);
				struct regex_computation_node * first_option;
				jump->type = REGEX_COMP_JUMP;
				jump->after = second_option;
				jump->path1 = p_after;

				first_option = reg_regular_expression(state, reg_second_child(p), jump);

				option->type = REGEX_COMP_SPLIT;
				option->after = first_option;
				option->path1 = first_option;
				option->path2 = second_option;
				return option;
			}
		}else{
			assert(0 && "Should not happen.");
			return (struct regex_computation_node *)0;
		}
	}else if(reg_check_one_child(p, REGEX_TERMINAL)){
		if(reg_first_child(p)->c == ')'){
			return p_after;
		}else{
			assert(0 && "Should not happen.");
			return (struct regex_computation_node *)0;
		}
	}else{
		assert(0 && "Should not happen.");
		return (struct regex_computation_node *)0;
	}
}

static struct regex_computation_node * reg_infix_expression(struct regex_compiler_state * state, struct regex_parser_node * p, struct regex_computation_node * p_after){
	if(state->errors){ return (struct regex_computation_node *)0; }
	if(reg_check_three_children(p, REGEX_TERMINAL, REGEX_REGULAR_EXPRESSION, REGEX_INFIX_EXPRESSION_REST)){
		if(reg_first_child(p)->c == '('){
			if(reg_first_child(reg_third_child(p))->c == ')'){/*  If this is the last option don't do a split */
				return reg_regular_expression(state, reg_second_child(p), p_after);
			}else{
				struct regex_computation_node * second_option = reg_infix_expression_rest(state, reg_third_child(p), p_after);
				struct regex_computation_node * jump = create_new_regex_computation_node(state);
				struct regex_computation_node * option = create_new_regex_computation_node(state);
				struct regex_computation_node * first_option;
				jump->type = REGEX_COMP_JUMP;
				jump->after = second_option;
				jump->path1 = p_after;

				first_option = reg_regular_expression(state, reg_second_child(p), jump);

				option->type = REGEX_COMP_SPLIT;
				option->after = first_option;
				option->path1 = first_option;
				option->path2 = second_option;
				return option;
			}
		}else{
			assert(0 && "Should not happen.");
			return (struct regex_computation_node *)0;
		}
	}else{
		assert(0 && "Should not happen.");
		return (struct regex_computation_node *)0;
	}
}

static struct regex_computation_node * remove_last_character(struct regex_computation_node * n){
	struct regex_computation_node * rtn = (struct regex_computation_node *)0;
	while(n->after){
		if(!n->after->after){
			rtn = n->after; /*  This was the last node */
			n->after = (struct regex_computation_node *)0;
			break;
		}
		n = n->after;
	}
	assert(rtn);
	return rtn;
}

static struct regex_computation_node * reg_postfix_expression(struct regex_compiler_state * state, struct regex_parser_node * p, struct regex_computation_node * p_after){
	if(state->errors){ return (struct regex_computation_node *)0; }

	if(reg_check_two_children(p, REGEX_CHARACTER_SEQUENCE, REGEX_POSTFIX_OPERATOR)){
		/*  This case is special because we have to take the last character our of the sequence and apply the postfix operator to it */
		struct regex_computation_node * n = reg_character_sequence(state, reg_first_child(p));
		if(state->errors){ return (struct regex_computation_node *)0; }
		if(!n){
			return p_after;
		}else if(n->after){
			/*  Take the last character off the character sequence, and apply the postfix operator to it. */
			struct regex_computation_node * postfixed = apply_postfix_operator(state, remove_last_character(n), reg_second_child(p), p_after);
			/*  Then take the second last character in the sequence, and set its after node to the postfix application */
			get_last_node(n)->after = postfixed;
			return n;
		}else{
			/*  If it is the only character, there is nothing in front */
			return apply_postfix_operator(state, n, reg_second_child(p), p_after);
		}
	}else if(reg_check_two_children(p, REGEX_INFIX_EXPRESSION, REGEX_POSTFIX_OPERATOR)){
		/*  The null 'after' node on the infix expression should get overwritten inside apply postfix operator*/
		return apply_postfix_operator(state, reg_infix_expression(state, reg_first_child(p), (struct regex_computation_node *)0), reg_second_child(p), p_after);
	}else if(reg_check_one_child(p, REGEX_CHARACTER_SEQUENCE)){
		struct regex_computation_node * n = reg_character_sequence(state, reg_first_child(p));
		get_last_node(n)->after = p_after;
		return n;
	}else if(reg_check_one_child(p, REGEX_INFIX_EXPRESSION)){
		return reg_infix_expression(state, reg_first_child(p), p_after);
	}else{
		assert(0 && "Should not happen.");
		return (struct regex_computation_node *)0;
	}
}

static struct regex_computation_node * reg_regular_expression(struct regex_compiler_state * state, struct regex_parser_node * p, struct regex_computation_node * p_after){
	if(state->errors){ return (struct regex_computation_node *)0; }

	if(reg_check_two_children(p, REGEX_POSTFIX_EXPRESSION, REGEX_REGULAR_EXPRESSION)){
		return reg_postfix_expression(state, reg_first_child(p), reg_regular_expression(state, reg_second_child(p), p_after));
	}else if(reg_check_one_child(p, REGEX_EPSILON)){
		return p_after;
	}else{
		assert(0 && "Should not happen.");
		return (struct regex_computation_node *)0;
	}
}

void regex_compiler_state_compile(struct regex_compiler_state * state){
	struct regex_computation_node * match = create_new_regex_computation_node(state);
	match->type = REGEX_COMP_MATCH;
	match->after = (struct regex_computation_node *)0;
	match->match_id = state->match_id;
	/*  Do parsing */
	regex_compiler_state_parse(state);
	/*  Do code generation */
	state->first_computation_node = reg_regular_expression(state, state->top_node, match);
	if(state->errors){
		destroy_regex_computation_nodes(state, match);
	}
}

static void explain_character_range(struct character_class_range * n){
	while(n){
		if(n->is_dot){
			printf("any non-newline");
		}else{
			if(n->low == n->high){
				printf("'%c'", n->low);
			}else{
				printf("'%c' to '%c'", n->low, n->high);
			}
		}
		n = n->next;
		if(n){
			printf(", ");
		}
	}
}

static void explain_error(struct regex_compiler_state * state){
	struct regex_compiler_error * error = state->errors;
	unsigned int i;
	printf("Begin displaying error stack\n");
	while(error){
		switch(error->type){
			case REGEX_ERROR_INVALID_CHARACTER_SEQUENCE:{
				printf("This character sequence is invalid.\n");
				break;
			}case REGEX_ERROR_NOT_ESCAPABLE:{
				printf("This back slash is trying to escape something that is not escapable.\n");
				break;
			}case REGEX_ERROR_EXPECTED_HEX_DIGIT:{
				printf("This is not a hexidecimal digit.  Expected 0-9, a-f, or A-F.\n");
				break;
			}case REGEX_ERROR_EXPECTED_INFIX_EXPRESSION_REST:{
				printf("Missing rest of infix expression.  Expected ')' or '|'.\n");
				break;
			}case REGEX_ERROR_INCOMPLETE_PARSING:{
				printf("The parser was not able to fully parse this regex because it does not match the regex grammar.\n");
				break;
			}case REGEX_ERROR_PARSING_FAILED:{
				printf("Parsing failed.\n");
				break;
			}case REGEX_ERROR_INVALID_REGULAR_EXPRESSION:{
				printf("Invalid regular expression.\n");
				break;
			}case REGEX_ERROR_INVALID_CHARACTER_RANGE:{
				printf("Invalid character range.\n");
				break;
			}case REGEX_ERROR_INVALID_CHARACTER_CLASS:{
				printf("Invalid character class.\n");
				break;
			}default:{
				assert(0);
			}
		}
		printf("Pattern: %s\n", state->buffer);
		printf("         ");
		for(i = 0; i < state->buffer_length; i++){
			if(i == error->character_index_of_error){
				printf("^");
			}else{
				printf(" ");
			}
		}
		printf("\n");
		error = error->next;
	}
	printf("Finished displaying error stack\n");
}

void regex_compiler_state_explain(struct regex_compiler_state * state){
	struct regex_computation_node * current_node = state->first_computation_node;
	if(state->errors){
		explain_error(state);
		return;
	}
	printf("Explaining regex...\n");
	while(current_node){
		switch(current_node->type){
			case REGEX_COMP_CHAR:{
				printf("%p: REGEX_COMP_CHAR: %c after is %p\n", (void*)current_node, current_node->c, (void*)current_node->after);
				break;
			}case REGEX_COMP_CHAR_STAR:{
				printf("%p: REGEX_COMP_CHAR_STAR: %c after is %p\n", (void*)current_node, current_node->c, (void*)current_node->after);
				break;
			}case REGEX_COMP_DOT:{
				printf("%p: REGEX_COMP_DOT after is %p\n", (void*)current_node, (void*)current_node->after);
				break;
			}case REGEX_COMP_DOT_STAR:{
				printf("%p: REGEX_COMP_DOT_STAR after is %p\n", (void*)current_node, (void*)current_node->after);
				break;
			}case REGEX_COMP_SPLIT:{
				printf("%p: REGEX_COMP_SPLIT %p %p after is %p\n", (void*)current_node, (void*)current_node->path1, (void*)current_node->path2, (void*)current_node->after);
				break;
			}case REGEX_COMP_JUMP:{
				printf("%p: REGEX_COMP_JUMP to %p after is %p\n", (void*)current_node, (void*)current_node->path1, (void*)current_node->after);
				break;
			}case REGEX_COMP_MATCH:{
				printf("%p: REGEX_COMP_MATCH after is %p\n", (void*)current_node, (void*)current_node->after);
				break;
			}case REGEX_COMP_NEG_CLASS:{
				printf("%p: REGEX_COMP_NEG_CLASS: Not one of [", (void*)current_node);
				explain_character_range(current_node->ranges);
				printf("] after is %p\n", (void*)current_node->after);
				break;
			}case REGEX_COMP_NEG_CLASS_STAR:{
				printf("%p: REGEX_COMP_NEG_CLASS_STAR: Not one of [", (void*)current_node);
				explain_character_range(current_node->ranges);
				printf("] after is %p\n", (void*)current_node->after);
				break;
			}case REGEX_COMP_CLASS:{
				printf("%p: REGEX_COMP_CLASS: One of [", (void*)current_node);
				explain_character_range(current_node->ranges);
				printf("] after is %p\n", (void*)current_node->after);
				break;
			}case REGEX_COMP_CLASS_STAR:{
				printf("%p: REGEX_COMP_CLASS_STAR: One of [", (void*)current_node);
				explain_character_range(current_node->ranges);
				printf("] after is %p\n", (void*)current_node->after);
				break;
			}default:{
				printf("Unknown node type\n");
				assert(0);
			}
		}
		current_node = current_node->after;
	}
}

static void destroy_errors(struct regex_compiler_error * n){
	if(n){
		struct regex_compiler_error * a = n->next;
		free((void*)n);
		destroy_errors(a);
	}
}

static void destroy_character_class_ranges(struct character_class_range * n){
	if(n){
		struct character_class_range * a = n->next;
		free((void*)n);
		destroy_character_class_ranges(a);
	}
}

static void destroy_regex_computation_nodes(struct regex_compiler_state * state, struct regex_computation_node * n){
	(void)state;
	if(n){
		struct regex_computation_node * a = n->after;
		destroy_character_class_ranges(n->ranges);
		struct_regex_computation_node_memory_pool_free(state->m->struct_regex_computation_node_pool, n);
		destroy_regex_computation_nodes(state, a);
	}
}

void regex_matcher_state_create(struct regex_matcher_state * matcher, struct regex_compiler_state ** compiler_states, unsigned int num_compiler_states){
	unsigned int i;
	struct regex_computation_node * j;
	unsigned int max_num_computation_nodes = 0;
	matcher->num_compiled_regexes = 0;
	matcher->num_unique_matches_found = 0;
	matcher->possible_matches = (struct regex_match*)malloc(sizeof(struct regex_match) * num_compiler_states); /*  In the worst case everything matches */
	matcher->actual_matches = (struct regex_match**)malloc(sizeof(struct regex_match*) * num_compiler_states); /*  In the worst case everything matches */
	matcher->compiler_states = compiler_states;
	matcher->num_compiled_regexes = num_compiler_states;

	/*  The worst-case number of states to keep track of is the number of computation nodes */
	for(i = 0; i < num_compiler_states; i++){
		assert(compiler_states[i]->match_id < num_compiler_states);
		assert(!compiler_states[i]->errors);
		assert(compiler_states[i]->first_computation_node);
		j = compiler_states[i]->first_computation_node;
		while(j){
			j->node_id = max_num_computation_nodes; /*  Set an index (unique to this matcher) that gives an index into the occupied list */
			max_num_computation_nodes++;
			j = j->after;
		}
	}
	matcher->next_list = (struct regex_computation_node **)malloc(sizeof(struct regex_computation_node *) * max_num_computation_nodes);
	matcher->current_list = (struct regex_computation_node **)malloc(sizeof(struct regex_computation_node *) * max_num_computation_nodes);
	matcher->next_occupied_list = (unsigned int *)malloc(sizeof(unsigned int) * max_num_computation_nodes);
	matcher->current_occupied_list = (unsigned int *)malloc(sizeof(unsigned int) * max_num_computation_nodes);
	/*  Initially, they are not in the list */
	for(i = 0; i < max_num_computation_nodes; i++){
		matcher->current_occupied_list[i] = 0;
		matcher->next_occupied_list[i] = 0;
	}

	matcher->num_optimized_possible_matches = (unsigned int*)malloc(sizeof(unsigned int) * 256);
	matcher->optimized_match_ids = (unsigned int**)malloc(sizeof(unsigned int*) * 256);

	matcher->num_unique_matches_found = 0;
	matcher->next_current_list_index = 0;
	matcher->next_next_list_index = 0;
	for(i = 0; i < matcher->num_compiled_regexes; i++){
		matcher->possible_matches[i].first_match = 1;  /*  Reset whether each match has been matched yet */
	}

	for(i = 0; i < 256; i++){
		regex_matcher_state_optimize(matcher, (unsigned char)i);
	}
}

void regex_matcher_state_destroy(struct regex_matcher_state * matcher){
	unsigned int i;
	free((void*)matcher->actual_matches);
	free((void*)matcher->possible_matches);
	free((void*)matcher->next_list);
	free((void*)matcher->current_list);
	free((void*)matcher->current_occupied_list);
	free((void*)matcher->next_occupied_list);
	free((void*)matcher->num_optimized_possible_matches);
	for(i = 0; i < 256; i++){
		free((void*)matcher->optimized_match_ids[i]);
	}
	free((void*)matcher->optimized_match_ids);
}

void regex_matcher_state_add(struct regex_matcher_state *, struct regex_computation_node *);

void regex_matcher_state_add(struct regex_matcher_state * matcher, struct regex_computation_node * n){
	if(!matcher->current_occupied_list[n->node_id]){
		matcher->current_occupied_list[n->node_id] = 1; /*  It is now in the list */
		matcher->current_list[matcher->next_current_list_index++] = n;
	}
}

void regex_matcher_state_add_next(struct regex_matcher_state *, struct regex_computation_node *);

void regex_matcher_state_add_next(struct regex_matcher_state * matcher, struct regex_computation_node * n){
	if(!matcher->next_occupied_list[n->node_id]){
		matcher->next_occupied_list[n->node_id] = 1; /*  It is now in the list */
		matcher->next_list[matcher->next_next_list_index++] = n;
	}
}

static void regex_matcher_state_optimize(struct regex_matcher_state * matcher, unsigned char c){
	unsigned int more_chars = 1;
	unsigned int i = 0;
	struct regex_computation_node * current;

	/*  Add every possible state, and see what matches remain after this first character  */
	matcher->num_unique_matches_found = 0;
	matcher->next_current_list_index = 0;
	matcher->next_next_list_index = 0;
	for(i = 0; i < matcher->num_compiled_regexes; i++){
		matcher->current_list[matcher->next_current_list_index] = matcher->compiler_states[i]->first_computation_node;
		matcher->next_current_list_index++;

		matcher->possible_matches[i].first_match = 1;  /*  Reset whether each match has been matched yet */
	}

	while(matcher->next_current_list_index){ /* While there are entires in the list for the current character */
		current = matcher->current_list[--matcher->next_current_list_index];
		matcher->current_occupied_list[current->node_id] = 0; /*  No longer in the list */
		switch(current->type){
			case REGEX_COMP_CHAR:{
				if(more_chars){
					if(c == current->c){
						regex_matcher_state_add_next(matcher, current->after);
					}
				}
				break;
			}case REGEX_COMP_CHAR_STAR:{
				if(more_chars){
					if(c == current->c){
						regex_matcher_state_add_next(matcher, current->path1);
					}else{
						regex_matcher_state_add(matcher, current->after);
					}
				}else{
					regex_matcher_state_add(matcher, current->after);
				}
				break;
			}case REGEX_COMP_DOT:{
				if(more_chars){
					if(c != '\n'){
						regex_matcher_state_add_next(matcher, current->after);
					}
				}
				break;
			}case REGEX_COMP_DOT_STAR:{
				if(more_chars){
					if(c != '\n'){
						regex_matcher_state_add_next(matcher, current->path1);
					}else{
						regex_matcher_state_add(matcher, current->after);
					}
				}else{
					regex_matcher_state_add(matcher, current->after);
				}
				break;
			}case REGEX_COMP_SPLIT:{
				regex_matcher_state_add(matcher, current->path1);
				regex_matcher_state_add(matcher, current->path2);
				break;
			}case REGEX_COMP_JUMP:{
				regex_matcher_state_add(matcher, current->path1);
				break;
			}case REGEX_COMP_MATCH:{
				matcher->possible_matches[current->match_id].match_node = current;
				if(matcher->possible_matches[current->match_id].first_match){ /*  Is this the first time this regex has matched? */
					matcher->actual_matches[matcher->num_unique_matches_found] = &matcher->possible_matches[current->match_id];
					matcher->num_unique_matches_found++;
					matcher->possible_matches[current->match_id].first_match = 0;
				}
				break;
			}case REGEX_COMP_NEG_CLASS:{
				if(more_chars){
					struct character_class_range * r = current->ranges;
					unsigned int matches = 0; /* Assume it does not match */
					while(r){
						if(r->is_dot){
							if(c != '\n'){
								matches = 1;
								break;
							}
						}else if(c >= r->low && c <= r->high){
							matches = 1;
							break;
						}
						r = r->next;
					}
					if(!matches){
						regex_matcher_state_add_next(matcher, current->after);
					}
				}
				break;
			}case REGEX_COMP_NEG_CLASS_STAR:{
				if(more_chars){
					struct character_class_range * r = current->ranges;
					unsigned int matches = 0; /* Assume it does not match */
					while(r){
						if(r->is_dot){
							if(c != '\n'){
								matches = 1;
								break;
							}
						}else if(c >= r->low && c <= r->high){
							matches = 1;
							break;
						}
						r = r->next;
					}
					if(matches){
						regex_matcher_state_add(matcher, current->after);
					}else{
						regex_matcher_state_add_next(matcher, current->path1);
					}
				}else{
					regex_matcher_state_add(matcher, current->after);
				}
				break;
			}case REGEX_COMP_CLASS:{
				if(more_chars){
					struct character_class_range * r = current->ranges;
					while(r){
						if(r->is_dot){
							if(c != '\n'){
								regex_matcher_state_add_next(matcher, current->after);
								break;
							}
						}else if(c >= r->low && c <= r->high){
							regex_matcher_state_add_next(matcher, current->after);
							break;
						}
						r = r->next;
					}
				}
				break;
			}case REGEX_COMP_CLASS_STAR:{
				if(more_chars){
					struct character_class_range * r = current->ranges;
					unsigned int matches = 0; /* Assume it does not match */
					while(r){
						if(r->is_dot){
							if(c != '\n'){
								matches = 1;
								break;
							}
						}else if(c >= r->low && c <= r->high){
							matches = 1;
							break;
						}
						r = r->next;
					}
					if(matches){
						regex_matcher_state_add_next(matcher, current->path1);
					}else{
						regex_matcher_state_add(matcher, current->after);
					}
				}else{
					regex_matcher_state_add(matcher, current->after);
				}
				break;
			}default:{
				printf("Unknown node type\n");
				assert(0);
			}
		}
	}
	/*  Take everything out of the next list and see what could have matched */
	while(matcher->next_next_list_index){
		matcher->next_next_list_index--;
		matcher->next_occupied_list[matcher->next_list[matcher->next_next_list_index]->node_id] = 0; /*  No longer in the list */
		current = matcher->next_list[matcher->next_next_list_index];
		current = get_last_node(current); /*  Go to end match node. */
		switch(current->type){
			case REGEX_COMP_MATCH:{
				/*  Record the match */
				matcher->possible_matches[current->match_id].match_node = current;
				if(matcher->possible_matches[current->match_id].first_match){ /*  Is this the first time this regex has matched? */
					matcher->actual_matches[matcher->num_unique_matches_found] = &matcher->possible_matches[current->match_id];
					matcher->num_unique_matches_found++;
					matcher->possible_matches[current->match_id].first_match = 0;
				}
				break;
			}default:{
				printf("Should not happen.\n");
				assert(0);
			}
		}
	}
	matcher->num_optimized_possible_matches[c] = matcher->num_unique_matches_found;

	if(matcher->num_unique_matches_found){
		/*  Make a new array to store all our match ids */
		matcher->optimized_match_ids[c] = (unsigned int *)malloc(sizeof(unsigned int) * matcher->num_unique_matches_found);
		for(i = 0; i < matcher->num_unique_matches_found; i++){
			/*  Add to the list of possible match ids for this character*/
			matcher->optimized_match_ids[c][i] = matcher->actual_matches[i]->match_node->match_id;
		}
	}else{
		matcher->optimized_match_ids[c] = (unsigned int *)0;
	}
	/*  Reset for our first match */
	matcher->num_unique_matches_found = 0;
	matcher->next_current_list_index = 0;
	matcher->next_next_list_index = 0;
}

void regex_matcher_state_run(struct regex_matcher_state * matcher, unsigned char * str, unsigned int max_length){
	unsigned int more_chars = 1;
	unsigned int j;
	unsigned int i = 0;
	unsigned int temp_next;
	struct regex_computation_node * current;
	struct regex_computation_node ** temp;
	unsigned int * temp_i;
	unsigned char first_char = max_length ? str[0] : (unsigned char)0; /*  There is an edge case where we're matching an emptry pattern against an empty string. */

        /*  Reset our state so that we can run the same compiled regex again against a different string */
        matcher->num_unique_matches_found = 0;
        matcher->next_current_list_index = 0;
        matcher->next_next_list_index = 0;

	for(j = 0; j < matcher->num_optimized_possible_matches[first_char]; j++){
		matcher->current_list[matcher->next_current_list_index] = matcher->compiler_states[matcher->optimized_match_ids[first_char][j]]->first_computation_node;
		matcher->next_current_list_index++;
		matcher->possible_matches[matcher->optimized_match_ids[first_char][j]].first_match = 1;  /*  Only need to reset whether each match has been matched yet for the possible matches */
	}

	while(matcher->next_current_list_index){ /* While there are still characters to look at (after list swap) */
		while(matcher->next_current_list_index){ /* While there are entires in the list for the current character */
			current = matcher->current_list[--matcher->next_current_list_index];
			matcher->current_occupied_list[current->node_id] = 0; /*  No longer in the list */
			switch(current->type){
				case REGEX_COMP_CHAR:{
					if(more_chars){
						if(str[i] == current->c){
							regex_matcher_state_add_next(matcher, current->after);
						}
					}
					break;
				}case REGEX_COMP_CHAR_STAR:{
					if(more_chars){
						if(str[i] == current->c){
							regex_matcher_state_add_next(matcher, current->path1);
						}else{
							regex_matcher_state_add(matcher, current->after);
						}
					}else{
						regex_matcher_state_add(matcher, current->after);
					}
					break;
				}case REGEX_COMP_DOT:{
					if(more_chars){
						if(str[i] != '\n'){
							regex_matcher_state_add_next(matcher, current->after);
						}
					}
					break;
				}case REGEX_COMP_DOT_STAR:{
					if(more_chars){
						if(str[i] != '\n'){
							regex_matcher_state_add_next(matcher, current->path1);
						}else{
							regex_matcher_state_add(matcher, current->after);
						}
					}else{
						regex_matcher_state_add(matcher, current->after);
					}
					break;
				}case REGEX_COMP_SPLIT:{
					regex_matcher_state_add(matcher, current->path1);
					regex_matcher_state_add(matcher, current->path2);
					break;
				}case REGEX_COMP_JUMP:{
					regex_matcher_state_add(matcher, current->path1);
					break;
				}case REGEX_COMP_MATCH:{
					matcher->possible_matches[current->match_id].match_node = current;
					matcher->possible_matches[current->match_id].length = i;
					if(matcher->possible_matches[current->match_id].first_match){ /*  Is this the first time this regex has matched? */
						matcher->actual_matches[matcher->num_unique_matches_found] = &matcher->possible_matches[current->match_id];
						matcher->num_unique_matches_found++;
						matcher->possible_matches[current->match_id].first_match = 0;
					}
					break;
				}case REGEX_COMP_NEG_CLASS:{
					if(more_chars){
						struct character_class_range * r = current->ranges;
						unsigned int matches = 0; /* Assume it does not match */
						while(r){
							if(r->is_dot){
								if(str[i] != '\n'){
									matches = 1;
									break;
								}
							}else if(str[i] >= r->low && str[i] <= r->high){
								matches = 1;
								break;
							}
							r = r->next;
						}
						if(!matches){
							regex_matcher_state_add_next(matcher, current->after);
						}
					}
					break;
				}case REGEX_COMP_NEG_CLASS_STAR:{
					if(more_chars){
						struct character_class_range * r = current->ranges;
						unsigned int matches = 0; /* Assume it does not match */
						while(r){
							if(r->is_dot){
								if(str[i] != '\n'){
									matches = 1;
									break;
								}
							}else if(str[i] >= r->low && str[i] <= r->high){
								matches = 1;
								break;
							}
							r = r->next;
						}
						if(matches){
							regex_matcher_state_add(matcher, current->after);
						}else{
							regex_matcher_state_add_next(matcher, current->path1);
						}
					}else{
						regex_matcher_state_add(matcher, current->after);
					}
					break;
				}case REGEX_COMP_CLASS:{
					if(more_chars){
						struct character_class_range * r = current->ranges;
						while(r){
							if(r->is_dot){
								if(str[i] != '\n'){
									regex_matcher_state_add_next(matcher, current->after);
									break;
								}
							}else if(str[i] >= r->low && str[i] <= r->high){
								regex_matcher_state_add_next(matcher, current->after);
								break;
							}
							r = r->next;
						}
					}
					break;
				}case REGEX_COMP_CLASS_STAR:{
					if(more_chars){
						struct character_class_range * r = current->ranges;
						unsigned int matches = 0; /* Assume it does not match */
						while(r){
							if(r->is_dot){
								if(str[i] != '\n'){
									matches = 1;
									break;
								}
							}else if(str[i] >= r->low && str[i] <= r->high){
								matches = 1;
								break;
							}
							r = r->next;
						}
						if(matches){
							regex_matcher_state_add_next(matcher, current->path1);
						}else{
							regex_matcher_state_add(matcher, current->after);
						}
					}else{
						regex_matcher_state_add(matcher, current->after);
					}
					break;
				}default:{
					printf("Unknown node type\n");
					assert(0);
				}
			}
		}
		/*  Swap the occupied lists*/
		temp_i = matcher->current_occupied_list;
		matcher->current_occupied_list = matcher->next_occupied_list;
		matcher->next_occupied_list = temp_i;

		/*  Swap the next and current node lists*/
		temp = matcher->current_list;
		matcher->current_list = matcher->next_list;
		matcher->next_list = temp;

		/*  Swap the indicies of the next nodes in lists */
		temp_next = matcher->next_current_list_index;
		matcher->next_current_list_index = matcher->next_next_list_index;
		matcher->next_next_list_index = temp_next;
		more_chars = (++i) < max_length;
	}
	assert(!matcher->next_next_list_index);  /*  Make sure there are no nodes sitting in the next step (otherwise we would need to remove them). */
}
