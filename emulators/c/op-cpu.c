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
#include "op-cpu.h"

/*

C89 implemention of op-cpu emulator.
This implementation assumes that sizeof(unsigned int) == 4

*/

static unsigned int do_interrupt(struct virtual_machine *);
static void fetch_decode_execute(struct virtual_machine *);
void setup_virtual_machine(struct virtual_machine *);
static unsigned int translate_virtual_address_to_linear(struct virtual_machine *, unsigned int, unsigned int, unsigned int, unsigned int *);
static void do_page_fault_exception(struct virtual_machine *, unsigned int, unsigned int, unsigned int, unsigned int);

static void do_page_fault_exception(struct virtual_machine * vm, unsigned int virtual_a, unsigned int access, unsigned int origin_pc, unsigned int level_2_page_pointer){
	/*  Before asserting page fault exception, make sure that it is not already asserted, otherwise
	    that means we're handling a page fault exception and we encountered another page fault exception */
	assert(!(vm->registeruint32[FR_index] & PAGE_FAULT_EXCEPTION_ASSERTED_BIT));
	vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | PAGE_FAULT_EXCEPTION_ASSERTED_BIT;
	/*  Set information that allows the software to handle the PFE */
	vm->memoryuint32[PFE_PAGE_POINTER / sizeof(unsigned int)] = level_2_page_pointer;
	vm->memoryuint32[PFE_PC_VALUE / sizeof(unsigned int)] = origin_pc;
	vm->memoryuint32[PFE_ACCESS / sizeof(unsigned int)] = access;
	vm->memoryuint32[PFE_VIRTUAL / sizeof(unsigned int)] = virtual_a;
}

static unsigned int translate_virtual_address_to_linear(struct virtual_machine * vm, unsigned int virtual_a, unsigned int access, unsigned int origin_pc, unsigned int * linear){
	if(vm->registeruint32[FR_index] & PAGEING_ENABLE_BIT){ /*  Only translate if paging exception is enabled */
		/*  Virtual Address = 32 bits:
		    <LEVEL_2_PAGE_TABLE_NUM_BITS bits for level 2 page index>...
		    <LEVEL_1_PAGE_TABLE_NUM_BITS bits for level 1 page index>...
		    <OP_CPU_PAGE_SIZE_NUM_BITS bits for offset in page>
		*/
		unsigned int level_2_page_table_pointer = vm->memoryuint32[PAGE_POINTER / sizeof(unsigned int)];
		unsigned int level_2_index = (virtual_a & LEVEL_2_PAGE_TABLE_INDEX_MASK) >> (LEVEL_1_PAGE_TABLE_NUM_BITS + OP_CPU_PAGE_SIZE_NUM_BITS);
		unsigned int level_1_index = (virtual_a & LEVEL_1_PAGE_TABLE_INDEX_MASK) >> OP_CPU_PAGE_SIZE_NUM_BITS;
		unsigned int offset = (virtual_a & PAGE_OFFSET_MASK);
		unsigned int level_2_page_table_entry = vm->memoryuint32[(level_2_page_table_pointer / sizeof(unsigned int)) + level_2_index];
		/*  Make sure this level 2 page table entry is valid */
		if(level_2_page_table_entry & LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED){
			unsigned int level_1_page_table_pointer = level_2_page_table_entry & (LEVEL_1_PAGE_TABLE_INDEX_MASK | LEVEL_2_PAGE_TABLE_INDEX_MASK);
			unsigned int level_1_page_table_entry = vm->memoryuint32[(level_1_page_table_pointer / sizeof(unsigned int)) + level_1_index];
			/*  Make sure we have access to this level 1 page table entry, and that it is valid */
			if((level_1_page_table_entry & access) && (level_1_page_table_entry & LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED)){
				unsigned int linear_address_page = level_1_page_table_entry & (LEVEL_1_PAGE_TABLE_INDEX_MASK | LEVEL_2_PAGE_TABLE_INDEX_MASK);
				assert(virtual_a == (linear_address_page + offset)); /*  Remove this to support non-identity mappings */
				*linear = linear_address_page + offset;
				return 0;
			}else{
				do_page_fault_exception(vm, virtual_a, access, origin_pc, level_2_page_table_pointer);
				return 1;
			}
		}else{
			do_page_fault_exception(vm, virtual_a, access, origin_pc, level_2_page_table_pointer);
			return 1;
		}
	}else{
		*linear = virtual_a;
		return 0;
	}
}

