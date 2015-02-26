/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
var sizeof_int = 0x4;
var literal22bitmask = 0x003FFFFF;
var num_registers = 64;
var num_special_registers = 6;

var PC_index = 0;
var SP_index = 1;
var FP_index = 2;
var ZR_index = 3;
var FR_index = 4;
var WR_index = 5;

var HALTED_BIT = 1 << 0;
var GLOBAL_INTERRUPT_ENABLE_BIT = 1 << 1;
var RTE_BIT = 1 << 2;
var TIMER1_ENABLE_BIT = 1 << 3;
var TIMER1_ASSERTED_BIT = 1 << 4;
var UART1_OUT_ENABLE_BIT = 1 << 5;
var UART1_OUT_ASSERTED_BIT = 1 << 6;
var UART1_IN_ENABLE_BIT = 1 << 7;
var UART1_IN_ASSERTED_BIT = 1 << 8;
var UART1_OUT_READY_BIT = 1 << 9;
var UART1_IN_READY_BIT = 1 << 10;

var _asm_op_codes = {
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
}

var _asm_templates = {
  "add": ["register", "register", "register"],
  "sub": ["register", "register", "register"],
  "mul": ["register", "register", "register"],
  "div": ["register", "register", "register", "register"],
  "beq": ["register", "register", "decimal-number"],
  "blt": ["register", "register", "decimal-number"],
  "loa": ["register", "register"],
  "sto": ["register", "register"],
  "ll": ["register", "hexadecimal-number"],
  "and": ["register", "register", "register"],
  "or": ["register", "register", "register"],
  "not": ["register", "register"],
  "shr": ["register", "register"],
  "shl": ["register", "register"],
  "dw": ["hexadecimal-number"]
}

function assert(expression, message){
  if(!expression){
    alert(message);
  }
}

function get_register_name(n){
  var exceptions = ["PC","SP","FP","ZR","FR","WR"];
  return n > (num_special_registers - 1) ? ("r" + (n - (num_special_registers -1))) : exceptions[n];
}

function get_register_number(name){
  var exceptions = { "PC": 0, "SP": 1,"FP": 2,"ZR": 3, "FR": 4, "WR": 5}
  if(name in exceptions){
    return exceptions[name];
  }else{
    var a = name.split("r")[1]
    return parseInt(a,10) + (num_special_registers - 1);
  }
}

function parse_part(part){
  switch (true){
    case /^PC$/.test(part): {
      return {type: "register", value: get_register_number(part)};
    }case /^SP$/.test(part): {
      return {type: "register", value: get_register_number(part)};
    }case /^FP$/.test(part): {
      return {type: "register", value: get_register_number(part)};
    }case /^ZR$/.test(part): {
      return {type: "register", value: get_register_number(part)};
    }case /^FR$/.test(part): {
      return {type: "register", value: get_register_number(part)};
    }case /^WR$/.test(part): {
      return {type: "register", value: get_register_number(part)};
    }case /^r[0-9]+$/.test(part): {
      return {type: "register", value: get_register_number(part)};
    }case /^-*[0-9]+$/.test(part): {
      return {type: "decimal-number", value: part};
    }case /^0x[0-9A-Fa-f]+$/.test(part): {
      return {type: "hexadecimal-number", value: eval(part)};
    }case /^add$/.test(part): {
      return {type: "add"};
    }case /^sub$/.test(part): {
      return {type: "sub"};
    }case /^mul$/.test(part): {
      return {type: "mul"};
    }case /^div$/.test(part): {
      return {type: "div"};
    }case /^beq$/.test(part): {
      return {type: "beq"};
    }case /^blt$/.test(part): {
      return {type: "blt"};
    }case /^ll$/.test(part): {
      return {type: "ll"};
    }case /^dw$/.test(part): {
      return {type: "dw"};
    }case /^loa$/.test(part): {
      return {type: "loa"};
    }case /^sto$/.test(part): {
      return {type: "sto"};
    }case /^and$/.test(part): {
      return {type: "and"};
    }case /^or$/.test(part): {
      return {type: "or"};
    }case /^not$/.test(part): {
      return {type: "not"};
    }case /^shr$/.test(part): {
      return {type: "shr"};
    }case /^shl$/.test(part): {
      return {type: "shl"};
    }default: {
      alert("Don't know what to do with part " + part);
    }
  }
}

function parse_parts(parts){
  for(var i = 0; i < parts.length; i++){
    parts[i] = parse_part(parts[i]);
  }
  return parts;
}

