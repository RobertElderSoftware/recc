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
#include "op-cpu.h"

/*

C89 implemention of op-cpu emulator.
This implementation assumes that sizeof(unsigned int) == 4

*/

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

#define UART1_OUT        0x00300000
#define UART1_IN         0x00300010
#define IRQ_HANDLER      0x00300020
#define TIMER_PERIOD     0x00300030

#define PC_index 0
#define SP_index 1
#define FR_index 4
#define WR_index 5

#define HALTED_BIT                  (1 << 0)
#define GLOBAL_INTERRUPT_ENABLE_BIT (1 << 1)
#define RTE_BIT                     (1 << 2)
#define TIMER1_ENABLE_BIT           (1 << 3)
#define TIMER1_ASSERTED_BIT         (1 << 4)
#define UART1_OUT_ENABLE_BIT        (1 << 5)
#define UART1_OUT_ASSERTED_BIT      (1 << 6)
#define UART1_IN_ENABLE_BIT         (1 << 7)
#define UART1_IN_ASSERTED_BIT       (1 << 8)
#define UART1_OUT_READY_BIT         (1 << 9)
#define UART1_IN_READY_BIT          (1 << 10)

#define NUM_INSTRUCTION_TYPES 14

enum instruction_type {
	ADD_INSTRUCTION,
	SUB_INSTRUCTION,
	MUL_INSTRUCTION,
	DIV_INSTRUCTION,
	BEQ_INSTRUCTION,
	BLT_INSTRUCTION,
	LOA_INSTRUCTION,
	STO_INSTRUCTION,
	LL_INSTRUCTION,
	AND_INSTRUCTION,
	OR_INSTRUCTION,
	NOT_INSTRUCTION,
	SHR_INSTRUCTION,
	SHL_INSTRUCTION
};

struct virtual_machine {
	unsigned int cycles_executed;
	unsigned int num_memory_words;
	unsigned int num_registers;
	unsigned int * memoryuint32;
	unsigned int * registeruint32;
};

static unsigned int instruction_op_codes [NUM_INSTRUCTION_TYPES][2] = {
	{ADD_INSTRUCTION, 0u  << OP_CODE_OFFSET },
	{SUB_INSTRUCTION, 1u  << OP_CODE_OFFSET },
	{MUL_INSTRUCTION, 2u  << OP_CODE_OFFSET },
	{DIV_INSTRUCTION, 3u  << OP_CODE_OFFSET },
	{AND_INSTRUCTION, 4u  << OP_CODE_OFFSET },
	{OR_INSTRUCTION , 5u  << OP_CODE_OFFSET },
	{NOT_INSTRUCTION, 6u  << OP_CODE_OFFSET },
	{LOA_INSTRUCTION, 7u  << OP_CODE_OFFSET },
	{STO_INSTRUCTION, 8u  << OP_CODE_OFFSET },
	{SHR_INSTRUCTION, 9u  << OP_CODE_OFFSET },
	{SHL_INSTRUCTION, 10u << OP_CODE_OFFSET },
	{BEQ_INSTRUCTION, 11u << OP_CODE_OFFSET },
	{BLT_INSTRUCTION, 12u << OP_CODE_OFFSET },
	{LL_INSTRUCTION , 13u << OP_CODE_OFFSET }
};

static enum instruction_type lookup_instruction_op_code(unsigned int);
static void do_interrupt(struct virtual_machine *);
static void fetch_decode_execute(struct virtual_machine *);
void setup_virtual_machine(struct virtual_machine *);

static enum instruction_type lookup_instruction_op_code(unsigned int op_code){
	unsigned int i;
	for(i = 0; i < NUM_INSTRUCTION_TYPES; i++){
		if(instruction_op_codes[i][1] == op_code){
			return instruction_op_codes[i][0];
		}
	}
	assert(0 && "Not possible.");
}

static void do_interrupt(struct virtual_machine * vm){
	/*  Disable global interrupts */
	vm->registeruint32[FR_index] = vm->registeruint32[FR_index] & ~GLOBAL_INTERRUPT_ENABLE_BIT;
	/*  Branch to irq handler */
	vm->registeruint32[SP_index] = vm->registeruint32[SP_index] - vm->registeruint32[WR_index]; /* SP = SP -4 */
	vm->memoryuint32[vm->registeruint32[SP_index] / sizeof(unsigned int)] = vm->registeruint32[PC_index]; /* [SP] = PC */
	vm->registeruint32[PC_index] = vm->memoryuint32[IRQ_HANDLER / sizeof(unsigned int)];
}

