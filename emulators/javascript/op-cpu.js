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
"use strict";

function opCPU(l0_description) {
  if(l0_description.data[0][0] != 17){ alert("First word must be num l0 items."); }
  if(l0_description.data[1][0] != 16){ alert("Second word must be image size."); }
  if(l0_description.data[2][0] != 3){ alert("Third word must be offset to load."); }

  this.OP_CPU_PAGE_SIZE_NUM_BITS = 10 >>> 0;
  this.LEVEL_1_PAGE_TABLE_NUM_BITS = 11 >>> 0;
  this.LEVEL_2_PAGE_TABLE_NUM_BITS = 11 >>> 0;

  this.OP_CPU_PAGE_SIZE = (1 << this.OP_CPU_PAGE_SIZE_NUM_BITS) >>> 0;

  this.PAGE_OFFSET_MASK = ((0xFFFFFFFF >>> 0) >>> (32 - this.OP_CPU_PAGE_SIZE_NUM_BITS)) >>> 0;
  this.LEVEL_1_PAGE_TABLE_INDEX_MASK = 0x001FFC00 >>> 0;
  this.LEVEL_2_PAGE_TABLE_INDEX_MASK = 0xFFE00000 >>> 0;

  this.LEVEL_1_PAGE_TABLE_ENTRY_EXECUTE_BIT    = (1 << 0) >>> 0;
  this.LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT      = (1 << 1) >>> 0;
  this.LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT       = (1 << 2) >>> 0;
  this.LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED    = (1 << 9) >>> 0;
  this.LEVEL_1_PAGE_TABLE_ENTRY_INITIALIZED    = (1 << 9) >>> 0;
  this.BITS_PER_BRANCH_DIST = 9;
  this.BITS_PER_LITERAL = 16;
  this.BITS_PER_OP_CODE = 5;
  this.OP_CODE_OFFSET = 27;
  this.BITS_PER_REGISTER = 9;
  this.ra_OFFSET = 18;
  this.rb_OFFSET = this.ra_OFFSET - this.BITS_PER_REGISTER;
  this.rc_OFFSET = this.rb_OFFSET - this.BITS_PER_REGISTER;

  this.UNSHIFTED_OP_CODE_MASK = (~0) >>> (32 - this.BITS_PER_OP_CODE);
  this.OP_CODE_MASK = (this.UNSHIFTED_OP_CODE_MASK << this.OP_CODE_OFFSET) >>> 0;
  this.UNSHIFTED_REGISTER_MASK = (~0) >>> (32 - this.BITS_PER_REGISTER);
  this.ra_MASK = this.UNSHIFTED_REGISTER_MASK << this.ra_OFFSET;
  this.rb_MASK = this.UNSHIFTED_REGISTER_MASK << this.rb_OFFSET;
  this.rc_MASK = this.UNSHIFTED_REGISTER_MASK << this.rc_OFFSET;
  this.LITERAL_MASK = (~0) >>> (32 - this.BITS_PER_LITERAL);
  this.BRANCH_DISTANCE_MASK = (~0) >>> (32 - this.BITS_PER_BRANCH_DIST);

  this.BRANCH_DISTANCE_SIGN_BIT = 0x100;

  this._asm_op_codes = {
    "add": (0  << this.OP_CODE_OFFSET) >>> 0,
    "sub": (1  << this.OP_CODE_OFFSET) >>> 0,
    "mul": (2  << this.OP_CODE_OFFSET) >>> 0,
    "div": (3  << this.OP_CODE_OFFSET) >>> 0,
    "and": (4  << this.OP_CODE_OFFSET) >>> 0,
    "or" : (5  << this.OP_CODE_OFFSET) >>> 0,
    "not": (6  << this.OP_CODE_OFFSET) >>> 0,
    "loa": (7  << this.OP_CODE_OFFSET) >>> 0,
    "sto": (8  << this.OP_CODE_OFFSET) >>> 0,
    "shr": (9  << this.OP_CODE_OFFSET) >>> 0,
    "shl": (10 << this.OP_CODE_OFFSET) >>> 0,
    "beq": (11 << this.OP_CODE_OFFSET) >>> 0,
    "blt": (12 << this.OP_CODE_OFFSET) >>> 0,
    "ll" : (13 << this.OP_CODE_OFFSET) >>> 0
  };

  this._asm_templates = {
    "add": ["register", "register", "register"],
    "sub": ["register", "register", "register"],
    "mul": ["register", "register", "register"],
    "div": ["register", "register", "register", "register"],
    "beq": ["register", "register", "decimal-number"],
    "blt": ["register", "register", "decimal-number"],
    "loa": ["register", "register"],
    "sto": ["register", "register"],
    "ll":  ["register", "hexadecimal-number"],
    "and": ["register", "register", "register"],
    "or":  ["register", "register", "register"],
    "not": ["register", "register"],
    "shr": ["register", "register"],
    "shl": ["register", "register"],
    "dw":  ["hexadecimal-number"],
    "sw":  ["hexadecimal-number"]
  };

  this.HALTED_BIT                          = 1 << 0;
  this.GLOBAL_INTERRUPT_ENABLE_BIT         = 1 << 1;
  this.RTE_BIT                             = 1 << 2;
  this.TIMER1_ENABLE_BIT                   = 1 << 3;
  this.TIMER1_ASSERTED_BIT                 = 1 << 4;
  this.UART1_OUT_ENABLE_BIT                = 1 << 5;
  this.UART1_OUT_ASSERTED_BIT              = 1 << 6;
  this.UART1_IN_ENABLE_BIT                 = 1 << 7;
  this.UART1_IN_ASSERTED_BIT               = 1 << 8;
  this.UART1_OUT_READY_BIT                 = 1 << 9;
  this.UART1_IN_READY_BIT                  = 1 << 10;
  this.DIV_ZERO_ASSERTED_BIT               = 1 << 11;
  this.PAGE_FAULT_EXCEPTION_ASSERTED_BIT   = 1 << 12;
  this.PAGEING_ENABLE_BIT     = 1 << 13;

  this.PC_index = 0;
  this.SP_index = 1;
  this.FP_index = 2;
  this.ZR_index = 3;
  this.FR_index = 4;
  this.WR_index = 5;
  
  this.num_registers = 1 << this.BITS_PER_REGISTER;
  this.sizeof_int = 0x4;

  this.MAIN_MEMORY_SIZE = l0_description.data[1][1];
  this.MAIN_MEMORY = new ArrayBuffer(this.MAIN_MEMORY_SIZE);
  this.REGISTERS = new ArrayBuffer(this.num_registers * this.sizeof_int);
  this.registeruint32 = new Uint32Array(this.REGISTERS);
  this.memoryuint32 = new Uint32Array(this.MAIN_MEMORY);
  this.memory_size_ints = this.memoryuint32.length;
  this.enable_stack_zeroing = 0; /* Must not be enabled when running kernel */

  this.UART1_OUT         = 0x00300000;
  this.UART1_IN          = 0x00300010;
  this.IRQ_HANDLER       = 0x00300020;
  this.TIMER_PERIOD      = 0x00300030;
  this.PAGE_POINTER      = 0x00300040;
  this.PFE_PAGE_POINTER  = 0x00300044;
  this.PFE_PC_VALUE      = 0x00300048;
  this.PFE_ACCESS        = 0x0030004C;
  this.PFE_VIRTUAL       = 0x00300050;

  this.cycles_executed = 0;
  this.watch_active = 0;
  this.watch_address = 0x1ffff4;
  this.watch_modified = 0;
  this.last_watch_value = 0;
  this.breakpoint_address = 0;
  this.breakpoint_active = 0;
  this.breakpoint_tripped = 0;
  this.recent_pc_values = [];

  //  Set the program counter to the instruction at 0.
  this.registeruint32[this.PC_index] = 0;
  this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] | this.UART1_OUT_READY_BIT;
  this.registeruint32[this.WR_index] = this.sizeof_int; /*  Set the word register to 4 */

  var machine_instructions = new Uint32Array(this.MAIN_MEMORY);
  var instruction_index = l0_description.data[2][1] / this.sizeof_int;
  /*  Load all machine instructions */
  for(var i = 0; i < l0_description.data.length; i++){
    if((this.MAIN_MEMORY_SIZE / this.sizeof_int) < instruction_index){
      alert("Not enough memory to assemble.");
    }
    switch(l0_description.data[i][0]){
      case 0:{ /*  Machine instruction */
        machine_instructions[instruction_index] = l0_description.data[i][1];
        instruction_index++;
        break;
      }case 1:{ /*  define word */
        machine_instructions[instruction_index] = l0_description.data[i][1];
        instruction_index++;
        break;
      }case 2:{ /*  skip words */
        for(var j = 0; j < l0_description.data[i][1]; j++){
          machine_instructions[instruction_index] = 0;
          instruction_index++;
        }
        break;
      }case 3:{ /*  offset */
        break;
      }case 4:{ /*  string */
        break;
      }case 5:{ /*  linker */
        break;
      }case 6:{ /*  unresolved*/
        alert("Attempting to load an unresolved symbol.");
        break;
      }case 7:{ /*  function directive */
        break;
      }case 8:{ /*  variable directive */
        break;
      }case 9:{ /*  constant directive */
        break;
      }case 10:{ /*  start directive */
        break;
      }case 11:{ /*  end directive */
        break;
      }case 12:{ /*  implemented directive */
        break;
      }case 13:{ /*  required directive */
        break;
      }case 14:{ /*  premission directive */
        break;
      }case 15:{ /*  region directive */
        break;
      }case 16:{ /*  image size directive */
        break;
      }case 17:{ /*  num l0 items directive */
        break;
      }default:{
        alert("Unknown loader directive.");
      }
    }
  }
}