function format_binary_string(str){
  rtn = "";
  rtn += "Ctrl: " + str.substring(0, 4) + " ";
  rtn += "r" + str.substring(4, 10) + " ";
  rtn += "r" + str.substring(10, 16) + " ";
  rtn += "r" + str.substring(16, 22) + " ";
  rtn += "r" + str.substring(22, 28) + " ";
  rtn += "left: " + str.substring(28, 32) + " ";
  return rtn;
}

function binary_disassemble_instruction(instruction){
  var branch_binary = instruction & 0x0000FFFF;
  var operation = (0xF0000000 & instruction)>>>0;
  var literal22bit = (literal22bitmask & instruction)>>>0;
  var literal5bit = (0x0000001F & instruction)>>>0;
  var ra = (0x0FC00000 & instruction) / 0x400000;
  var rb = (0x003F0000 & instruction) / 0x10000;
  var rc = (0x0000FC00 & instruction) / 0x400;
  var rd = (0x000003F0 & instruction) / 0x10;
  switch(operation){
    case _asm_op_codes["add"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40);
      break;
    }case _asm_op_codes["sub"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40);
      break;
    }case _asm_op_codes["mul"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40);
      break;
    }case _asm_op_codes["div"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40) +  " r" + get_binary_string(rd, 0x40);
      break;
    }case _asm_op_codes["beq"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " i" + get_binary_string(branch_binary, 0x10000);
      break;
    }case _asm_op_codes["blt"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " i" + get_binary_string(branch_binary, 0x10000);
      break;
    }case _asm_op_codes["loa"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40);
      break;
    }case _asm_op_codes["sto"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40);
      break;
    }case _asm_op_codes["ll"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " l" + get_binary_string(literal22bit, 0x400000);
      break;
    }case _asm_op_codes["and"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40);
      break;
    }case _asm_op_codes["or"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40);
      break;
    }case _asm_op_codes["not"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40);
      break;
    }case _asm_op_codes["shr"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40);
      break;
    }case _asm_op_codes["shl"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40);
      break;
    }default:{
      return "op-code:" + get_binary_string(operation >> 28, 0x10) + "(Unrecognized op code.)";
    }
  }
}

function get_binary_string(int32bit, base){
  str = "";
  do{
    base /= 2;
    str += (int32bit & base) ? "1" : "0";
  }while (base != 1);
  return str;
}