static unsigned int do_interrupt(struct virtual_machine * vm){
	unsigned int virtual_address = vm->registeruint32[SP_index] - vm->registeruint32[WR_index];
	unsigned int linear_address = 0;
	if(translate_virtual_address_to_linear(vm, virtual_address, LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT, vm->registeruint32[PC_index], &linear_address)){
		assert(0 && "Can't page fault here.");
	}else{
		vm->memoryuint32[linear_address / sizeof(unsigned int)] = vm->registeruint32[PC_index]; /* [SP - 4] = PC */
		vm->registeruint32[SP_index] = virtual_address; /* SP = SP -4 */
		/*  Disable global interrupts */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] & ~GLOBAL_INTERRUPT_ENABLE_BIT;
		/*  Branch to irq handler */
		vm->registeruint32[PC_index] = vm->memoryuint32[IRQ_HANDLER / sizeof(unsigned int)];
	}
	return 0;
}

static void fetch_decode_execute(struct virtual_machine * vm){
	unsigned int initial_pc = vm->registeruint32[PC_index];
	unsigned int linear_address = 0;
	if(translate_virtual_address_to_linear(vm, initial_pc, LEVEL_1_PAGE_TABLE_ENTRY_EXECUTE_BIT, initial_pc, &linear_address)){
		do_interrupt(vm);
	}else{
		unsigned int current_inst = vm->memoryuint32[linear_address / sizeof(unsigned int)];
		int branch_dist = current_inst & BRANCH_DISTANCE_SIGN_BIT ? -((BRANCH_DISTANCE_MASK & ~(current_inst & BRANCH_DISTANCE_MASK)) + 1) : (current_inst & BRANCH_DISTANCE_MASK);
		unsigned int literal = LITERAL_MASK & current_inst;
		unsigned int ra = (ra_MASK & current_inst) / (1u << ra_OFFSET);
		unsigned int rb = (rb_MASK & current_inst) / (1u << rb_OFFSET);
		unsigned int rc = (rc_MASK & current_inst) / (1u << rc_OFFSET);

		assert(vm->registeruint32[PC_index] / sizeof(unsigned int) < vm->num_memory_words);
		vm->registeruint32[PC_index] += sizeof(unsigned int);

		switch(current_inst & OP_CODE_MASK){
			case ADD_OP_CODE:{
				vm->registeruint32[ra] = vm->registeruint32[rb] + vm->registeruint32[rc];
				break;
			}case SUB_OP_CODE:{
				vm->registeruint32[ra] = vm->registeruint32[rb] - vm->registeruint32[rc];
				break;
			}case MUL_OP_CODE:{
				vm->registeruint32[ra] = vm->registeruint32[rb] * vm->registeruint32[rc];
				break;
			}case DIV_OP_CODE:{
				if(vm->registeruint32[rc]){
					vm->registeruint32[ra] = vm->registeruint32[rb] / vm->registeruint32[rc];
				}else{
					/*  Division by zero detected */
					vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | DIV_ZERO_ASSERTED_BIT;
				}
				break;
			}case BEQ_OP_CODE:{
				if(vm->registeruint32[ra] == vm->registeruint32[rb]){
					vm->registeruint32[PC_index] = vm->registeruint32[PC_index] + (unsigned int)((int)sizeof(unsigned int) * branch_dist);
				}
				break;
			}case BLT_OP_CODE:{
				if(vm->registeruint32[ra] < vm->registeruint32[rb]){
					vm->registeruint32[PC_index] = vm->registeruint32[PC_index] + (unsigned int)((int)sizeof(unsigned int) * branch_dist);
				}
				break;
			}case LOA_OP_CODE:{
				unsigned int loa_linear = 0;
				if(translate_virtual_address_to_linear(vm, vm->registeruint32[rb], LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT, initial_pc, &loa_linear)){
					vm->registeruint32[PC_index] = initial_pc; /*  Restore original PC */
					do_interrupt(vm);
				}else{
					vm->registeruint32[ra] = vm->memoryuint32[loa_linear / sizeof(unsigned int)];
				}
				break;
			}case STO_OP_CODE:{
				unsigned int sto_linear = 0;
				if(translate_virtual_address_to_linear(vm, vm->registeruint32[ra], LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT, initial_pc, &sto_linear)){
					vm->registeruint32[PC_index] = initial_pc; /*  Restore original PC */
					do_interrupt(vm);
				}else{
					vm->memoryuint32[sto_linear / sizeof(unsigned int)] = vm->registeruint32[rb];
				}
				break;
			}case LL_OP_CODE:{
				vm->registeruint32[ra] = literal;
				break;
			}case AND_OP_CODE:{
				vm->registeruint32[ra] = vm->registeruint32[rb] & vm->registeruint32[rc];
				break;
			}case OR_OP_CODE:{
				vm->registeruint32[ra] = vm->registeruint32[rb] | vm->registeruint32[rc];
				break;
			}case NOT_OP_CODE:{
				vm->registeruint32[ra] = ~vm->registeruint32[rb];
				break;
			}case SHR_OP_CODE:{
				vm->registeruint32[ra] = vm->registeruint32[ra] >> vm->registeruint32[rb];
				break;
			}case SHL_OP_CODE:{
				vm->registeruint32[ra] = vm->registeruint32[ra] << vm->registeruint32[rb];
				break;
			}default:{
				assert(0 && "Illegial op code: ");
			}
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

	/*  There should never be a page fault exception when attempting to return from an interrupt */
	assert(!((vm->registeruint32[FR_index] & RTE_BIT) && (vm->registeruint32[FR_index] & PAGE_FAULT_EXCEPTION_ASSERTED_BIT)));

	if(vm->registeruint32[FR_index] & RTE_BIT){
		unsigned int linear_address = 0;
		if(translate_virtual_address_to_linear(vm, vm->registeruint32[SP_index], LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT, vm->registeruint32[PC_index], &linear_address)){
			assert(0 && "Can't page fault here.");
		}else{
			vm->registeruint32[PC_index] = vm->memoryuint32[linear_address / sizeof(unsigned int)]; /* Set PC to [SP] */
			vm->registeruint32[FR_index] = vm->registeruint32[FR_index] & ~RTE_BIT; /* Unset RET bit. */
			vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | GLOBAL_INTERRUPT_ENABLE_BIT; /* Set global interrupt enable. */
			vm->registeruint32[SP_index] = vm->registeruint32[SP_index] + vm->registeruint32[WR_index];/* Set SP to SP + WR */
		}
		return;
	}

	if(vm->registeruint32[FR_index] & GLOBAL_INTERRUPT_ENABLE_BIT){
		if(vm->registeruint32[FR_index] & DIV_ZERO_ASSERTED_BIT){
			do_interrupt(vm);
			return; 
		}else if(vm->registeruint32[FR_index] & TIMER1_ENABLE_BIT && (vm->registeruint32[FR_index] & TIMER1_ASSERTED_BIT)){
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

	/*  Check for timer interrupt condition */
	if(vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)] && vm->cycles_executed % vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)] == 0){
		/*  Assert our timer interrupt */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | TIMER1_ASSERTED_BIT;
		vm->cycles_executed = 0; /*  Avoid eventual overflow */
	}

	fetch_decode_execute(vm);
	vm->cycles_executed = vm->cycles_executed + 1;
}

