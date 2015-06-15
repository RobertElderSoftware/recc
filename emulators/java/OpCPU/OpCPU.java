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
package OpCPU;

class OpCPU{
    OpCPUDataInterface data;

    private static long BITS_PER_BRANCH_DIST = 9L;
    private static long BITS_PER_LITERAL = 16L;
    private static long BITS_PER_OP_CODE = 5L;
    private static long OP_CODE_OFFSET = 27L;
    private static long BITS_PER_REGISTER = 9L;
    private static long ra_OFFSET = 18L;
    private static long rb_OFFSET = OpCPU.ra_OFFSET - OpCPU.BITS_PER_REGISTER;
    private static long rc_OFFSET = OpCPU.rb_OFFSET - OpCPU.BITS_PER_REGISTER;
 
    private static long UNSHIFTED_OP_CODE_MASK = 0xFFFFFFFFL >> (32L - OpCPU.BITS_PER_OP_CODE);
    private static long OP_CODE_MASK = OpCPU.UNSHIFTED_OP_CODE_MASK << OpCPU.OP_CODE_OFFSET;
    private static long UNSHIFTED_REGISTER_MASK = 0xFFFFFFFFL >> (32L - OpCPU.BITS_PER_REGISTER);
    private static long ra_MASK = OpCPU.UNSHIFTED_REGISTER_MASK << OpCPU.ra_OFFSET;
    private static long rb_MASK = OpCPU.UNSHIFTED_REGISTER_MASK << OpCPU.rb_OFFSET;
    private static long rc_MASK = OpCPU.UNSHIFTED_REGISTER_MASK << OpCPU.rc_OFFSET;
    private static long LITERAL_MASK = 0xFFFFFFFFL >> (32L - OpCPU.BITS_PER_LITERAL);
    private static long BRANCH_DISTANCE_MASK = 0xFFFFFFFFL >> (32L - OpCPU.BITS_PER_BRANCH_DIST);
 
    private static long BRANCH_DISTANCE_SIGN_BIT = 0x100L;

    private static long UART1_OUT        = 0x00300000L;
    private static long UART1_IN         = 0x00300010L;
    private static long IRQ_HANDLER      = 0x00300020L;
    private static long TIMER_PERIOD     = 0x00300030L;

    private static int PC_index = 0;
    private static int SP_index = 1;
    private static int FR_index = 4;
    private static int WR_index = 5;
    
    private static long HALTED_BIT                  = 0x1L;
    private static long GLOBAL_INTERRUPT_ENABLE_BIT = 0x2L;
    private static long RTE_BIT                     = 0x4L;
    private static long TIMER1_ENABLE_BIT           = 0x8L;
    private static long TIMER1_ASSERTED_BIT         = 0x10L;
    private static long UART1_OUT_ENABLE_BIT        = 0x20L;
    private static long UART1_OUT_ASSERTED_BIT      = 0x40L;
    private static long UART1_IN_ENABLE_BIT         = 0x80L;
    private static long UART1_IN_ASSERTED_BIT       = 0x100L;
    private static long UART1_OUT_READY_BIT         = 0x200L;
    private static long UART1_IN_READY_BIT          = 0x400L;
    
    private static long numRegisters = 1L << OpCPU.BITS_PER_REGISTER;
    private static long sizeofInt = 4L;

    private long [] memoryUInt32;
    private long numMemoryWords;
    private long cyclesExecuted = 0L;
    private long [] registerUInt32;

    enum InstructionType {
        ADD_INSTRUCTION(0L  << OpCPU.OP_CODE_OFFSET),
        SUB_INSTRUCTION(1L  << OpCPU.OP_CODE_OFFSET),
        MUL_INSTRUCTION(2L  << OpCPU.OP_CODE_OFFSET),
        DIV_INSTRUCTION(3L  << OpCPU.OP_CODE_OFFSET),
        AND_INSTRUCTION(4L  << OpCPU.OP_CODE_OFFSET),
        OR_INSTRUCTION (5L  << OpCPU.OP_CODE_OFFSET),
        NOT_INSTRUCTION(6L  << OpCPU.OP_CODE_OFFSET),
        LOA_INSTRUCTION(7L  << OpCPU.OP_CODE_OFFSET),
        STO_INSTRUCTION(8L  << OpCPU.OP_CODE_OFFSET),
        SHR_INSTRUCTION(9L  << OpCPU.OP_CODE_OFFSET),
        SHL_INSTRUCTION(10L << OpCPU.OP_CODE_OFFSET),
        BEQ_INSTRUCTION(11L << OpCPU.OP_CODE_OFFSET),
        BLT_INSTRUCTION(12L << OpCPU.OP_CODE_OFFSET),
        LL_INSTRUCTION (13L << OpCPU.OP_CODE_OFFSET);
    
        private long value;    

        public static InstructionType lookup(long l){
            for(InstructionType i : values()){
                if(i.getValue() == l){
                    return i;
                }
            }
            return null;
        }
    
        private InstructionType(long value) {
          this.value = value;
        }
    
        public long getValue() {
          return value;
        }
    };

