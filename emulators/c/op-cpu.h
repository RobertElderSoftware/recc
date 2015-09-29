#ifndef __OP_CPU_H__
#define __OP_CPU_H__
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
#include <stdlib.h>
#include <assert.h>

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

#define PC_index 0u
#define SP_index 1u
#define FR_index 4u
#define WR_index 5u

#define HALTED_BIT                  (1u << 0u)
#define GLOBAL_INTERRUPT_ENABLE_BIT (1u << 1u)
#define RTE_BIT                     (1u << 2u)
#define TIMER1_ENABLE_BIT           (1u << 3u)
#define TIMER1_ASSERTED_BIT         (1u << 4u)
#define UART1_OUT_ENABLE_BIT        (1u << 5u)
#define UART1_OUT_ASSERTED_BIT      (1u << 6u)
#define UART1_IN_ENABLE_BIT         (1u << 7u)
#define UART1_IN_ASSERTED_BIT       (1u << 8u)
#define UART1_OUT_READY_BIT         (1u << 9u)
#define UART1_IN_READY_BIT          (1u << 10u)
#define DIV_ZERO_ASSERTED_BIT       (1u << 11u)

#define NUM_INSTRUCTION_TYPES 14u

struct virtual_machine;

unsigned int is_halted(struct virtual_machine *);
unsigned int vm_putc(struct virtual_machine *, unsigned int);
unsigned int vm_getc(struct virtual_machine *, unsigned int *);
void step(struct virtual_machine *);
struct virtual_machine * vm_create(unsigned char [4], unsigned char [4], unsigned char [1][5]);
void vm_destroy(struct virtual_machine *);

#endif