function virtual_machine() {
  var MAIN_MEMORY_SIZE = 1024 * 1024 * 5;
  var MAIN_MEMORY = new ArrayBuffer(MAIN_MEMORY_SIZE);
  var REGISTERS = new ArrayBuffer(num_registers * sizeof_int);
  var registeruint32 = new Uint32Array(REGISTERS);
  var memoryuint32 = new Uint32Array(MAIN_MEMORY);
  var memory_size_ints = memoryuint32.length;
  var memorysint16 = new Int16Array(MAIN_MEMORY);
  var enable_stack_zeroing = 0; /* Must not be enabled when running kernel */

  var UART1_OUT = 0x300000;
  var UART1_IN = 0x300010;
  var IRQ_HANDLER = 0x300020;
  var TIMER_PERIOD = 0x300030;

  var cycles_executed = 0;
  var watch_active = 0;
  var watch_address = 0x1ffff4;
  var watch_modified = 0;
  var last_watch_value = 0;
  var breakpoint_address = 0;
  var breakpoint_active = 0;
  var breakpoint_tripped = 0;

  //  Set the program counter to the instruction at 0.
  registeruint32[PC_index] = 0;
  registeruint32[FR_index] = registeruint32[FR_index] | UART1_OUT_READY_BIT;
  registeruint32[WR_index] = sizeof_int; /*  Set the word register to 4 */

  function assert_memory_index_in_range(index, thing){
    //  Halt the processor if we get to this condition.
    if(!(index < memory_size_ints)){
      registeruint32[FR_index] = registeruint32[FR_index] | HALTED_BIT;
    }
    assert(
      index < memory_size_ints,
      thing + " was out of bounds at index " + index + " but memory only contains " + memory_size_ints + " ints."
    );
  }

  function fetch_decode_execute() {
    assert_memory_index_in_range(registeruint32[PC_index] / sizeof_int, "PC");
    var current_inst = memoryuint32[registeruint32[PC_index] / sizeof_int];
    //  TODO this is little-endian dependent
    var branch_dist = memorysint16[(registeruint32[PC_index] / 2)];
    registeruint32[PC_index] += sizeof_int;
    var operation = (0xF0000000 & current_inst)>>>0;
    var literal22bit = (literal22bitmask & current_inst)>>>0;
    var literal5bit = (0x0000001F & current_inst)>>>0;
    var ra = (0x0FC00000 & current_inst) / 0x400000;
    var rb = (0x003F0000 & current_inst) / 0x10000;
    var rc = (0x0000FC00 & current_inst) / 0x400;
    var rd = (0x000003F0 & current_inst) / 0x10;

    function perform_stack_zeroing(ra, newvalue, oldvalue){
      /*  In order for premption to work, it is critical that any important information is always located on or under
          the current value of the stack pointer.  This function helps in testing this by explicitly zeroing any poped
          stack items.  This should cause missbehaving code to fail even without premption, and cause obvious failures
          that can be debugged.
      */
      if(!enable_stack_zeroing){
        return;
      }
      if(ra == 1){ /*  If it's the SP register */
        if(newvalue > oldvalue){  /*  If this operation pops the stack */
          var words_poped = (newvalue - oldvalue) / sizeof_int;
          for(var i = 0; i < words_poped; i++){
            memoryuint32[(oldvalue - (i * sizeof_int))/sizeof_int] = 0; /* Zero this poped item */
          }
        }
      }
    }
    
    switch(operation){
      case _asm_op_codes["add"]:{
        var sp_value_before = registeruint32[SP_index];
        registeruint32[ra] = registeruint32[rb] + registeruint32[rc];
        perform_stack_zeroing(ra, registeruint32[ra], sp_value_before);
        break;
      }case _asm_op_codes["sub"]:{
        var sp_value_before = registeruint32[SP_index];
        registeruint32[ra] = registeruint32[rb] - registeruint32[rc];
        perform_stack_zeroing(ra, registeruint32[ra], sp_value_before);
        break;
      }case _asm_op_codes["mul"]:{
        /*  Unfortunately, this must be done in parts to avoid intermediate floating point rounding errors
            that will cause inaccurate results for overflowing unsigned multiplication operations eg.
            0xFFFFFFFF * 0xFFFFFFFF will = 0 (but it should be 1)
        */
        var mask_low = 0xFFFF;
        var mask_high = 0xFFFF0000;
        var low_b = (mask_low >>> 0) & registeruint32[rb];
        var low_c = (mask_low >>> 0) & registeruint32[rc];
        var high_b = ((mask_high >>> 0) & registeruint32[rb]) >>> 16;
        var high_c = ((mask_high >>> 0) & registeruint32[rc]) >>> 16;
        var low_sum = (low_b * low_c);
        var mid_sum = (high_b * low_c) + (low_b * high_c);
        /*var high_sum = high_b * high_c; The contribution of this factor would produce bits higher than the 32nd bit */
        var low_result = (low_sum >>> 0) & (mask_low >>> 0);
        var high_result_low = (((low_sum >>> 0) & (mask_high >>> 0)) >>> 16);
        var high_result_mid = (((mid_sum >>> 0) & (mask_low >>> 0)) >>> 0);
        var high_result = (((((high_result_low + high_result_mid) >>> 0) & (mask_low >>> 0)) << 16) >>> 0);
        registeruint32[ra] = (low_result >>> 0) | (high_result >>> 0);
        break;
      }case _asm_op_codes["div"]:{
        var c = registeruint32[rc];
        var d = registeruint32[rd];
        registeruint32[ra] = c / d;
        registeruint32[rb] = c % d;
        break;
      }case _asm_op_codes["beq"]:{
        if(registeruint32[ra] == registeruint32[rb]){
          registeruint32[PC_index] += sizeof_int * branch_dist;
        }
        break;
      }case _asm_op_codes["blt"]:{
        if(registeruint32[ra] < registeruint32[rb]){
          registeruint32[PC_index] += sizeof_int * branch_dist;
        }
        break;
      }case _asm_op_codes["loa"]:{
        var sp_value_before = registeruint32[SP_index];
        assert_memory_index_in_range(registeruint32[rb]/sizeof_int, "loa instruction");
        registeruint32[ra] = memoryuint32[registeruint32[rb]/sizeof_int];
        perform_stack_zeroing(ra, registeruint32[ra], sp_value_before);
        break;
      }case _asm_op_codes["sto"]:{
        assert_memory_index_in_range(registeruint32[ra]/sizeof_int, "loa instruction");
        memoryuint32[registeruint32[ra]/sizeof_int] = registeruint32[rb];
        break;
      }case _asm_op_codes["ll"]:{
        registeruint32[ra] = literal22bit;
        break;
      }case _asm_op_codes["and"]:{
        registeruint32[ra] = registeruint32[rb] & registeruint32[rc];
        break;
      }case _asm_op_codes["or"]:{
        registeruint32[ra] = registeruint32[rb] | registeruint32[rc];
        break;
      }case _asm_op_codes["not"]:{
        registeruint32[ra] = ~registeruint32[rb];
        break;
      }case _asm_op_codes["shr"]:{
        registeruint32[ra] = registeruint32[ra] >>> registeruint32[rb];
        break;
      }case _asm_op_codes["shl"]:{
        registeruint32[ra] = registeruint32[ra] << registeruint32[rb];
        break;
      }default:{
        alert("Illegial op code: " + format_binary_string(get_binary_string(current_inst, 0x100000000)));
      }
    }
  }

  return {
    set_options: function (options) {
      for(var key in options){
        if(key == "breakpoint_active"){
          breakpoint_active = options["breakpoint_active"];
        }else if(key == "watch_active"){
          watch_active = options["watch_active"];
        }else if(key == "breakpoint_address"){
          breakpoint_address = options["breakpoint_address"];
        }else{
          alert("Unknown option.");
        }
      }
    },
    get_memoryuint32: function () {
      return memoryuint32;
    },
    get_registeruint32: function () {
      return registeruint32;
    },
    get_memorysint16: function () {
      return memorysint16;
    },
    watch_modified: function () {
      return watch_modified;
    },
    unmodify_watch: function () {
      return watch_modified = 0;
    },
    untrip_breakpoint: function () {
      return breakpoint_tripped = 0;
    },
    breakpoint_tripped: function () {
      return breakpoint_tripped;
    },
    is_halted: function () {
      return registeruint32[FR_index] & HALTED_BIT;
    },
    putc: function (c) {
      if(registeruint32[FR_index] & UART1_IN_READY_BIT){
        return 1; /* Item was NOT input */
      }
      //  Set the flag bit to indicate there is data
      registeruint32[FR_index] = registeruint32[FR_index] | UART1_IN_READY_BIT;
      //  Assert the interrupt
      registeruint32[FR_index] = registeruint32[FR_index] | UART1_IN_ASSERTED_BIT;
      memoryuint32[UART1_IN / sizeof_int] = c;
      return 0; /* Item was input */
    },
    getc: function () {
      if(!(registeruint32[FR_index] & UART1_OUT_READY_BIT)){
        //  Set the flag bit back to ready.
        registeruint32[FR_index] = registeruint32[FR_index] | UART1_OUT_READY_BIT;
        //  Assert the interrupt
        registeruint32[FR_index] = registeruint32[FR_index] | UART1_OUT_ASSERTED_BIT;
        return memoryuint32[UART1_OUT / sizeof_int];
      }else{
        return null;
      }
    },
    step: function () {
      if(registeruint32[FR_index] & HALTED_BIT){
        /*  Processor has been halted */
        return; 
      }

      if(breakpoint_active && (breakpoint_address == registeruint32[PC_index])){
        breakpoint_tripped = 1;
        return; 
      }

      if(watch_active && last_watch_value != (memoryuint32[watch_address/sizeof_int]>>>0)){
        watch_modified = 1;
        return; 
      }

      last_watch_value = memoryuint32[watch_address/sizeof_int]>>>0;

      fetch_decode_execute();
      cycles_executed++;

      if(registeruint32[FR_index] & HALTED_BIT){
        /*  Processor has been halted */
        return; 
      }

      if(registeruint32[FR_index] & RTE_BIT){
        registeruint32[FR_index] = (registeruint32[FR_index] & ~RTE_BIT) >>> 0; /* Unset RET bit. */
        registeruint32[FR_index] = (registeruint32[FR_index] | GLOBAL_INTERRUPT_ENABLE_BIT) >>> 0; /* Set global interrupt enable. */
        registeruint32[PC_index] = memoryuint32[registeruint32[SP_index] / sizeof_int]; /* Set PC to [SP] */
        registeruint32[SP_index] = registeruint32[SP_index] + registeruint32[WR_index];  /* Set SP to SP + WR */
        return; 
      }

      /*  Check for timer interrupt condition */
      if(cycles_executed % memoryuint32[TIMER_PERIOD / sizeof_int] == 0){
        /*  Assert our timer interrupt */
        registeruint32[FR_index] = registeruint32[FR_index] | TIMER1_ASSERTED_BIT;
        cycles_executed = 0; /* Avoid eventual overflow */
        return; 
      }

      function do_interrupt(){
        /*  Disable global interrupts */
        registeruint32[FR_index] = registeruint32[FR_index] & ~GLOBAL_INTERRUPT_ENABLE_BIT;
        /*  Branch to irq handler */
        registeruint32[SP_index] = registeruint32[SP_index] - registeruint32[WR_index]; /* SP = SP -4 */
        memoryuint32[registeruint32[SP_index] / sizeof_int] = registeruint32[PC_index]; /* [SP] = PC */
        registeruint32[PC_index] = memoryuint32[IRQ_HANDLER / sizeof_int];
      }

      if(registeruint32[FR_index] & GLOBAL_INTERRUPT_ENABLE_BIT){
        if(registeruint32[FR_index] & TIMER1_ENABLE_BIT && (registeruint32[FR_index] & TIMER1_ASSERTED_BIT)){
          do_interrupt();
          return; 
        }else if(registeruint32[FR_index] & UART1_OUT_ENABLE_BIT && (registeruint32[FR_index] & UART1_OUT_ASSERTED_BIT)){
          do_interrupt();
          return; 
        }else if(registeruint32[FR_index] & UART1_IN_ENABLE_BIT && (registeruint32[FR_index] & UART1_IN_ASSERTED_BIT)){
          do_interrupt();
          return; 
        }
      }
    },

    assemble_l1_asm_to_memory: function (str){
      if(str.length > 0){
        if(str.charCodeAt(str.length -1) == 0xA){
          str = str.substr(0, str.length -1);
        }
      }
      var lines = str.split("\n");
      var first_line = lines.shift();
      var address_dec = parseInt(first_line.split(" ")[1]);
      var machine_instructions = new Uint32Array(MAIN_MEMORY);
      for(var i = 0; i < lines.length; i++){
        var instruction_index = i + address_dec;
        var a = parse_parts(lines[i].split(" "));
        machine_instructions[instruction_index] = 0;
  
        if(!(a.length - 1 == _asm_templates[a[0].type].length))
          alert("Invalid number of arguments on line " + i);
  
        var operation = a.shift();
        for(var j = 0; j < _asm_templates[operation.type].length; j++){
          if(!(_asm_templates[operation.type][j] == a[j].type))
            alert("Non-matching argument type line " + i)
  
          if(a[j].type == "register"){
            if(a[j].value < 0 || a[j].value > 63)
              alert("Register value out of range (must be 0 to 63 inclusive) line " + i);
  
            machine_instructions[instruction_index] += (0x400000 / Math.pow(0x40,j)) * a[j].value;
          }else if(a[j].type == "decimal-number"){
            if(a[j].value < -Math.pow(2,15) || a[j].value > (Math.pow(2,15) -1))
              alert("Decimal number out of range (must be " + (-Math.pow(2,15)) + " to " + Math.pow(2,15) + " inclusive) line " + i)
            var tmpbuffer = new ArrayBuffer(sizeof_int);
            var tmpuint32View = new Uint32Array(tmpbuffer,0,1);
            var tmpsint16View = new Int16Array(tmpbuffer,0,2);
            //  TODO this is endian dependent now, currently assuming little-endian
            tmpsint16View[0] = a[j].value;
            tmpsint16View[1] = 0;
            machine_instructions[instruction_index] += tmpuint32View[0];
          }else if(a[j].type == "hexadecimal-number"){
            if(operation.type == "ll"){
              if(a[j].value < 0 || literal22bitmask < a[j].value){
                alert("ll value " + a[j].value + " out of range (must be 0 to " + literal22bitmask + " inclusive).");
              }
              machine_instructions[instruction_index] += a[j].value;
            }else if(operation.type == "dw"){
              machine_instructions[instruction_index] += a[j].value;
            }else{
              alert("Unknown state.");
            }
          }else{
            alert(a[j].type + " not matched on line " + i);
          }
        }
        
        if(operation.type != "dw")
          machine_instructions[instruction_index] += _asm_op_codes[operation.type];
      }
    }
  };
}
