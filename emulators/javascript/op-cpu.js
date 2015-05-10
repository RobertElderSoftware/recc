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
"use strict";

function opCPU(l0_description) {
  this._asm_op_codes = {
    "add": 0x00000000,
    "sub": 0x10000000,
    "mul": 0x20000000,
    "div": 0x30000000,
    "beq": 0x40000000,
    "blt": 0x50000000,
    "loa": 0x60000000,
    "sto": 0x70000000,
    "ll":  0x80000000,
    "and": 0x90000000,
    "or":  0xA0000000,
    "not": 0xB0000000,
    "shr": 0xC0000000,
    "shl": 0xD0000000
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

  this.HALTED_BIT                  = 1 << 0;
  this.GLOBAL_INTERRUPT_ENABLE_BIT = 1 << 1;
  this.RTE_BIT                     = 1 << 2;
  this.TIMER1_ENABLE_BIT           = 1 << 3;
  this.TIMER1_ASSERTED_BIT         = 1 << 4;
  this.UART1_OUT_ENABLE_BIT        = 1 << 5;
  this.UART1_OUT_ASSERTED_BIT      = 1 << 6;
  this.UART1_IN_ENABLE_BIT         = 1 << 7;
  this.UART1_IN_ASSERTED_BIT       = 1 << 8;
  this.UART1_OUT_READY_BIT         = 1 << 9;
  this.UART1_IN_READY_BIT          = 1 << 10;

  this.PC_index = 0;
  this.SP_index = 1;
  this.FP_index = 2;
  this.ZR_index = 3;
  this.FR_index = 4;
  this.WR_index = 5;
  
  this.num_registers = 64;
  this.literal22bitmask = 0x003FFFFF;
  this.sizeof_int = 0x4;

  this.MAIN_MEMORY_SIZE = l0_description.data_end;
  this.MAIN_MEMORY = new ArrayBuffer(this.MAIN_MEMORY_SIZE);
  this.REGISTERS = new ArrayBuffer(this.num_registers * this.sizeof_int);
  this.registeruint32 = new Uint32Array(this.REGISTERS);
  this.memoryuint32 = new Uint32Array(this.MAIN_MEMORY);
  this.memory_size_ints = this.memoryuint32.length;
  this.memorysint16 = new Int16Array(this.MAIN_MEMORY);
  this.enable_stack_zeroing = 0; /* Must not be enabled when running kernel */

  this.UART1_OUT    = 0x300000;
  this.UART1_IN     = 0x300010;
  this.IRQ_HANDLER  = 0x300020;
  this.TIMER_PERIOD = 0x300030;

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
  var instruction_index = l0_description.data_start / this.sizeof_int;
  /*  Load all machine instructions */
  for(var i = 0; i < l0_description.data.length; i++){
    if(this.MAIN_MEMORY_SIZE / this.sizeof_int < instruction_index){
      alert("Not enough memory to assemble.");
    }
    if(l0_description.data[i][0] == 0){ /*  Regular instruction */
      machine_instructions[instruction_index] = l0_description.data[i][1];
      instruction_index++;
    }else{ /*  SW directive to skip n words */
      for(var j = 0; j < l0_description.data[i][1]; j++){
        machine_instructions[instruction_index] = 0;
        instruction_index++;
      }
    }
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

  this.assert_memory_index_in_range(this.registeruint32[this.PC_index] / this.sizeof_int, "PC");
  var current_inst = this.memoryuint32[this.registeruint32[this.PC_index] / this.sizeof_int];
  //  TODO this is little-endian dependent
  var branch_dist = this.memorysint16[(this.registeruint32[this.PC_index] / 2)];
  this.registeruint32[this.PC_index] += this.sizeof_int;
  var operation = (0xF0000000 & current_inst)>>>0;
  var literal22bit = (this.literal22bitmask & current_inst)>>>0;
  var literal5bit = (0x0000001F & current_inst)>>>0;
  var ra = (0x0FC00000 & current_inst) / 0x400000;
  var rb = (0x003F0000 & current_inst) / 0x10000;
  var rc = (0x0000FC00 & current_inst) / 0x400;
  var rd = (0x000003F0 & current_inst) / 0x10;

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
      var c = this.registeruint32[rc];
      var d = this.registeruint32[rd];
      this.registeruint32[ra] = c / d;
      this.registeruint32[rb] = c % d;
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
      var sp_value_before = this.registeruint32[this.SP_index];
      this.assert_memory_index_in_range(this.registeruint32[rb]/this.sizeof_int, "loa instruction");
      this.registeruint32[ra] = this.memoryuint32[this.registeruint32[rb]/this.sizeof_int];
      this.perform_stack_zeroing(ra, this.registeruint32[ra], sp_value_before);
      break;
    }case this._asm_op_codes["sto"]:{
      this.assert_memory_index_in_range(this.registeruint32[ra]/this.sizeof_int, "loa instruction");
      this.memoryuint32[this.registeruint32[ra]/this.sizeof_int] = this.registeruint32[rb];
      break;
    }case this._asm_op_codes["ll"]:{
      this.registeruint32[ra] = literal22bit;
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
opCPU.prototype.get_memorysint16 = function () {
  return this.memorysint16;
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

  if(this.breakpoint_active && (this.breakpoint_address == this.registeruint32[this.PC_index])){
    this.breakpoint_tripped = 1;
    return; 
  }

  if(this.watch_active && last_watch_value != (this.memoryuint32[this.watch_address/this.sizeof_int]>>>0)){
    this.watch_modified = 1;
    return; 
  }

  this.last_watch_value = this.memoryuint32[this.watch_address/this.sizeof_int]>>>0;

  this.fetch_decode_execute();
  this.cycles_executed++;

  if(this.registeruint32[this.FR_index] & this.HALTED_BIT){
    /*  Processor has been halted */
    return; 
  }

  if(this.registeruint32[this.FR_index] & this.RTE_BIT){
    this.registeruint32[this.FR_index] = (this.registeruint32[this.FR_index] & ~this.RTE_BIT) >>> 0; /* Unset RET bit. */
    this.registeruint32[this.FR_index] = (this.registeruint32[this.FR_index] | this.GLOBAL_INTERRUPT_ENABLE_BIT) >>> 0; /* Set global interrupt enable. */
    this.registeruint32[this.PC_index] = this.memoryuint32[this.registeruint32[this.SP_index] / this.sizeof_int]; /* Set PC to [SP] */
    this.registeruint32[this.SP_index] = this.registeruint32[this.SP_index] + this.registeruint32[this.WR_index];  /* Set SP to SP + WR */
    return; 
  }

  /*  Check for timer interrupt condition */
  if(this.cycles_executed % this.memoryuint32[this.TIMER_PERIOD / this.sizeof_int] == 0){
    /*  Assert our timer interrupt */
    this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] | this.TIMER1_ASSERTED_BIT;
    this.cycles_executed = 0; /* Avoid eventual overflow */
    return; 
  }

  if(this.registeruint32[this.FR_index] & this.GLOBAL_INTERRUPT_ENABLE_BIT){
    if(this.registeruint32[this.FR_index] & this.TIMER1_ENABLE_BIT && (this.registeruint32[this.FR_index] & this.TIMER1_ASSERTED_BIT)){
      this.do_interrupt();
      return; 
    }else if(this.registeruint32[this.FR_index] & this.UART1_OUT_ENABLE_BIT && (this.registeruint32[this.FR_index] & this.UART1_OUT_ASSERTED_BIT)){
      this.do_interrupt();
      return; 
    }else if(this.registeruint32[this.FR_index] & this.UART1_IN_ENABLE_BIT && (this.registeruint32[this.FR_index] & this.UART1_IN_ASSERTED_BIT)){
      this.do_interrupt();
      return; 
    }
  }
}

opCPU.prototype.do_interrupt = function(){
  /*  Disable global interrupts */
  this.registeruint32[this.FR_index] = this.registeruint32[this.FR_index] & ~this.GLOBAL_INTERRUPT_ENABLE_BIT;
  /*  Branch to irq handler */
  this.registeruint32[this.SP_index] = this.registeruint32[this.SP_index] - this.registeruint32[this.WR_index]; /* SP = SP -4 */
  this.memoryuint32[this.registeruint32[this.SP_index] / this.sizeof_int] = this.registeruint32[this.PC_index]; /* [SP] = PC */
  this.registeruint32[this.PC_index] = this.memoryuint32[this.IRQ_HANDLER / this.sizeof_int];
}
