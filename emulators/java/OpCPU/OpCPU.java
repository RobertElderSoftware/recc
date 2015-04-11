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

    private static long LITERAL22BITMASK = 0x003FFFFFL;
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
    
    private static long numRegisters = 64L;
    private static long sizeofInt = 4L;
    private static long numMemoryWords = 0L;
    private static long cyclesExecuted = 0L;

    private static long [] memoryUInt32;
    private static long [] registerUInt32;

    enum InstructionType {
        ADD_INSTRUCTION(0x00000000L),
        SUB_INSTRUCTION(0x10000000L),
        MUL_INSTRUCTION(0x20000000L),
        DIV_INSTRUCTION(0x30000000L),
        BEQ_INSTRUCTION(0x40000000L),
        BLT_INSTRUCTION(0x50000000L),
        LOA_INSTRUCTION(0x60000000L),
        STO_INSTRUCTION(0x70000000L),
        LL_INSTRUCTION(0x80000000L),
        AND_INSTRUCTION(0x90000000L),
        OR_INSTRUCTION(0xA0000000L),
        NOT_INSTRUCTION(0xB0000000L),
        SHR_INSTRUCTION(0xC0000000L),
        SHL_INSTRUCTION(0xD0000000L);
    
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
        this.numMemoryWords = (this.data.getDataEnd() - this.data.getDataStart()) / this.sizeofInt;
        this.memoryUInt32 = new long[(int)this.numMemoryWords];
        this.registerUInt32 = new long[(int)this.numRegisters];
    
        /*  Initialize registers */
        for(i = 0; i < 17; i++){
            this.registerUInt32[(int)i] = 0x0L;
        }
        this.registerUInt32[FR_index] = 0x200L;  /* FR */
        this.registerUInt32[WR_index] = this.sizeofInt;    /* WR */
    
        this.memoryUInt32[(int)(UART1_OUT / this.sizeofInt)]     = 0L;
        this.memoryUInt32[(int)(UART1_IN / this.sizeofInt)]      = 0L;
        this.memoryUInt32[(int)(IRQ_HANDLER / this.sizeofInt)]   = 0L;
        this.memoryUInt32[(int)(TIMER_PERIOD / this.sizeofInt)]  = 0L;
    
        /*  Go through all the loader directives and load the machine code */
        i = 0;
        while(currentAddr != endAddr){
            long [] item = data.getData(i);
            boolean is_sw_directive = item[0] > 0 ? true : false;
            long value = item[1];
            if(is_sw_directive){ /* Skip Word directive */
                currentAddr += this.sizeofInt * value;
                memoryIndex += value;
            }else{ /*  32 bit machine instruction */
                currentAddr += this.sizeofInt;
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
        this.memoryUInt32[(int)(this.registerUInt32[SP_index] / this.sizeofInt)] = this.registerUInt32[PC_index]; /* [SP] = PC */
        this.registerUInt32[PC_index] = this.memoryUInt32[(int)(IRQ_HANDLER / this.sizeofInt)];
    }
    
    private void fetchDecodeExecute(){
        long current_inst = this.memoryUInt32[(int)(this.registerUInt32[PC_index] / this.sizeofInt)];
        int branch_dist = (int)(((current_inst & 0x8000L) != 0) ? -((0xFFFFL & ~(current_inst & 0xFFFFL)) + 1L) : (current_inst & 0xFFFFL));
        long literal22bit = LITERAL22BITMASK & current_inst;
        int ra = (int)((0x0FC00000L & current_inst) / 0x400000L);
        int rb = (int)((0x003F0000L & current_inst) / 0x10000L);
        int rc = (int)((0x0000FC00L & current_inst) / 0x400L);
        int rd = (int)((0x000003F0L & current_inst) / 0x10L);
        InstructionType opType = InstructionType.lookup(current_inst & 0xF0000000L);


        this.registerUInt32[PC_index] += this.sizeofInt;
    
        assert(this.registerUInt32[PC_index] / this.sizeofInt < this.numMemoryWords);
    
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
                long c = this.registerUInt32[rc];
                long d = this.registerUInt32[rd];
                this.registerUInt32[ra] = (c / d) & 0xFFFFFFFF;
                this.registerUInt32[rb] = (c % d) & 0xFFFFFFFF;
                break;
            }case BEQ_INSTRUCTION:{
                if(this.registerUInt32[ra] == this.registerUInt32[rb]){
                    this.registerUInt32[PC_index] += this.sizeofInt * branch_dist;
                }
                break;
            }case BLT_INSTRUCTION:{
                if(this.registerUInt32[ra] < this.registerUInt32[rb]){
                    this.registerUInt32[PC_index] += this.sizeofInt * branch_dist;
                }
                break;
            }case LOA_INSTRUCTION:{
                assert(this.registerUInt32[rb] / this.sizeofInt < this.numMemoryWords);
                this.registerUInt32[ra] = this.memoryUInt32[(int)(this.registerUInt32[rb] / this.sizeofInt)];
                break;
            }case STO_INSTRUCTION:{
                assert(this.registerUInt32[ra] / this.sizeofInt < this.numMemoryWords);
                this.memoryUInt32[(int)(this.registerUInt32[ra] / this.sizeofInt)] = this.registerUInt32[rb];
                break;
            }case LL_INSTRUCTION:{
                this.registerUInt32[ra] = literal22bit;
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
        this.memoryUInt32[(int)(UART1_IN / this.sizeofInt)] = c;
        return false; /* Item was input */
    }
    
    long [] vmGetC(){
        if((this.registerUInt32[FR_index] & UART1_OUT_READY_BIT) == 0){
            /*  Set the flag bit back to ready. */
            this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | UART1_OUT_READY_BIT;
            /*  Assert the interrupt */
            this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | UART1_OUT_ASSERTED_BIT;
            return new long [] {1, this.memoryUInt32[(int)(UART1_OUT / this.sizeofInt)]};
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
            this.registerUInt32[PC_index] = this.memoryUInt32[(int)(this.registerUInt32[SP_index] / this.sizeofInt)]; /* Set PC to [SP] */
            this.registerUInt32[SP_index] = this.registerUInt32[SP_index] + this.registerUInt32[WR_index];/* Set SP to SP + WR */
            return; 
        }
    
        /*  Check for timer interrupt condition */
        if((this.memoryUInt32[(int)(TIMER_PERIOD / this.sizeofInt)] != 0) && this.cyclesExecuted % this.memoryUInt32[(int)(TIMER_PERIOD / this.sizeofInt)] == 0){
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