opCPU.prototype.do_page_fault_exception = function (virtual, access, origin_pc, level_2_page_table_pointer){
  /*  Before asserting page fault exception, make sure that it is not already asserted, otherwise
      that means we're handling a page fault exception and we encountered another page fault exception */
  this.assert(!(this.registeruint32[this.FR_index] & this.PAGE_FAULT_EXCEPTION_ASSERTED_BIT), "Nested page fault exception");
  this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] | this.PAGE_FAULT_EXCEPTION_ASSERTED_BIT;
  /*  Set information that allows the software to handle the PFE */
  this.memoryuint32[this.PFE_PAGE_POINTER / this.sizeof_int] = level_2_page_table_pointer;
  this.memoryuint32[this.PFE_PC_VALUE / this.sizeof_int] = origin_pc;
  this.memoryuint32[this.PFE_ACCESS / this.sizeof_int] = access;
  this.memoryuint32[this.PFE_VIRTUAL / this.sizeof_int] = virtual;
}

opCPU.prototype.translate_virtual_address_to_linear = function (virtual, access, origin_pc, linear_address){
  if(this.registeruint32[this.FR_index] & this.PAGEING_ENABLE_BIT){ /*  Only translate if paging exception is enabled */
    /*  Virtual Address = 32 bits:
        <LEVEL_2_PAGE_TABLE_NUM_BITS bits for level 2 page index>...
        <LEVEL_1_PAGE_TABLE_NUM_BITS bits for level 1 page index>...
        <OP_CPU_PAGE_SIZE_NUM_BITS bits for offset in page>
    */
    var level_2_page_table_pointer = this.memoryuint32[this.PAGE_POINTER / this.sizeof_int];
    var level_2_index = (((virtual & this.LEVEL_2_PAGE_TABLE_INDEX_MASK) >>> 0) >>> (this.LEVEL_1_PAGE_TABLE_NUM_BITS + this.OP_CPU_PAGE_SIZE_NUM_BITS)) >>> 0;
    var level_1_index = (((virtual & this.LEVEL_1_PAGE_TABLE_INDEX_MASK) >>> 0) >>> this.OP_CPU_PAGE_SIZE_NUM_BITS) >>> 0;
    var offset = (virtual & this.PAGE_OFFSET_MASK);
    var level_2_page_table_entry = this.memoryuint32[(level_2_page_table_pointer / this.sizeof_int) + level_2_index];
    /*  Make sure this level 2 page table entry is valid */
    if(level_2_page_table_entry & this.LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED){
      var level_1_page_table_pointer = (level_2_page_table_entry & ((this.LEVEL_1_PAGE_TABLE_INDEX_MASK | this.LEVEL_2_PAGE_TABLE_INDEX_MASK) >>> 0)) >>> 0;
      var level_1_page_table_entry = this.memoryuint32[(level_1_page_table_pointer / this.sizeof_int) + level_1_index];
      /*  Make sure we have access to this level 1 page table entry, and that it is valid */
      if((level_1_page_table_entry & access) && (level_1_page_table_entry & this.LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED)){
        var linear_address_page = (level_1_page_table_entry & ((this.LEVEL_1_PAGE_TABLE_INDEX_MASK | this.LEVEL_2_PAGE_TABLE_INDEX_MASK) >>> 0)) >>> 0;
        this.assert(virtual == (linear_address_page + offset), "Not identity mapping."); /*  Remove this to support non-identity mappings */
        linear_address[0] = linear_address_page + offset;
        return 0;
      }else{
        this.do_page_fault_exception(virtual, access, origin_pc, level_2_page_table_pointer);
        return 1;
      }
    }else{
      this.do_page_fault_exception(virtual, access, origin_pc, level_2_page_table_pointer);
      return 1;
    }
  }else{
    linear_address[0] = virtual;
    return 0;
  }
}

