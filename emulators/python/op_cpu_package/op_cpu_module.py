#   Copyright 2019 Robert Elder Software Inc.
#   
#   Licensed under the Apache License, Version 2.0 (the "License"); you may not 
#   use this file except in compliance with the License.  You may obtain a copy 
#   of the License at
#   
#       http://www.apache.org/licenses/LICENSE-2.0
#   
#   Unless required by applicable law or agreed to in writing, software 
#   distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
#   WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
#   License for the specific language governing permissions and limitations 
#   under the License.

class OpCPU(object):

    OP_CPU_PAGE_SIZE_NUM_BITS = 10L;
    LEVEL_1_PAGE_TABLE_NUM_BITS = 11L;
    LEVEL_2_PAGE_TABLE_NUM_BITS = 11L;

    OP_CPU_PAGE_SIZE = (1L << OP_CPU_PAGE_SIZE_NUM_BITS);

    PAGE_OFFSET_MASK = (0xFFFFFFFFL >> (32L - OP_CPU_PAGE_SIZE_NUM_BITS));
    LEVEL_1_PAGE_TABLE_INDEX_MASK = 0x001FFC00L;
    LEVEL_2_PAGE_TABLE_INDEX_MASK = 0xFFE00000L;

    LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED = (0x1L << 9L);
    LEVEL_1_PAGE_TABLE_ENTRY_INITIALIZED = (0x1L << 9L);
 
    LEVEL_1_PAGE_TABLE_ENTRY_EXECUTE_BIT = 1L << 0
    LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT   = 1L << 1
    LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT    = 1L << 2
    BITS_PER_BRANCH_DIST = 9L;
    BITS_PER_LITERAL = 16L;
    BITS_PER_OP_CODE = 5L;
    OP_CODE_OFFSET = 27L;
    BITS_PER_REGISTER = 9L;
    ra_OFFSET = 18L;
    rb_OFFSET = ra_OFFSET - BITS_PER_REGISTER;
    rc_OFFSET = rb_OFFSET - BITS_PER_REGISTER;
    
    UNSHIFTED_OP_CODE_MASK = 0xFFFFFFFFL >> (32L - BITS_PER_OP_CODE);
    OP_CODE_MASK = UNSHIFTED_OP_CODE_MASK << OP_CODE_OFFSET;
    UNSHIFTED_REGISTER_MASK = 0xFFFFFFFFL >> (32L - BITS_PER_REGISTER);
    ra_MASK = UNSHIFTED_REGISTER_MASK << ra_OFFSET;
    rb_MASK = UNSHIFTED_REGISTER_MASK << rb_OFFSET;
    rc_MASK = UNSHIFTED_REGISTER_MASK << rc_OFFSET;
    LITERAL_MASK = 0xFFFFFFFFL >> (32L - BITS_PER_LITERAL);
    BRANCH_DISTANCE_MASK = 0xFFFFFFFFL >> (32L - BITS_PER_BRANCH_DIST);
    
    BRANCH_DISTANCE_SIGN_BIT = 0x100L;

    ADD_INSTRUCTION = 0L  << OP_CODE_OFFSET
    SUB_INSTRUCTION = 1L  << OP_CODE_OFFSET
    MUL_INSTRUCTION = 2L  << OP_CODE_OFFSET
    DIV_INSTRUCTION = 3L  << OP_CODE_OFFSET
    AND_INSTRUCTION = 4L  << OP_CODE_OFFSET
    OR_INSTRUCTION  = 5L  << OP_CODE_OFFSET
    NOT_INSTRUCTION = 6L  << OP_CODE_OFFSET
    LOA_INSTRUCTION = 7L  << OP_CODE_OFFSET
    STO_INSTRUCTION = 8L  << OP_CODE_OFFSET
    SHR_INSTRUCTION = 9L  << OP_CODE_OFFSET
    SHL_INSTRUCTION = 10L << OP_CODE_OFFSET
    BEQ_INSTRUCTION = 11L << OP_CODE_OFFSET
    BLT_INSTRUCTION = 12L << OP_CODE_OFFSET
    LL_INSTRUCTION  = 13L << OP_CODE_OFFSET

    UART1_OUT        = 0x00300000L
    UART1_IN         = 0x00300010L
    IRQ_HANDLER      = 0x00300020L
    TIMER_PERIOD     = 0x00300030L
    PAGE_POINTER     = 0x00300040L
    PFE_PAGE_POINTER = 0x00300044L
    PFE_PC_VALUE     = 0x00300048L
    PFE_ACCESS       = 0x0030004CL
    PFE_VIRTUAL      = 0x00300050L

    PC_index = 0
    SP_index = 1
    FR_index = 4
    WR_index = 5

    HALTED_BIT                          = 1 << 0
    GLOBAL_INTERRUPT_ENABLE_BIT         = 1 << 1
    RTE_BIT                             = 1 << 2
    TIMER1_ENABLE_BIT                   = 1 << 3
    TIMER1_ASSERTED_BIT                 = 1 << 4
    UART1_OUT_ENABLE_BIT                = 1 << 5
    UART1_OUT_ASSERTED_BIT              = 1 << 6
    UART1_IN_ENABLE_BIT                 = 1 << 7
    UART1_IN_ASSERTED_BIT               = 1 << 8
    UART1_OUT_READY_BIT                 = 1 << 9
    UART1_IN_READY_BIT                  = 1 << 10
    DIV_ZERO_ASSERTED_BIT               = 1 << 11
    PAGE_FAULT_EXCEPTION_ASSERTED_BIT   = 1 << 12
    PAGEING_ENABLE_BIT     = 1 << 13

    def __init__(self, loader_data):
        self.cycles_executed = 0
        self.num_memory_words = loader_data.data[1][1] / 4

        #  Set up main memory
        self.memoryuint32 = [0] * self.num_memory_words

        #  Set up registers
        self.registeruint32 = [0] * (1L << OpCPU.BITS_PER_REGISTER)
        self.registeruint32[OpCPU.FR_index] = 0x200
        self.registeruint32[OpCPU.WR_index] = 4 
 
        #  Set up memory mapped registers
        self.memoryuint32[OpCPU.UART1_OUT / 4]     = 0
        self.memoryuint32[OpCPU.UART1_IN / 4]      = 0
        self.memoryuint32[OpCPU.IRQ_HANDLER / 4]   = 0
        self.memoryuint32[OpCPU.TIMER_PERIOD / 4]  = 0
 
        #  Go through all the loader directives and load the machine code
        bytes_loaded = 0
        i = 0
        memory_index = loader_data.data[2][1] / 4 #  Second item should be offset to load into
        while (i < loader_data.data[0][1]):
            load_type = loader_data.data[i][0]
            value = loader_data.data[i][1]
            if load_type == 0: # Machine instruction
                bytes_loaded += 4
                self.memoryuint32[memory_index] = value
                memory_index += 1
            elif load_type == 1: # Define word
                bytes_loaded += 4
                self.memoryuint32[memory_index] = value
                memory_index += 1
            elif load_type == 2: # Skip Word directive 
                bytes_loaded += 4 * value
                memory_index += value
            elif load_type == 3: # offset directive 
                pass
            elif load_type == 4: # string directive 
                pass
            elif load_type == 5: # linkage directive 
                pass
            elif load_type == 6: # unresolved directive 
                print "Attempting to load unresolved address."
            elif load_type == 7: # function directive 
                pass
            elif load_type == 8: # variable directive 
                pass
            elif load_type == 9: # constant directive 
                pass
            elif load_type == 10: # start directive 
                pass
            elif load_type == 11: # end directive 
                pass
            elif load_type == 12: # implemented directive 
                pass
            elif load_type == 13: # required directive 
                pass
            elif load_type == 14: # premission directive 
                pass
            elif load_type == 15: # region directive 
                pass
            elif load_type == 16: # image size directive 
                pass
            elif load_type == 17: # num l0 items directive 
                pass
            else:
                print "Unknown loader directive."
            i += 1


    def do_page_fault_exception(self, virtual, access, origin_pc, level_2_page_pointer):
        #  Before asserting page fault exception, make sure that it is not already asserted, otherwise
        #  that means we're handling a page fault exception and we encountered another page fault exception */
        if not ((self.registeruint32[OpCPU.FR_index] & OpCPU.PAGE_FAULT_EXCEPTION_ASSERTED_BIT) == 0):
            print "nested page fault exception"
        self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] | OpCPU.PAGE_FAULT_EXCEPTION_ASSERTED_BIT;
        #  Set information that allows the software to handle the PFE
        self.memoryuint32[OpCPU.PFE_PAGE_POINTER / 4] = level_2_page_pointer;
        self.memoryuint32[OpCPU.PFE_PC_VALUE / 4] = origin_pc;
        self.memoryuint32[OpCPU.PFE_ACCESS / 4] = access;
        self.memoryuint32[OpCPU.PFE_VIRTUAL / 4] = virtual;

    def translate_virtual_address_to_linear(self, virtual, access, origin_pc):
        if((self.registeruint32[OpCPU.FR_index] & OpCPU.PAGEING_ENABLE_BIT) != 0): #  Only translate if paging exception is enabled
            #  Virtual Address = 32 bits:
            #  <LEVEL_2_PAGE_TABLE_NUM_BITS bits for level 2 page index>...
            #  <LEVEL_1_PAGE_TABLE_NUM_BITS bits for level 1 page index>...
            #  <OP_CPU_PAGE_SIZE_NUM_BITS bits for offset in page>
            level_2_page_table_pointer = self.memoryuint32[OpCPU.PAGE_POINTER / 4];
            level_2_index = (virtual & OpCPU.LEVEL_2_PAGE_TABLE_INDEX_MASK) >> (OpCPU.LEVEL_1_PAGE_TABLE_NUM_BITS + OpCPU.OP_CPU_PAGE_SIZE_NUM_BITS);
            level_1_index = (virtual & OpCPU.LEVEL_1_PAGE_TABLE_INDEX_MASK) >> OpCPU.OP_CPU_PAGE_SIZE_NUM_BITS;
            offset = (virtual & OpCPU.PAGE_OFFSET_MASK);
            level_2_page_table_entry = self.memoryuint32[((level_2_page_table_pointer / 4) + level_2_index)];
            #  Make sure this level 2 page table entry is valid 
            if((level_2_page_table_entry & OpCPU.LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED) != 0):
                level_1_page_table_pointer = level_2_page_table_entry & (OpCPU.LEVEL_1_PAGE_TABLE_INDEX_MASK | OpCPU.LEVEL_2_PAGE_TABLE_INDEX_MASK);
                level_1_page_table_entry = self.memoryuint32[((level_1_page_table_pointer / 4) + level_1_index)];
                #  Make sure we have access to this level 1 page table entry, and that it is valid 
                if(((level_1_page_table_entry & access) != 0) and ((level_1_page_table_entry & OpCPU.LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED) != 0)):
                    linear_address_page = level_1_page_table_entry & (OpCPU.LEVEL_1_PAGE_TABLE_INDEX_MASK | OpCPU.LEVEL_2_PAGE_TABLE_INDEX_MASK);
                    if(not virtual == (linear_address_page + offset)): # Remove this to support non-identity mappings 
                        print "Only supporting identity mapping right now."
                    return False, linear_address_page + offset;
                else:
                    self.do_page_fault_exception(virtual, access, origin_pc, level_2_page_table_pointer);
                    return True, 0;
            else:
                self.do_page_fault_exception(virtual, access, origin_pc, level_2_page_table_pointer);
                return True, 0;
        else:
            return False, virtual;

    def fetch_decode_execute(self):
        initial_pc = self.registeruint32[OpCPU.PC_index]
        did_fault, linear_address = self.translate_virtual_address_to_linear(initial_pc, OpCPU.LEVEL_1_PAGE_TABLE_ENTRY_EXECUTE_BIT, initial_pc)
        if did_fault:
            self.do_interrupt()
        else:
            current_inst = self.memoryuint32[linear_address / 4]
            if (current_inst & OpCPU.BRANCH_DISTANCE_SIGN_BIT):
                branch_dist = -((OpCPU.BRANCH_DISTANCE_MASK & ~(current_inst & OpCPU.BRANCH_DISTANCE_MASK)) + 1)
            else:
                branch_dist = (current_inst & OpCPU.BRANCH_DISTANCE_MASK)
            
            literal = OpCPU.LITERAL_MASK & current_inst
            ra = (OpCPU.ra_MASK & current_inst) / (1L << OpCPU.ra_OFFSET);
            rb = (OpCPU.rb_MASK & current_inst) / (1L << OpCPU.rb_OFFSET);
            rc = (OpCPU.rc_MASK & current_inst) / (1L << OpCPU.rc_OFFSET);
            
            op_type = current_inst & OpCPU.OP_CODE_MASK
            self.registeruint32[OpCPU.PC_index] = 4 + self.registeruint32[OpCPU.PC_index]
            
            if op_type == OpCPU.ADD_INSTRUCTION:
                self.registeruint32[ra] = self.registeruint32[rb] + self.registeruint32[rc]
            elif op_type == OpCPU.SUB_INSTRUCTION:
                self.registeruint32[ra] = self.registeruint32[rb] - self.registeruint32[rc]
            elif op_type == OpCPU.MUL_INSTRUCTION:
                self.registeruint32[ra] = self.registeruint32[rb] * self.registeruint32[rc]
            elif op_type == OpCPU.DIV_INSTRUCTION:
              if(self.registeruint32[rc]):
                self.registeruint32[ra] = self.registeruint32[rb] / self.registeruint32[rc];
              else:
                #  Division by zero detected
                self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] | OpCPU.DIV_ZERO_ASSERTED_BIT;
            elif op_type == OpCPU.BEQ_INSTRUCTION:
                if(self.registeruint32[ra] == self.registeruint32[rb]):
                    self.registeruint32[OpCPU.PC_index] = self.registeruint32[OpCPU.PC_index] + 4 * branch_dist
            elif op_type == OpCPU.BLT_INSTRUCTION:
                if(self.registeruint32[ra] < self.registeruint32[rb]):
                    self.registeruint32[OpCPU.PC_index] = self.registeruint32[OpCPU.PC_index] + 4 * branch_dist
            elif op_type == OpCPU.LOA_INSTRUCTION:
                did_fault, linear_loa = self.translate_virtual_address_to_linear(self.registeruint32[rb], OpCPU.LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT, initial_pc)
                if did_fault:
                    self.registeruint32[OpCPU.PC_index] = initial_pc
                    self.do_interrupt()
                else:
                    self.registeruint32[ra] = self.memoryuint32[linear_loa / 4]
            elif op_type == OpCPU.STO_INSTRUCTION:
                did_fault, linear_sto = self.translate_virtual_address_to_linear(self.registeruint32[ra], OpCPU.LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT, initial_pc)
                if did_fault:
                    self.registeruint32[OpCPU.PC_index] = initial_pc
                    self.do_interrupt()
                else:
                    self.memoryuint32[linear_sto / 4] = self.registeruint32[rb]
            elif op_type == OpCPU.LL_INSTRUCTION:
                self.registeruint32[ra] = literal
            elif op_type == OpCPU.AND_INSTRUCTION:
                self.registeruint32[ra] = self.registeruint32[rb] & self.registeruint32[rc]
            elif op_type == OpCPU.OR_INSTRUCTION:
                self.registeruint32[ra] = self.registeruint32[rb] | self.registeruint32[rc]
            elif op_type == OpCPU.NOT_INSTRUCTION:
                self.registeruint32[ra] = ~self.registeruint32[rb]
            elif op_type == OpCPU.SHR_INSTRUCTION:
                self.registeruint32[ra] = self.registeruint32[ra] >> self.registeruint32[rb]
            elif op_type == OpCPU.SHL_INSTRUCTION:
                self.registeruint32[ra] = self.registeruint32[ra] << self.registeruint32[rb]
            else:
                print "Illegial op code: "

    def do_interrupt(self):
        virtual_address = self.registeruint32[OpCPU.SP_index] - self.registeruint32[OpCPU.WR_index]
        did_fault, linear = self.translate_virtual_address_to_linear(virtual_address, OpCPU.LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT, self.registeruint32[OpCPU.PC_index])
        if did_fault:
            print "Can't page fault here"
        else:
            self.memoryuint32[linear / 4] = self.registeruint32[OpCPU.PC_index] # [SP - 4] = PC
            self.registeruint32[OpCPU.SP_index] = virtual_address # SP = SP -4
            #  Disable global interrupts
            self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] & ~OpCPU.GLOBAL_INTERRUPT_ENABLE_BIT
            #  Branch to irq handler 
            self.registeruint32[OpCPU.PC_index] = self.memoryuint32[OpCPU.IRQ_HANDLER / 4]


    def is_halted(self):
        return self.registeruint32[OpCPU.FR_index] & OpCPU.HALTED_BIT

    def vm_getc(self):
        if(not(self.registeruint32[OpCPU.FR_index] & OpCPU.UART1_OUT_READY_BIT)):
            self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] | OpCPU.UART1_OUT_READY_BIT #  Set the flag bit back to ready. 
            self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] | OpCPU.UART1_OUT_ASSERTED_BIT #  Assert the interrupt 
            return {'chr': self.memoryuint32[OpCPU.UART1_OUT / 4]}
        else:
            return {}

    def vm_putc(self, c):
        if(self.registeruint32[OpCPU.FR_index] & OpCPU.UART1_IN_READY_BIT):
            return 1 # Item was NOT input 

        #  Set the flag bit to indicate there is data 
        self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] | OpCPU.UART1_IN_READY_BIT
        self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] | OpCPU.UART1_IN_ASSERTED_BIT  #  Assert the interrupt 
        self.memoryuint32[OpCPU.UART1_IN / 4] = c
        return 0 # Item was input 

    def step(self):
        if(self.registeruint32[OpCPU.FR_index] & OpCPU.HALTED_BIT):
            return #   Processor has been halted

        #  There should never be a page fault exception when attempting to return from an interrupt 
        if((self.registeruint32[OpCPU.FR_index] & OpCPU.RTE_BIT) and (self.registeruint32[OpCPU.FR_index] & OpCPU.PAGE_FAULT_EXCEPTION_ASSERTED_BIT)):
            print "RTE and page fault exception are both set."
 
        if(self.registeruint32[OpCPU.FR_index] & OpCPU.RTE_BIT):
            did_fault, linear = self.translate_virtual_address_to_linear(self.registeruint32[OpCPU.SP_index], OpCPU.LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT, self.registeruint32[OpCPU.PC_index])
            if did_fault:
                print "Can't fault here."
            else:
                self.registeruint32[OpCPU.PC_index] = self.memoryuint32[linear / 4]                                             # Set PC to [SP]
                self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] & ~OpCPU.RTE_BIT                      # Unset RET bit.
                self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] | OpCPU.GLOBAL_INTERRUPT_ENABLE_BIT   # Set global interrupt enable.
                self.registeruint32[OpCPU.SP_index] = self.registeruint32[OpCPU.SP_index] + self.registeruint32[OpCPU.WR_index] # Set SP to SP + WR 
            return 


 
        if(self.registeruint32[OpCPU.FR_index] & OpCPU.GLOBAL_INTERRUPT_ENABLE_BIT):
            if(self.registeruint32[OpCPU.FR_index] & OpCPU.DIV_ZERO_ASSERTED_BIT):
                self.do_interrupt()
                return
            elif(self.registeruint32[OpCPU.FR_index] & OpCPU.TIMER1_ENABLE_BIT and (self.registeruint32[OpCPU.FR_index] & OpCPU.TIMER1_ASSERTED_BIT)):
                self.do_interrupt()
                return 
            elif(self.registeruint32[OpCPU.FR_index] & OpCPU.UART1_OUT_ENABLE_BIT and (self.registeruint32[OpCPU.FR_index] & OpCPU.UART1_OUT_ASSERTED_BIT)):
                self.do_interrupt()
                return 
            elif(self.registeruint32[OpCPU.FR_index] & OpCPU.UART1_IN_ENABLE_BIT and (self.registeruint32[OpCPU.FR_index] & OpCPU.UART1_IN_ASSERTED_BIT)):
                self.do_interrupt()
                return 

        #  Check for timer interrupt condition
        if(self.memoryuint32[OpCPU.TIMER_PERIOD / 4] and self.cycles_executed % self.memoryuint32[OpCPU.TIMER_PERIOD / 4] == 0):
            self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] | OpCPU.TIMER1_ASSERTED_BIT  #  Assert our timer interrupt 
            self.cycles_executed = 0 #  Avoid eventual overflow 

        self.fetch_decode_execute()
        self.cycles_executed += 1