    public OpCPU(OpCPUDataInterface data){
        this.data = data;
        long currentAddr = this.data.getDataStart();
        long endAddr = this.data.getDataEnd();
        long i = 0;
        long memoryIndex = 0;
        this.numMemoryWords = (this.data.getDataEnd() - this.data.getDataStart()) / OpCPU.sizeofInt;
        this.memoryUInt32 = new long[(int)this.numMemoryWords];
        this.registerUInt32 = new long[(int)OpCPU.numRegisters];
    
        /*  Initialize registers */
        for(i = 0; i < 17; i++){
            this.registerUInt32[(int)i] = 0x0L;
        }
        this.registerUInt32[FR_index] = 0x200L;  /* FR */
        this.registerUInt32[WR_index] = OpCPU.sizeofInt;    /* WR */
    
        this.memoryUInt32[(int)(UART1_OUT / OpCPU.sizeofInt)]     = 0L;
        this.memoryUInt32[(int)(UART1_IN / OpCPU.sizeofInt)]      = 0L;
        this.memoryUInt32[(int)(IRQ_HANDLER / OpCPU.sizeofInt)]   = 0L;
        this.memoryUInt32[(int)(TIMER_PERIOD / OpCPU.sizeofInt)]  = 0L;
    
        /*  Go through all the loader directives and load the machine code */
        i = 0;
        while(currentAddr != endAddr){
            long [] item = data.getData(i);
            boolean is_sw_directive = item[0] > 0 ? true : false;
            long value = item[1];
            if(is_sw_directive){ /* Skip Word directive */
                currentAddr += OpCPU.sizeofInt * value;
                memoryIndex += value;
            }else{ /*  32 bit machine instruction */
                currentAddr += OpCPU.sizeofInt;
                this.memoryUInt32[(int)memoryIndex] = value;
                memoryIndex++;
            }
            i++;
        }
    }
    
    private void doInterrupt(){
        /*  Disable global interrupts */
        this.registerUInt32[FR_index] = (this.registerUInt32[FR_index] & ~GLOBAL_INTERRUPT_ENABLE_BIT) & 0xFFFFFFFF;
        /*  Branch to irq handler */
        this.registerUInt32[SP_index] = this.registerUInt32[SP_index] - this.registerUInt32[WR_index]; /* SP = SP -4 */
        this.memoryUInt32[(int)(this.registerUInt32[SP_index] / OpCPU.sizeofInt)] = this.registerUInt32[PC_index]; /* [SP] = PC */
        this.registerUInt32[PC_index] = this.memoryUInt32[(int)(IRQ_HANDLER / OpCPU.sizeofInt)];
    }
    
    private void fetchDecodeExecute(){
        long current_inst = this.memoryUInt32[(int)(this.registerUInt32[PC_index] / OpCPU.sizeofInt)];
        int branch_dist = (int)(((current_inst & BRANCH_DISTANCE_SIGN_BIT) != 0) ? -((BRANCH_DISTANCE_MASK & ~(current_inst & BRANCH_DISTANCE_MASK)) + 1L) : (current_inst & BRANCH_DISTANCE_MASK));
        long literal = OpCPU.LITERAL_MASK & current_inst;
        int ra = (int)((OpCPU.ra_MASK & current_inst) / (1L << OpCPU.ra_OFFSET));
        int rb = (int)((OpCPU.rb_MASK & current_inst) / (1L << OpCPU.rb_OFFSET));
        int rc = (int)((OpCPU.rc_MASK & current_inst) / (1L << OpCPU.rc_OFFSET));

        InstructionType opType = InstructionType.lookup(current_inst & OpCPU.OP_CODE_MASK);

        this.registerUInt32[PC_index] += OpCPU.sizeofInt;
    
        assert(this.registerUInt32[PC_index] / OpCPU.sizeofInt < this.numMemoryWords);
    
        switch(opType){
            case ADD_INSTRUCTION:{
                this.registerUInt32[ra] = (this.registerUInt32[rb] + this.registerUInt32[rc]) & 0xFFFFFFFF;
                break;
            }case SUB_INSTRUCTION:{
                this.registerUInt32[ra] = (this.registerUInt32[rb] - this.registerUInt32[rc]) & 0xFFFFFFFF;
                break;
            }case MUL_INSTRUCTION:{
                this.registerUInt32[ra] = (this.registerUInt32[rb] * this.registerUInt32[rc]) & 0xFFFFFFFF;
                break;
            }case DIV_INSTRUCTION:{
                this.registerUInt32[ra] = (this.registerUInt32[rb] / this.registerUInt32[rc]) & 0xFFFFFFFF;
                break;
            }case BEQ_INSTRUCTION:{
                if(this.registerUInt32[ra] == this.registerUInt32[rb]){
                    this.registerUInt32[PC_index] += OpCPU.sizeofInt * branch_dist;
                }
                break;
            }case BLT_INSTRUCTION:{
                if(this.registerUInt32[ra] < this.registerUInt32[rb]){
                    this.registerUInt32[PC_index] += OpCPU.sizeofInt * branch_dist;
                }
                break;
            }case LOA_INSTRUCTION:{
                assert(this.registerUInt32[rb] / OpCPU.sizeofInt < this.numMemoryWords);
                this.registerUInt32[ra] = this.memoryUInt32[(int)(this.registerUInt32[rb] / OpCPU.sizeofInt)];
                break;
            }case STO_INSTRUCTION:{
                assert(this.registerUInt32[ra] / OpCPU.sizeofInt < this.numMemoryWords);
                this.memoryUInt32[(int)(this.registerUInt32[ra] / OpCPU.sizeofInt)] = this.registerUInt32[rb];
                break;
            }case LL_INSTRUCTION:{
                this.registerUInt32[ra] = literal;
                break;
            }case AND_INSTRUCTION:{
                this.registerUInt32[ra] = (this.registerUInt32[rb] & this.registerUInt32[rc]) & 0xFFFFFFFF;
                break;
            }case OR_INSTRUCTION:{
                this.registerUInt32[ra] = (this.registerUInt32[rb] | this.registerUInt32[rc]) & 0xFFFFFFFF;
                break;
            }case NOT_INSTRUCTION:{
                this.registerUInt32[ra] = (~this.registerUInt32[rb]) & 0xFFFFFFFF;
                break;
            }case SHR_INSTRUCTION:{
                this.registerUInt32[ra] = (this.registerUInt32[ra] >>> this.registerUInt32[rb]) & 0xFFFFFFFF;
                break;
            }case SHL_INSTRUCTION:{
                this.registerUInt32[ra] = (this.registerUInt32[ra] << this.registerUInt32[rb]) & 0xFFFFFFFF;
                break;
            }default:{
                System.out.println("Unknown op code.");
            }
        }
    }
    
