#    
#    Copyright 2015 Robert Elder Software Inc.  All rights reserved.
#    
#    This software is not currently available under any license, and unauthorized use
#    or copying is not permitted.
#    
#    This software will likely be available under a common open source license in the
#    near future.  Licensing is currently pending feedback from a lawyer.  If you have
#    an opinion on this subject you can send it to recc [at] robertelder.org.
#    
#    This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
#    Software Inc. be liable for incidental or consequential damages in connection with
#    use of this software.
#    

class OpCPU(object):

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

    PC_index = 0
    SP_index = 1
    FR_index = 4
    WR_index = 5

    HALTED_BIT                  = 1 << 0
    GLOBAL_INTERRUPT_ENABLE_BIT = 1 << 1
    RTE_BIT                     = 1 << 2
    TIMER1_ENABLE_BIT           = 1 << 3
    TIMER1_ASSERTED_BIT         = 1 << 4
    UART1_OUT_ENABLE_BIT        = 1 << 5
    UART1_OUT_ASSERTED_BIT      = 1 << 6
    UART1_IN_ENABLE_BIT         = 1 << 7
    UART1_IN_ASSERTED_BIT       = 1 << 8
    UART1_OUT_READY_BIT         = 1 << 9
    UART1_IN_READY_BIT          = 1 << 10

    def __init__(self, loader_data):
        self.cycles_executed = 0
        self.num_memory_words = (loader_data.data_end - loader_data.data_start) / 4

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
        current_addr = loader_data.data_start
        i = 0
        memory_index = 0
        while (current_addr != loader_data.data_end):
            is_sw_directive = loader_data.data[i][0]
            value = loader_data.data[i][1]
            if is_sw_directive: # Skip Word directive 
                current_addr += 4 * value
                memory_index += value
            else: #  32 bit machine instruction
                current_addr += 4
                self.memoryuint32[memory_index] = value
                memory_index += 1
            i += 1

    def fetch_decode_execute(self):
        current_inst = self.memoryuint32[self.registeruint32[OpCPU.PC_index] / 4]
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
            self.registeruint32[ra] = self.registeruint32[rb] / self.registeruint32[rc]
        elif op_type == OpCPU.BEQ_INSTRUCTION:
            if(self.registeruint32[ra] == self.registeruint32[rb]):
                self.registeruint32[OpCPU.PC_index] = self.registeruint32[OpCPU.PC_index] + 4 * branch_dist
        elif op_type == OpCPU.BLT_INSTRUCTION:
            if(self.registeruint32[ra] < self.registeruint32[rb]):
                self.registeruint32[OpCPU.PC_index] = self.registeruint32[OpCPU.PC_index] + 4 * branch_dist
        elif op_type == OpCPU.LOA_INSTRUCTION:
            self.registeruint32[ra] = self.memoryuint32[self.registeruint32[rb]/4]
        elif op_type == OpCPU.STO_INSTRUCTION:
            self.memoryuint32[self.registeruint32[ra]/4] = self.registeruint32[rb]
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
        #  Disable global interrupts 
        self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] & ~OpCPU.GLOBAL_INTERRUPT_ENABLE_BIT
        #  Branch to irq handler 
        self.registeruint32[OpCPU.SP_index] = self.registeruint32[OpCPU.SP_index] - self.registeruint32[OpCPU.WR_index] # SP = SP -4
        self.memoryuint32[self.registeruint32[OpCPU.SP_index] / 4] = self.registeruint32[OpCPU.PC_index] # [SP] = PC
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
 
        self.fetch_decode_execute()
        self.cycles_executed += 1
 
        if(self.registeruint32[OpCPU.FR_index] & OpCPU.HALTED_BIT):
            return #   Processor has been halted
 
        if(self.registeruint32[OpCPU.FR_index] & OpCPU.RTE_BIT):
            self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] & ~OpCPU.RTE_BIT                      # Unset RET bit.
            self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] | OpCPU.GLOBAL_INTERRUPT_ENABLE_BIT   # Set global interrupt enable.
            self.registeruint32[OpCPU.PC_index] = self.memoryuint32[self.registeruint32[OpCPU.SP_index] / 4]                # Set PC to [SP]
            self.registeruint32[OpCPU.SP_index] = self.registeruint32[OpCPU.SP_index] + self.registeruint32[OpCPU.WR_index] # Set SP to SP + WR 
            return 
 
        #  Check for timer interrupt condition
        if(self.memoryuint32[OpCPU.TIMER_PERIOD / 4] and self.cycles_executed % self.memoryuint32[OpCPU.TIMER_PERIOD / 4] == 0):
            self.registeruint32[OpCPU.FR_index] = self.registeruint32[OpCPU.FR_index] | OpCPU.TIMER1_ASSERTED_BIT  #  Assert our timer interrupt 
            self.cycles_executed = 0 #  Avoid eventual overflow 
            return 
 
        if(self.registeruint32[OpCPU.FR_index] & OpCPU.GLOBAL_INTERRUPT_ENABLE_BIT):
            if(self.registeruint32[OpCPU.FR_index] & OpCPU.TIMER1_ENABLE_BIT and (self.registeruint32[OpCPU.FR_index] & OpCPU.TIMER1_ASSERTED_BIT)):
                self.do_interrupt()
                return 
            elif(self.registeruint32[OpCPU.FR_index] & OpCPU.UART1_OUT_ENABLE_BIT and (self.registeruint32[OpCPU.FR_index] & OpCPU.UART1_OUT_ASSERTED_BIT)):
                self.do_interrupt()
                return 
            elif(self.registeruint32[OpCPU.FR_index] & OpCPU.UART1_IN_ENABLE_BIT and (self.registeruint32[OpCPU.FR_index] & OpCPU.UART1_IN_ASSERTED_BIT)):
                self.do_interrupt()
                return 