struct virtual_machine * vm_create(unsigned char data[][5]){
	struct virtual_machine * vm = (struct virtual_machine *)malloc(sizeof(struct virtual_machine));
	unsigned int num_l0_items = ((unsigned int)data[0][1] << 24u) + ((unsigned int)data[0][2] << 16u) + ((unsigned int)data[0][3] << 8u) + (unsigned int)data[0][4];
	unsigned int image_size_bytes = ((unsigned int)data[1][1] << 24u) + ((unsigned int)data[1][2] << 16u) + ((unsigned int)data[1][3] << 8u) + (unsigned int)data[1][4];
	unsigned int starting_offset = ((unsigned int)data[2][1] << 24u) + ((unsigned int)data[2][2] << 16u) + ((unsigned int)data[2][3] << 8u) + (unsigned int)data[2][4];
	unsigned int current_addr = starting_offset;
	unsigned int i = 0;
	unsigned int memory_index = 0;
	assert(data[0][0] == L0_NUM_L0_ITEMS);  /*  Make sure first word was actually the num items. */
	assert(data[1][0] == L0_IMAGE_SIZE);  /*  Make sure second word was actually the image size. */
	assert(data[2][0] == L0_OFFSET_ADDRESS_DIRECTIVE);  /*  Make sure third word was actually the offset. */
	/*  Allocate enough memory to hold all the instructions */

