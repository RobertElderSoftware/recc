#ifndef OP_CPU_H_
#define OP_CPU_H_
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
#include <stdlib.h>
#include <assert.h>
#include "enum_l0_directive_type.h"
#include "enum_l0_instruction_type.h"

#define OP_CPU_PAGE_SIZE_NUM_BITS 10
#define LEVEL_1_PAGE_TABLE_NUM_BITS 11
#define LEVEL_2_PAGE_TABLE_NUM_BITS 11

#define OP_CPU_PAGE_SIZE (1 << OP_CPU_PAGE_SIZE_NUM_BITS)

#define PAGE_OFFSET_MASK (0xFFFFFFFF >> (32u - OP_CPU_PAGE_SIZE_NUM_BITS))
#define LEVEL_1_PAGE_TABLE_INDEX_MASK 0x001FFC00
#define LEVEL_2_PAGE_TABLE_INDEX_MASK 0xFFE00000

#define LEVEL_1_PAGE_TABLE_ENTRY_EXECUTE_BIT (1 << 0)
#define LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT   (1 << 1)
#define LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT    (1 << 2)
#define LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED (0x1 << 9)
#define LEVEL_1_PAGE_TABLE_ENTRY_INITIALIZED (0x1 << 9)

#define BITS_PER_BRANCH_DIST  9u
#define BITS_PER_LITERAL     16u
#define BITS_PER_OP_CODE      5u
#define OP_CODE_OFFSET       27u
#define BITS_PER_REGISTER     9u
#define ra_OFFSET            18u
#define rb_OFFSET            (ra_OFFSET - BITS_PER_REGISTER)
#define rc_OFFSET            (rb_OFFSET - BITS_PER_REGISTER)

#define UNSHIFTED_OP_CODE_MASK   (0xFFFFFFFF >> (32u - BITS_PER_OP_CODE))
#define OP_CODE_MASK             (UNSHIFTED_OP_CODE_MASK << OP_CODE_OFFSET)
#define UNSHIFTED_REGISTER_MASK  (0xFFFFFFFF >> (32u - BITS_PER_REGISTER))
#define ra_MASK                  (UNSHIFTED_REGISTER_MASK << ra_OFFSET)
#define rb_MASK                  (UNSHIFTED_REGISTER_MASK << rb_OFFSET)
#define rc_MASK                  (UNSHIFTED_REGISTER_MASK << rc_OFFSET)
#define LITERAL_MASK             (0xFFFFFFFF >> (32u - BITS_PER_LITERAL))
#define BRANCH_DISTANCE_MASK     (0xFFFFFFFF >> (32u - BITS_PER_BRANCH_DIST))

#define BRANCH_DISTANCE_SIGN_BIT 0x100

#define MAX_LL_CONSTANT 0xFFFF
#define MAX_BRANCH_POS 255
#define MAX_BRANCH_NEG 256

#define UART1_OUT        0x00300000u
#define UART1_IN         0x00300010u
#define IRQ_HANDLER      0x00300020u
#define TIMER_PERIOD     0x00300030u
#define PAGE_POINTER     0x00300040u
#define PFE_PAGE_POINTER 0x00300044u 
#define PFE_PC_VALUE     0x00300048u 
#define PFE_ACCESS       0x0030004Cu 
#define PFE_VIRTUAL      0x00300050u 

#define PC_index 0u
#define SP_index 1u
#define FR_index 4u
#define WR_index 5u

#define HALTED_BIT                          (1u << 0u)
#define GLOBAL_INTERRUPT_ENABLE_BIT         (1u << 1u)
#define RTE_BIT                             (1u << 2u)
#define TIMER1_ENABLE_BIT                   (1u << 3u)
#define TIMER1_ASSERTED_BIT                 (1u << 4u)
#define UART1_OUT_ENABLE_BIT                (1u << 5u)
#define UART1_OUT_ASSERTED_BIT              (1u << 6u)
#define UART1_IN_ENABLE_BIT                 (1u << 7u)
#define UART1_IN_ASSERTED_BIT               (1u << 8u)
#define UART1_OUT_READY_BIT                 (1u << 9u)
#define UART1_IN_READY_BIT                  (1u << 10u)
#define DIV_ZERO_ASSERTED_BIT               (1u << 11u)
#define PAGE_FAULT_EXCEPTION_ASSERTED_BIT   (1u << 12u)
#define PAGEING_ENABLE_BIT                  (1u << 13u)

#define NUM_INSTRUCTION_TYPES 14u

#define ADD_OP_CODE (0u  << OP_CODE_OFFSET)
#define SUB_OP_CODE (1u  << OP_CODE_OFFSET)
#define MUL_OP_CODE (2u  << OP_CODE_OFFSET)
#define DIV_OP_CODE (3u  << OP_CODE_OFFSET)
#define AND_OP_CODE (4u  << OP_CODE_OFFSET)
#define OR_OP_CODE  (5u  << OP_CODE_OFFSET)
#define NOT_OP_CODE (6u  << OP_CODE_OFFSET)
#define	LOA_OP_CODE (7u  << OP_CODE_OFFSET)
#define	STO_OP_CODE (8u  << OP_CODE_OFFSET)
#define	SHR_OP_CODE (9u  << OP_CODE_OFFSET)
#define	SHL_OP_CODE (10u << OP_CODE_OFFSET)
#define	BEQ_OP_CODE (11u << OP_CODE_OFFSET)
#define	BLT_OP_CODE (12u << OP_CODE_OFFSET)
#define	LL_OP_CODE  (13u << OP_CODE_OFFSET)

struct virtual_machine {
	unsigned int * memoryuint32;
	unsigned int * registeruint32;
	unsigned int cycles_executed;
	unsigned int num_memory_words;
	unsigned int num_registers;
	unsigned int pad;
};

unsigned int is_halted(struct virtual_machine *);
unsigned int vm_putc(struct virtual_machine *, unsigned int);
unsigned int vm_getc(struct virtual_machine *, unsigned int *);
void step(struct virtual_machine *);
struct virtual_machine * vm_create(unsigned char [1][5]);
void vm_destroy(struct virtual_machine *);

#endif