opCPU.prototype.assert = function (expression, message){
  if(!expression){
    alert(message);
  }
}

opCPU.prototype.assert_memory_index_in_range = function (index, thing){
  //  Halt the processor if we get to this condition.
  if(!(index < this.memory_size_ints)){
    this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] | this.HALTED_BIT;
  }

  var pc_values = "";
  for(var i = 0; i < this.recent_pc_values.length; i++){
    pc_values += ", 0x" + this.recent_pc_values[i].toString(16);
  }

  this.assert(
    index < this.memory_size_ints,
    thing + " was out of bounds at index " + index + " but memory only contains " + this.memory_size_ints + " ints.  Recent PC values:" + pc_values
  );
}

opCPU.prototype.perform_stack_zeroing = function (ra, newvalue, oldvalue){
  /*  In order for premption to work, it is critical that any important information is always located on or under
      the current value of the stack pointer.  This function helps in testing this by explicitly zeroing any poped
      stack items.  This should cause missbehaving code to fail even without premption, and cause obvious failures
      that can be debugged.
  */
  if(!this.enable_stack_zeroing){
    return;
  }
  if(ra == 1){ /*  If it's the SP register */
    if(newvalue > oldvalue){  /*  If this operation pops the stack */
      var words_poped = (newvalue - oldvalue) / this.sizeof_int;
      for(var i = 0; i < words_poped; i++){
        this.memoryuint32[(oldvalue - (i * this.sizeof_int))/this.sizeof_int] = 0; /* Zero this poped item */
      }
    }
  }
}

