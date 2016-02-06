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
package OpCPU;

/*  TODO:  This emulator will have issues dealing with memory addresses above 2^31 since the implementation of 'int' in java is a signed
    32-bit integer which cannot represent all the addresses that a 32-bit unsigned integer can.
    This can be fixed by emulating the required address as 'long' types.

*/

class OpCPU{
    OpCPUDataInterface data;

    private static long OP_CPU_PAGE_SIZE_NUM_BITS = 10L;
    private static long LEVEL_1_PAGE_TABLE_NUM_BITS = 11L;
    private static long LEVEL_2_PAGE_TABLE_NUM_BITS = 11L;

    private static long OP_CPU_PAGE_SIZE = (1L << OpCPU.OP_CPU_PAGE_SIZE_NUM_BITS);

    private static long PAGE_OFFSET_MASK = (0xFFFFFFFFL >> (32L - OpCPU.OP_CPU_PAGE_SIZE_NUM_BITS));
    private static long LEVEL_1_PAGE_TABLE_INDEX_MASK = 0x001FFC00L;
    private static long LEVEL_2_PAGE_TABLE_INDEX_MASK = 0xFFE00000L;

    private static long LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED = (0x1L << 9L);
    private static long LEVEL_1_PAGE_TABLE_ENTRY_INITIALIZED = (0x1L << 9L);

    private static long LEVEL_1_PAGE_TABLE_ENTRY_EXECUTE_BIT = 1L << 0;
    private static long LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT   = 1L << 1;
    private static long LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT    = 1L << 2;
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
    private static long PAGE_POINTER     = 0x00300040L;
    private static long PFE_PAGE_POINTER = 0x00300044L;
    private static long PFE_PC_VALUE     = 0x00300048L;
    private static long PFE_ACCESS       = 0x0030004CL;
    private static long PFE_VIRTUAL      = 0x00300050L;

    private static int PC_index = 0;
    private static int SP_index = 1;
    private static int FR_index = 4;
    private static int WR_index = 5;
    