static void fetch_decode_execute(struct virtual_machine * vm){
	unsigned int current_inst = vm->memoryuint32[vm->registeruint32[PC_index] / sizeof(unsigned int)];
	int branch_dist = current_inst & BRANCH_DISTANCE_SIGN_BIT ? -((BRANCH_DISTANCE_MASK & ~(current_inst & BRANCH_DISTANCE_MASK)) + 1) : (current_inst & BRANCH_DISTANCE_MASK);
	unsigned int literal = LITERAL_MASK & current_inst;
	unsigned int ra = (ra_MASK & current_inst) / (1u << ra_OFFSET);
	unsigned int rb = (rb_MASK & current_inst) / (1u << rb_OFFSET);
	unsigned int rc = (rc_MASK & current_inst) / (1u << rc_OFFSET);

	enum instruction_type op_type = lookup_instruction_op_code(current_inst & OP_CODE_MASK);
	vm->registeruint32[PC_index] += sizeof(unsigned int);

	assert(vm->registeruint32[PC_index] / sizeof(unsigned int) < vm->num_memory_words);

	switch(op_type){
		case ADD_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] + vm->registeruint32[rc];
			break;
		}case SUB_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] - vm->registeruint32[rc];
			break;
		}case MUL_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] * vm->registeruint32[rc];
			break;
		}case DIV_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] / vm->registeruint32[rc];
			break;
		}case BEQ_INSTRUCTION:{
			if(vm->registeruint32[ra] == vm->registeruint32[rb]){
				vm->registeruint32[PC_index] += sizeof(unsigned int) * branch_dist;
			}
			break;
		}case BLT_INSTRUCTION:{
			if(vm->registeruint32[ra] < vm->registeruint32[rb]){
				vm->registeruint32[PC_index] += sizeof(unsigned int) * branch_dist;
			}
			break;
		}case LOA_INSTRUCTION:{
			assert(vm->registeruint32[rb] / sizeof(unsigned int) < vm->num_memory_words);
			vm->registeruint32[ra] = vm->memoryuint32[vm->registeruint32[rb]/sizeof(unsigned int)];
			break;
		}case STO_INSTRUCTION:{
			assert(vm->registeruint32[ra] / sizeof(unsigned int) < vm->num_memory_words);
			vm->memoryuint32[vm->registeruint32[ra]/sizeof(unsigned int)] = vm->registeruint32[rb];
			break;
		}case LL_INSTRUCTION:{
			vm->registeruint32[ra] = literal;
			break;
		}case AND_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] & vm->registeruint32[rc];
			break;
		}case OR_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] | vm->registeruint32[rc];
			break;
		}case NOT_INSTRUCTION:{
			vm->registeruint32[ra] = ~vm->registeruint32[rb];
			break;
		}case SHR_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[ra] >> vm->registeruint32[rb];
			break;
		}case SHL_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[ra] << vm->registeruint32[rb];
			break;
		}default:{
			assert(0 && "Illegial op code: ");
		}
	}
}

unsigned int is_halted(struct virtual_machine * vm){
	return vm->registeruint32[FR_index] & HALTED_BIT;
}

unsigned int vm_putc(struct virtual_machine * vm, unsigned int c){
	if(vm->registeruint32[FR_index] & UART1_IN_READY_BIT){
		return 1; /* Item was NOT input */
	}
	/*  Set the flag bit to indicate there is data */
	vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | UART1_IN_READY_BIT;
	/*  Assert the interrupt */
	vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | UART1_IN_ASSERTED_BIT;
	vm->memoryuint32[UART1_IN / sizeof(unsigned int)] = c;
	return 0; /* Item was input */
}

unsigned int vm_getc(struct virtual_machine * vm, unsigned int * rtn){
	if(!(vm->registeruint32[FR_index] & UART1_OUT_READY_BIT)){
		/*  Set the flag bit back to ready. */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | UART1_OUT_READY_BIT;
		/*  Assert the interrupt */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | UART1_OUT_ASSERTED_BIT;
		*rtn = vm->memoryuint32[UART1_OUT / sizeof(unsigned int)];
		return 1;
	}else{
		return 0;
	}
}