opCPU.prototype.fetch_decode_execute = function(){
  /*
  Too slow to use all the time.
  this.recent_pc_values.push(this.registeruint32[this.PC_index]);

  if(this.recent_pc_values.length > 30){
    this.recent_pc_values.shift();
  }
  */

  var initial_pc = this.registeruint32[this.PC_index];
  var linear_address = [];
  if(this.translate_virtual_address_to_linear(initial_pc, this.LEVEL_1_PAGE_TABLE_ENTRY_EXECUTE_BIT, initial_pc, linear_address)){
    this.do_interrupt();
  }else{
    var current_inst = this.memoryuint32[linear_address[0] / this.sizeof_int];
    var low_bits = (current_inst & this.BRANCH_DISTANCE_MASK) >>> 0;
    var branch_dist = (current_inst & this.BRANCH_DISTANCE_SIGN_BIT) >>> 0 ? -((((this.BRANCH_DISTANCE_MASK & ~(low_bits)) >>> 0) + 1) >>> 0) : low_bits;
    this.registeruint32[this.PC_index] += this.sizeof_int;
    var operation = (this.OP_CODE_MASK & current_inst)>>>0;
    var literal = (this.LITERAL_MASK & current_inst)>>>0;
    
    var ra = (this.ra_MASK & current_inst) / (1 << this.ra_OFFSET);
    var rb = (this.rb_MASK & current_inst) / (1 << this.rb_OFFSET);
    var rc = (this.rc_MASK & current_inst) / (1 << this.rc_OFFSET);
    
    switch(operation){
      case this._asm_op_codes["add"]:{
        var sp_value_before = this.registeruint32[this.SP_index];
        this.registeruint32[ra] = this.registeruint32[rb] + this.registeruint32[rc];
        this.perform_stack_zeroing(ra, this.registeruint32[ra], sp_value_before);
        break;
      }case this._asm_op_codes["sub"]:{
        var sp_value_before = this.registeruint32[this.SP_index];
        this.registeruint32[ra] = this.registeruint32[rb] - this.registeruint32[rc];
        this.perform_stack_zeroing(ra, this.registeruint32[ra], sp_value_before);
        break;
      }case this._asm_op_codes["mul"]:{
        /*  Unfortunately, this must be done in parts to avoid intermediate floating point rounding errors
            that will cause inaccurate results for overflowing unsigned multiplication operations eg.
            0xFFFFFFFF * 0xFFFFFFFF will = 0 (but it should be 1)
        */
        var mask_low = 0xFFFF;
        var mask_high = 0xFFFF0000;
        var low_b = (mask_low >>> 0) & this.registeruint32[rb];
        var low_c = (mask_low >>> 0) & this.registeruint32[rc];
        var high_b = ((mask_high >>> 0) & this.registeruint32[rb]) >>> 16;
        var high_c = ((mask_high >>> 0) & this.registeruint32[rc]) >>> 16;
        var low_sum = (low_b * low_c);
        var mid_sum = (high_b * low_c) + (low_b * high_c);
        /*var high_sum = high_b * high_c; The contribution of this factor would produce bits higher than the 32nd bit */
        var low_result = (low_sum >>> 0) & (mask_low >>> 0);
        var high_result_low = (((low_sum >>> 0) & (mask_high >>> 0)) >>> 16);
        var high_result_mid = (((mid_sum >>> 0) & (mask_low >>> 0)) >>> 0);
        var high_result = (((((high_result_low + high_result_mid) >>> 0) & (mask_low >>> 0)) << 16) >>> 0);
        this.registeruint32[ra] = (low_result >>> 0) | (high_result >>> 0);
        break;
      }case this._asm_op_codes["div"]:{
        if(this.registeruint32[rc]){
          this.registeruint32[ra] = this.registeruint32[rb] / this.registeruint32[rc];
        }else{
          /*  Division by zero detected */
          this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] | this.DIV_ZERO_ASSERTED_BIT;
        }
        break;
      }case this._asm_op_codes["beq"]:{
        if(this.registeruint32[ra] == this.registeruint32[rb]){
          this.registeruint32[this.PC_index] += this.sizeof_int * branch_dist;
        }
        break;
      }case this._asm_op_codes["blt"]:{
        if(this.registeruint32[ra] < this.registeruint32[rb]){
          this.registeruint32[this.PC_index] += this.sizeof_int * branch_dist;
        }
        break;
      }case this._asm_op_codes["loa"]:{
        var linear_loa = [];
        if(this.translate_virtual_address_to_linear(this.registeruint32[rb], this.LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT, initial_pc, linear_loa)){
          this.registeruint32[this.PC_index] = initial_pc;
          this.do_interrupt();
        }else{
          var sp_value_before = this.registeruint32[this.SP_index];
          this.registeruint32[ra] = this.memoryuint32[linear_loa[0] / this.sizeof_int];
          this.perform_stack_zeroing(ra, this.registeruint32[ra], sp_value_before);
        }
        break;
      }case this._asm_op_codes["sto"]:{
        var linear_sto = [];
        if(this.translate_virtual_address_to_linear(this.registeruint32[ra], this.LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT, initial_pc, linear_sto)){
          this.registeruint32[this.PC_index] = initial_pc;
          this.do_interrupt();
        }else{
          this.memoryuint32[linear_sto[0] / this.sizeof_int] = this.registeruint32[rb];
        }
        break;
      }case this._asm_op_codes["ll"]:{
        this.registeruint32[ra] = literal;
        break;
      }case this._asm_op_codes["and"]:{
        this.registeruint32[ra] = this.registeruint32[rb] & this.registeruint32[rc];
        break;
      }case this._asm_op_codes["or"]:{
        this.registeruint32[ra] = this.registeruint32[rb] | this.registeruint32[rc];
        break;
      }case this._asm_op_codes["not"]:{
        this.registeruint32[ra] = ~this.registeruint32[rb];
        break;
      }case this._asm_op_codes["shr"]:{
        this.registeruint32[ra] = this.registeruint32[ra] >>> this.registeruint32[rb];
        break;
      }case this._asm_op_codes["shl"]:{
        this.registeruint32[ra] = this.registeruint32[ra] << this.registeruint32[rb];
        break;
      }default:{
        alert("Illegial op code: " + format_binary_string(get_binary_string(current_inst, 0x100000000)));
      }
    }
  }
}