	vm->cycles_executed = 0;
	vm->num_memory_words = image_size_bytes / sizeof(unsigned int);
	vm->num_registers = 1u << BITS_PER_REGISTER;
	vm->memoryuint32 = (unsigned int *)malloc(vm->num_memory_words * sizeof(unsigned int));
	vm->registeruint32 = (unsigned int *)malloc(vm->num_registers * sizeof(unsigned int));

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
	while(i < num_l0_items){
		unsigned int type = data[i][0];
		unsigned int value = ((unsigned int)data[i][1u] << 24u) + ((unsigned int)data[i][2u] << 16u) + ((unsigned int)data[i][3u] << 8u) + (unsigned int)data[i][4u];
		switch(type){
			case L0_MACHINE_INSTRUCTION:{ /*  32 bit machine instruction */
				current_addr += sizeof(unsigned int);
				vm->memoryuint32[memory_index] = value;
				memory_index++;
				break;
			}case L0_DW_DIRECTIVE:{
				current_addr += sizeof(unsigned int);
				vm->memoryuint32[memory_index] = value;
				memory_index++;
				break;
			}case L0_SW_DIRECTIVE:{
				current_addr += sizeof(unsigned int) * value;
				memory_index += value;
				break;
			}case L0_UNRESOLVED_DIRECTIVE:{
				assert(0 && "Trying to load and execute unresolved symbol.");
				break;
			}case L0_OFFSET_ADDRESS_DIRECTIVE:; case L0_STRING_DIRECTIVE:; case L0_LINKAGE_DIRECTIVE:;
			 case L0_FUNCTION_DIRECTIVE: case L0_VARIABLE_DIRECTIVE:; case L0_CONSTANT_DIRECTIVE:; case L0_START_DIRECTIVE:;
			 case L0_END_DIRECTIVE:; case L0_IMPLEMENTED_DIRECTIVE:; case L0_REQUIRED_DIRECTIVE:; case L0_PERMISSION_DIRECTIVE:;
			 case L0_REGION_DIRECTIVE:; case L0_IMAGE_SIZE:; case L0_NUM_L0_ITEMS:{
				/*  Currently ignoring these directives */
				break;
			}default:{
				assert(0 && "Unknown directive.");
			}
		}
		i++;
	}
	assert(current_addr == (starting_offset + image_size_bytes));
	return vm;
}

void vm_destroy(struct virtual_machine * vm){
	free(vm->memoryuint32);
	free(vm->registeruint32);
	free(vm);
}
