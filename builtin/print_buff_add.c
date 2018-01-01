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

#include "print_buff_add.h"

void print_buff_add(struct printing_state * ps, char c){
	if(ps->chars_required < ps->buffer_size){
		ps->buffer[ps->chars_required] = c;
	}
	ps->chars_required = ps->chars_required + 1;
}


void print_buff_left_zero_pad(struct printing_state * ps, unsigned int start, unsigned int pads){
        unsigned int chars_actually_printed = (ps->chars_required - start);
        if(chars_actually_printed < pads){
                unsigned int pads_needed = pads - chars_actually_printed;
                if((ps->chars_required + pads_needed) <= ps->buffer_size){
                        /*  Copy from the end of of the buffer back */
                        unsigned int i;
                        for(i = 0; i < chars_actually_printed; i++){
                                ps->buffer[ps->chars_required -1 -i + pads_needed] = ps->buffer[ps->chars_required -1 -i];
                        }
                        /*  Data is moved now, put in the zeros: */
                        for(i = 0; i < pads_needed; i++){
                                ps->buffer[start + i] = '0';
                        }
                }
                ps->chars_required = ps->chars_required + pads_needed;
        }
}