opCPU.prototype.set_options = function (options) {
  for(var key in options){
    if(key == "breakpoint_active"){
      this.breakpoint_active = options["breakpoint_active"];
    }else if(key == "watch_active"){
      this.watch_active = options["watch_active"];
    }else if(key == "breakpoint_address"){
      this.breakpoint_address = options["breakpoint_address"];
    }else{
      alert("Unknown option.");
    }
  }
}

opCPU.prototype.get_memoryuint32 = function () {
  return this.memoryuint32;
}
opCPU.prototype.get_registeruint32 = function () {
  return this.registeruint32;
}
opCPU.prototype.is_watch_modified = function () {
  return this.watch_modified;
}
opCPU.prototype.unmodify_watch = function () {
  return this.watch_modified = 0;
}
opCPU.prototype.untrip_breakpoint = function () {
  return this.breakpoint_tripped = 0;
}
opCPU.prototype.is_breakpoint_tripped = function () {
  return this.breakpoint_tripped;
}
opCPU.prototype.is_halted = function () {
  return this.registeruint32[this.FR_index] & this.HALTED_BIT;
}
opCPU.prototype.putc = function (c) {
  if(this.registeruint32[this.FR_index] & this.UART1_IN_READY_BIT){
    return 1; /* Item was NOT input */
  }
  //  Set the flag bit to indicate there is data
  this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] | this.UART1_IN_READY_BIT;
  //  Assert the interrupt
  this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] | this.UART1_IN_ASSERTED_BIT;
  this.memoryuint32[this.UART1_IN / this.sizeof_int] = c;
  return 0; /* Item was input */
}
opCPU.prototype.getc = function () {
  if(!(this.registeruint32[this.FR_index] & this.UART1_OUT_READY_BIT)){
    //  Set the flag bit back to ready.
    this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] | this.UART1_OUT_READY_BIT;
    //  Assert the interrupt
    this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] | this.UART1_OUT_ASSERTED_BIT;
    return this.memoryuint32[this.UART1_OUT / this.sizeof_int];
  }else{
    return null;
  }
}
opCPU.prototype.step = function () {
  if(this.registeruint32[this.FR_index] & this.HALTED_BIT){
    /*  Processor has been halted */
    return; 
  }

  /*  There should never be a page fault exception when attempting to return from an interrupt */
  this.assert(!((this.registeruint32[this.FR_index] & this.RTE_BIT) && (this.registeruint32[this.FR_index] & this.PAGE_FAULT_EXCEPTION_ASSERTED_BIT)), "RTE and page fault exception are both set.");

  if(this.breakpoint_active && (this.breakpoint_address == this.registeruint32[this.PC_index])){
    this.breakpoint_tripped = 1;
    return; 
  }

  if(this.watch_active && last_watch_value != (this.memoryuint32[this.watch_address/this.sizeof_int]>>>0)){
    this.watch_modified = 1;
    return; 
  }

  this.last_watch_value = this.memoryuint32[this.watch_address/this.sizeof_int]>>>0;

  if(this.registeruint32[this.FR_index] & this.RTE_BIT){
    var linear = [];
    if(this.translate_virtual_address_to_linear(this.registeruint32[this.SP_index], this.LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT, this.registeruint32[this.PC_index], linear)){
      assert(0, "Can't page fault here.");
    }else{
      this.registeruint32[this.PC_index] = this.memoryuint32[linear[0] / this.sizeof_int]; /* Set PC to [SP] */
      this.registeruint32[this.FR_index] = (this.registeruint32[this.FR_index] & ~this.RTE_BIT) >>> 0; /* Unset RET bit. */
      this.registeruint32[this.FR_index] = (this.registeruint32[this.FR_index] | this.GLOBAL_INTERRUPT_ENABLE_BIT) >>> 0; /* Set global interrupt enable. */
      this.registeruint32[this.SP_index] = this.registeruint32[this.SP_index] + this.registeruint32[this.WR_index];  /* Set SP to SP + WR */
    }
    return; 
  }

  if(this.registeruint32[this.FR_index] & this.GLOBAL_INTERRUPT_ENABLE_BIT){
    if(this.registeruint32[this.FR_index] & this.DIV_ZERO_ASSERTED_BIT){
      this.do_interrupt();
      return; 
    }else if((this.registeruint32[this.FR_index] & this.TIMER1_ENABLE_BIT) && (this.registeruint32[this.FR_index] & this.TIMER1_ASSERTED_BIT)){
      this.do_interrupt();
      return; 
    }else if((this.registeruint32[this.FR_index] & this.UART1_OUT_ENABLE_BIT) && (this.registeruint32[this.FR_index] & this.UART1_OUT_ASSERTED_BIT)){
      this.do_interrupt();
      return; 
    }else if((this.registeruint32[this.FR_index] & this.UART1_IN_ENABLE_BIT) && (this.registeruint32[this.FR_index] & this.UART1_IN_ASSERTED_BIT)){
      this.do_interrupt();
      return; 
    }
  }

  /*  Check for timer interrupt condition */
  if(this.cycles_executed % this.memoryuint32[this.TIMER_PERIOD / this.sizeof_int] == 0){
    /*  Assert our timer interrupt */
    this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] | this.TIMER1_ASSERTED_BIT;
    this.cycles_executed = 0; /* Avoid eventual overflow */
  }

  this.fetch_decode_execute();
  this.cycles_executed++;
}

opCPU.prototype.do_interrupt = function(){
  var linear = [];
  var virtual_addr = this.registeruint32[this.SP_index] - this.registeruint32[this.WR_index];
  if(this.translate_virtual_address_to_linear(virtual_addr, this.LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT, this.registeruint32[this.PC_index], linear)){
    assert(0, "Cannot page fault here.");
  }else{
    this.memoryuint32[linear[0] / this.sizeof_int] = this.registeruint32[this.PC_index]; /* [SP - 4] = PC */
    this.registeruint32[this.SP_index] = virtual_addr; /* SP = SP -4 */
    /*  Disable global interrupts */
    this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] & ~this.GLOBAL_INTERRUPT_ENABLE_BIT;
    /*  Branch to irq handler */
    this.registeruint32[this.PC_index] = this.memoryuint32[this.IRQ_HANDLER / this.sizeof_int];
  }
}