void step(struct virtual_machine * vm){
	if(vm->registeruint32[FR_index] & HALTED_BIT){
		/* Processor has been halted */
		return; 
	}

	fetch_decode_execute(vm);
	vm->cycles_executed = vm->cycles_executed + 1;

	if(vm->registeruint32[FR_index] & HALTED_BIT){
		/*  Processor has been halted */
		return; 
	}

	if(vm->registeruint32[FR_index] & RTE_BIT){
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] & ~RTE_BIT; /* Unset RET bit. */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | GLOBAL_INTERRUPT_ENABLE_BIT; /* Set global interrupt enable. */
		vm->registeruint32[PC_index] = vm->memoryuint32[vm->registeruint32[SP_index] / sizeof(unsigned int)]; /* Set PC to [SP] */
		vm->registeruint32[SP_index] = vm->registeruint32[SP_index] + vm->registeruint32[WR_index];/* Set SP to SP + WR */
		return; 
	}

	/*  Check for timer interrupt condition */
	if(vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)] && vm->cycles_executed % vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)] == 0){
		/*  Assert our timer interrupt */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | TIMER1_ASSERTED_BIT;
		vm->cycles_executed = 0; /*  Avoid eventual overflow */
		return; 
	}

	if(vm->registeruint32[FR_index] & GLOBAL_INTERRUPT_ENABLE_BIT){
		if(vm->registeruint32[FR_index] & TIMER1_ENABLE_BIT && (vm->registeruint32[FR_index] & TIMER1_ASSERTED_BIT)){
			do_interrupt(vm);
			return; 
		}else if(vm->registeruint32[FR_index] & UART1_OUT_ENABLE_BIT && (vm->registeruint32[FR_index] & UART1_OUT_ASSERTED_BIT)){
			do_interrupt(vm);
			return; 
		}else if(vm->registeruint32[FR_index] & UART1_IN_ENABLE_BIT && (vm->registeruint32[FR_index] & UART1_IN_ASSERTED_BIT)){
			do_interrupt(vm);
			return; 
		}
	}
}

struct virtual_machine * vm_create(unsigned char start[4], unsigned char end[4], unsigned char data[1][5]){
	struct virtual_machine * vm = malloc(sizeof(struct virtual_machine));
	unsigned int start_addr = (start[0] << 24) + (start[1] << 16) + (start[2] << 8) + start[3];
	unsigned int end_addr = (end[0] << 24) + (end[1] << 16) + (end[2] << 8) + end[3];
	unsigned int current_addr = start_addr;
	unsigned int i = 0;
	unsigned int memory_index = 0;
	/*  Allocate enough memory to hold all the instructions */

	vm->cycles_executed = 0;
	vm->num_memory_words = (end_addr - start_addr) / sizeof(unsigned int);
	vm->num_registers = 1u << BITS_PER_REGISTER;
	vm->memoryuint32 = malloc(vm->num_memory_words * sizeof(unsigned int));
	vm->registeruint32 = malloc(vm->num_registers * sizeof(unsigned int));

	/*  Initialize registers */
	for(i = 0; i < 17; i++){
		vm->registeruint32[i] = 0x0;
	}
	vm->registeruint32[FR_index] = 0x200;  /* FR */
	vm->registeruint32[WR_index] = sizeof(unsigned int);    /* WR */

	vm->memoryuint32[UART1_OUT / sizeof(unsigned int)]     = 0;
	vm->memoryuint32[UART1_IN / sizeof(unsigned int)]      = 0;
	vm->memoryuint32[IRQ_HANDLER / sizeof(unsigned int)]   = 0;
	vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)]  = 0;

	/*  Go through all the loader directives and load the machine code */
	i = 0;
	while(current_addr != end_addr){
		unsigned int is_sw_directive = data[i][0];
		unsigned int value = (data[i][1] << 24) + (data[i][2] << 16) + (data[i][3] << 8) + data[i][4];
		if(is_sw_directive){ /* Skip Word directive */
			current_addr += sizeof(unsigned int) * value;
			memory_index += value;
		}else{ /*  32 bit machine instruction */
			current_addr += sizeof(unsigned int);
			vm->memoryuint32[memory_index] = value;
			memory_index++;
		}
		i++;
	}

	return vm;
}

void vm_destroy(struct virtual_machine * vm){
	free(vm->memoryuint32);
	free(vm->registeruint32);
	free(vm);
}