    private static long HALTED_BIT                           = 0x1L;
    private static long GLOBAL_INTERRUPT_ENABLE_BIT          = 0x2L;
    private static long RTE_BIT                              = 0x4L;
    private static long TIMER1_ENABLE_BIT                    = 0x8L;
    private static long TIMER1_ASSERTED_BIT                  = 0x10L;
    private static long UART1_OUT_ENABLE_BIT                 = 0x20L;
    private static long UART1_OUT_ASSERTED_BIT               = 0x40L;
    private static long UART1_IN_ENABLE_BIT                  = 0x80L;
    private static long UART1_IN_ASSERTED_BIT                = 0x100L;
    private static long UART1_OUT_READY_BIT                  = 0x200L;
    private static long UART1_IN_READY_BIT                   = 0x400L;
    private static long DIV_ZERO_ASSERTED_BIT                = 0x800L;
    private static long PAGE_FAULT_EXCEPTION_ASSERTED_BIT    = 0x1000L;
    private static long PAGEING_ENABLE_BIT      = 0x2000L;
    
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
        assert(this.data.getData(0L)[1] == 17); /*  Make sure it is actually the number of items */
        assert(this.data.getData(1L)[1] == 16); /*  Make sure it is actually the image size */
        assert(this.data.getData(2L)[1] == 3); /*  Make sure it is actually the load offset */
        this.data = data;
        long numL0Items = this.data.getData(0L)[1];
        long imageSize = this.data.getData(1L)[1];
        long initialOffset = this.data.getData(2L)[1];
        long i = 0;
        long memoryIndex = initialOffset / OpCPU.sizeofInt;
        this.numMemoryWords = imageSize / OpCPU.sizeofInt;
        this.memoryUInt32 = new long[(int)this.numMemoryWords];
        this.registerUInt32 = new long[(int)OpCPU.numRegisters];
        long currentAddr = initialOffset;
    
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
        while(i < numL0Items){
            long [] item = data.getData(i);
            long value = item[1];
            switch ((int)item[0]){
                case 0:{ /*  Machine instruction */
                    currentAddr += OpCPU.sizeofInt;
                    this.memoryUInt32[(int)memoryIndex] = value;
                    memoryIndex++;
                    break;
                }case 1:{ /*  DW directive */
                    currentAddr += OpCPU.sizeofInt;
                    this.memoryUInt32[(int)memoryIndex] = value;
                    memoryIndex++;
                    break;
                }case 2:{ /* SW directive */
                    currentAddr += OpCPU.sizeofInt * value;
                    memoryIndex += value;
                    break;
                }case 3:{ /* offset directive */
                    break;
                }case 4:{ /* string directive */
                    break;
                }case 5:{ /* linkage directive */
                    break;
                }case 6:{ /* unresolved symbol  */
                    assert(false);
                    break;
                }case 7:{ /* function directive */
                    break;
                }case 8:{ /* variable directive */
                    break;
                }case 9:{ /* constant directive */
                    break;
                }case 10:{ /* start directive */
                    break;
                }case 11:{ /* end directive */
                    break;
                }case 12:{ /* implemented directive */
                    break;
                }case 13:{ /* required directive */
                    break;
                }case 14:{ /* permission directive */
                    break;
                }case 15:{ /* region directive */
                    break;
                }case 16:{ /* image size directive */
                    break;
                }case 17:{ /* num l0 items directive */
                    break;
                }default:{
                    assert(false);
                }
            }
            i++;
        }
        assert(currentAddr == (imageSize + initialOffset));
    }
    
    private int doInterrupt(){
        long virtualAddress = this.registerUInt32[SP_index] - this.registerUInt32[WR_index];
        long [] linearAddress = new long[]{0};
        if(this.translateVirtualAddressToLinear(virtualAddress, LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT, this.registerUInt32[PC_index], linearAddress)){
            assert(false);
        }else{
            this.memoryUInt32[(int)(linearAddress[0] / OpCPU.sizeofInt)] = this.registerUInt32[PC_index]; /* [SP - 4] = PC */
            this.registerUInt32[SP_index] = virtualAddress; /* SP = SP -4 */
            /*  Disable global interrupts */
            this.registerUInt32[FR_index] = (this.registerUInt32[FR_index] & ~GLOBAL_INTERRUPT_ENABLE_BIT) & 0xFFFFFFFF;
            /*  Branch to irq handler */
            this.registerUInt32[PC_index] = this.memoryUInt32[(int)(IRQ_HANDLER / OpCPU.sizeofInt)];
        }
        return 0;
    }

    private void doPageFaultException(long virtual, long access, long origin_pc, long level_2_page_pointer){
        /*  Before asserting page fault exception, make sure that it is not already asserted, otherwise
            that means we're handling a page fault exception and we encountered another page fault exception */
        assert((this.registerUInt32[FR_index] & PAGE_FAULT_EXCEPTION_ASSERTED_BIT) == 0);
        this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | PAGE_FAULT_EXCEPTION_ASSERTED_BIT;
        /*  Set information that allows the software to handle the PFE */
        this.memoryUInt32[(int)(PFE_PAGE_POINTER / OpCPU.sizeofInt)] = level_2_page_pointer;
        this.memoryUInt32[(int)(PFE_PC_VALUE / OpCPU.sizeofInt)] = origin_pc;
        this.memoryUInt32[(int)(PFE_ACCESS / OpCPU.sizeofInt)] = access;
        this.memoryUInt32[(int)(PFE_VIRTUAL / OpCPU.sizeofInt)] = virtual;
    }

    private boolean translateVirtualAddressToLinear(long virtual, long access, long origin_pc, long [] linearAddress){
        if((this.registerUInt32[FR_index] & PAGEING_ENABLE_BIT) != 0){ /*  Only translate if paging exception is enabled */
            /*  Virtual Address = 32 bits:
                <LEVEL_2_PAGE_TABLE_NUM_BITS bits for level 2 page index>...
                <LEVEL_1_PAGE_TABLE_NUM_BITS bits for level 1 page index>...
                <OP_CPU_PAGE_SIZE_NUM_BITS bits for offset in page>
            */
            long level_2_page_table_pointer = this.memoryUInt32[(int)(PAGE_POINTER / OpCPU.sizeofInt)];
            long level_2_index = (virtual & LEVEL_2_PAGE_TABLE_INDEX_MASK) >> (LEVEL_1_PAGE_TABLE_NUM_BITS + OP_CPU_PAGE_SIZE_NUM_BITS);
            long level_1_index = (virtual & LEVEL_1_PAGE_TABLE_INDEX_MASK) >> OP_CPU_PAGE_SIZE_NUM_BITS;
            long offset = (virtual & PAGE_OFFSET_MASK);
            long level_2_page_table_entry = this.memoryUInt32[(int)((level_2_page_table_pointer / OpCPU.sizeofInt) + level_2_index)];
            /*  Make sure this level 2 page table entry is valid */
            if((level_2_page_table_entry & LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED) != 0){
                long level_1_page_table_pointer = level_2_page_table_entry & (LEVEL_1_PAGE_TABLE_INDEX_MASK | LEVEL_2_PAGE_TABLE_INDEX_MASK);
                long level_1_page_table_entry = this.memoryUInt32[(int)((level_1_page_table_pointer / OpCPU.sizeofInt) + level_1_index)];
                /*  Make sure we have access to this level 1 page table entry, and that it is valid */
                if(((level_1_page_table_entry & access) != 0) && ((level_1_page_table_entry & LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED) != 0)){
                    long linear_address_page = level_1_page_table_entry & (LEVEL_1_PAGE_TABLE_INDEX_MASK | LEVEL_2_PAGE_TABLE_INDEX_MASK);
                    assert(virtual == (linear_address_page + offset)); /*  Remove this to support non-identity mappings */
                    linearAddress[0] = linear_address_page + offset;
                    return false;
                }else{
                    this.doPageFaultException(virtual, access, origin_pc, level_2_page_table_pointer);
                    return true;
                }
            }else{
                this.doPageFaultException(virtual, access, origin_pc, level_2_page_table_pointer);
                return true;
            }
        }else{
            linearAddress[0] = virtual;
            return false;
        }
    }
    
    private void fetchDecodeExecute(){
        long initial_pc = this.registerUInt32[PC_index];
        long [] linearAddress = new long[]{0};
        if(this.translateVirtualAddressToLinear(initial_pc, LEVEL_1_PAGE_TABLE_ENTRY_EXECUTE_BIT, initial_pc, linearAddress)){
            doInterrupt();
        }else{
            long current_inst = this.memoryUInt32[(int)(linearAddress[0] / OpCPU.sizeofInt)];
            long branchBits = current_inst & BRANCH_DISTANCE_MASK;
            int branch_dist = (int)(((current_inst & BRANCH_DISTANCE_SIGN_BIT) != 0) ? -((BRANCH_DISTANCE_MASK & ~branchBits) + 1L) : branchBits);
            long literal = OpCPU.LITERAL_MASK & current_inst;
            int ra = (int)((OpCPU.ra_MASK & current_inst) / (1L << OpCPU.ra_OFFSET));
            int rb = (int)((OpCPU.rb_MASK & current_inst) / (1L << OpCPU.rb_OFFSET));
            int rc = (int)((OpCPU.rc_MASK & current_inst) / (1L << OpCPU.rc_OFFSET));
            
            InstructionType opType = InstructionType.lookup(current_inst & OpCPU.OP_CODE_MASK);
            
            this.registerUInt32[PC_index] += OpCPU.sizeofInt;
            
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
                    if(this.registerUInt32[rc] != 0){
                      this.registerUInt32[ra] = (this.registerUInt32[rb] / this.registerUInt32[rc]) & 0xFFFFFFFF;
                    }else{
                      /*  Division by zero detected */
                      this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | DIV_ZERO_ASSERTED_BIT;
                    }
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
                    long [] linearLOA = new long[]{0};
                    if(this.translateVirtualAddressToLinear(this.registerUInt32[rb], LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT, initial_pc, linearLOA)){
                        this.registerUInt32[PC_index] = initial_pc;
                        doInterrupt();
                    }else{
                        this.registerUInt32[ra] = this.memoryUInt32[(int)(linearLOA[0] / OpCPU.sizeofInt)];
                    }
                    break;
                }case STO_INSTRUCTION:{
                    long [] linearSTO = new long[]{0};
                    if(this.translateVirtualAddressToLinear(this.registerUInt32[ra], LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT, initial_pc, linearSTO)){
                        this.registerUInt32[PC_index] = initial_pc;
                        doInterrupt();
                    }else{
                        this.memoryUInt32[(int)(linearSTO[0] / OpCPU.sizeofInt)] = this.registerUInt32[rb];
                    }
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

        /*  There should never be a page fault exception when attempting to return from an interrupt */
        assert(((this.registerUInt32[FR_index] & RTE_BIT) == 0) ||  (this.registerUInt32[FR_index] & PAGE_FAULT_EXCEPTION_ASSERTED_BIT) == 0);
    
        if((this.registerUInt32[FR_index] & RTE_BIT) != 0){
            long [] linearAddress = new long[]{0};
            if(this.translateVirtualAddressToLinear(this.registerUInt32[SP_index], LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT, this.registerUInt32[PC_index], linearAddress)){
                assert(false);
            }else{
                this.registerUInt32[PC_index] = this.memoryUInt32[(int)(linearAddress[0] / OpCPU.sizeofInt)]; /* Set PC to [SP] */
                this.registerUInt32[FR_index] = (this.registerUInt32[FR_index] & ~RTE_BIT) & 0xFFFFFFFF; /* Unset RET bit. */
                this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | GLOBAL_INTERRUPT_ENABLE_BIT; /* Set global interrupt enable. */
                this.registerUInt32[SP_index] = this.registerUInt32[SP_index] + this.registerUInt32[WR_index];/* Set SP to SP + WR */
            }
            return; 
        }


        if((this.registerUInt32[FR_index] & GLOBAL_INTERRUPT_ENABLE_BIT) != 0){
            if((this.registerUInt32[FR_index] & DIV_ZERO_ASSERTED_BIT) != 0){
                doInterrupt();
                return; 
            }else if((this.registerUInt32[FR_index] & TIMER1_ENABLE_BIT) != 0 && (this.registerUInt32[FR_index] & TIMER1_ASSERTED_BIT) != 0){
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

        /*  Check for timer interrupt condition */
        if((this.memoryUInt32[(int)(TIMER_PERIOD / OpCPU.sizeofInt)] != 0) && this.cyclesExecuted % this.memoryUInt32[(int)(TIMER_PERIOD / OpCPU.sizeofInt)] == 0){
            /*  Assert our timer interrupt */
            this.registerUInt32[FR_index] = this.registerUInt32[FR_index] | TIMER1_ASSERTED_BIT;
            this.cyclesExecuted = 0;
        }

        fetchDecodeExecute();
        this.cyclesExecuted = this.cyclesExecuted + 1;
    }
}