    boolean isHalted(){
        return (this.registerUInt32[FR_index] & HALTED_BIT) != 0;
    }
    
    boolean vmPutC(long c){
        if((this.registerUInt32[FR_index] & UART1_IN_READY_BIT) != 0){
            return true; /* Item was NOT input */
        }
        /*  Set the flag bit to indicate there is data */
        this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | UART1_IN_READY_BIT;
        /*  Assert the interrupt */
        this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | UART1_IN_ASSERTED_BIT;
        this.memoryUInt32[(int)(UART1_IN / OpCPU.sizeofInt)] = c;
        return false; /* Item was input */
    }
    
    long [] vmGetC(){
        if((this.registerUInt32[FR_index] & UART1_OUT_READY_BIT) == 0){
            /*  Set the flag bit back to ready. */
            this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | UART1_OUT_READY_BIT;
            /*  Assert the interrupt */
            this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | UART1_OUT_ASSERTED_BIT;
            return new long [] {1, this.memoryUInt32[(int)(UART1_OUT / OpCPU.sizeofInt)]};
        }else{
            return new long [] {0};
        }
    }
    
    public void step() throws Exception {
        if((this.registerUInt32[FR_index] & HALTED_BIT) != 0){
            /* Processor has been halted */
            return; 
        }
    
        fetchDecodeExecute();
        this.cyclesExecuted = this.cyclesExecuted + 1;
    
        if((this.registerUInt32[FR_index] & HALTED_BIT) != 0){
            /*  Processor has been halted */
            return; 
        }
    
        if((this.registerUInt32[FR_index] & RTE_BIT) != 0){
            this.registerUInt32[FR_index] = (this.registerUInt32[FR_index] & ~RTE_BIT) & 0xFFFFFFFF; /* Unset RET bit. */
            this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | GLOBAL_INTERRUPT_ENABLE_BIT; /* Set global interrupt enable. */
            this.registerUInt32[PC_index] = this.memoryUInt32[(int)(this.registerUInt32[SP_index] / OpCPU.sizeofInt)]; /* Set PC to [SP] */
            this.registerUInt32[SP_index] = this.registerUInt32[SP_index] + this.registerUInt32[WR_index];/* Set SP to SP + WR */
            return; 
        }
    
        /*  Check for timer interrupt condition */
        if((this.memoryUInt32[(int)(TIMER_PERIOD / OpCPU.sizeofInt)] != 0) && this.cyclesExecuted % this.memoryUInt32[(int)(TIMER_PERIOD / OpCPU.sizeofInt)] == 0){
            /*  Assert our timer interrupt */
            this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | TIMER1_ASSERTED_BIT;
            return; 
        }
    
        if((this.registerUInt32[FR_index] & GLOBAL_INTERRUPT_ENABLE_BIT) != 0){
            if((this.registerUInt32[FR_index] & TIMER1_ENABLE_BIT) != 0 && (this.registerUInt32[FR_index] & TIMER1_ASSERTED_BIT) != 0){
                doInterrupt();
                return; 
            }else if((this.registerUInt32[FR_index] & UART1_OUT_ENABLE_BIT) != 0 && (this.registerUInt32[FR_index] & UART1_OUT_ASSERTED_BIT) != 0){
                doInterrupt();
                return; 
            }else if((this.registerUInt32[FR_index] & UART1_IN_ENABLE_BIT) != 0 && (this.registerUInt32[FR_index] & UART1_IN_ASSERTED_BIT) != 0){
                doInterrupt();
                return; 
            }
        }
    }
}
