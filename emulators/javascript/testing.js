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
var running_kernel = window.top.location.href.indexOf("kernel_mode") > -1 ? "kernel" : null;
var specific_tests = running_kernel ? "kernel" : null;
var input_queue = [];
var memory_size_ints;
var is_debug_mode;
var num_special_registers = 6;

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

function get_binary_string(int32bit, base){
  var str = "";
  do{
    base /= 2;
    str += (int32bit & base) ? "1" : "0";
  }while (base != 1);
  return str;
}

function binary_disassemble_instruction(instruction, vm){
  var branch_binary = instruction & 0x0000FFFF;
  var operation = (0xF0000000 & instruction)>>>0;
  var literal22bit = (vm.literal22bitmask & instruction)>>>0;
  var literal5bit = (0x0000001F & instruction)>>>0;
  var ra = (0x0FC00000 & instruction) / 0x400000;
  var rb = (0x003F0000 & instruction) / 0x10000;
  var rc = (0x0000FC00 & instruction) / 0x400;
  var rd = (0x000003F0 & instruction) / 0x10;
  switch(operation){
    case vm._asm_op_codes["add"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40);
      break;
    }case vm._asm_op_codes["sub"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40);
      break;
    }case vm._asm_op_codes["mul"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40);
      break;
    }case vm._asm_op_codes["div"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40) +  " r" + get_binary_string(rd, 0x40);
      break;
    }case vm._asm_op_codes["beq"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " i" + get_binary_string(branch_binary, 0x10000);
      break;
    }case vm._asm_op_codes["blt"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " i" + get_binary_string(branch_binary, 0x10000);
      break;
    }case vm._asm_op_codes["loa"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40);
      break;
    }case vm._asm_op_codes["sto"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40);
      break;
    }case vm._asm_op_codes["ll"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " l" + get_binary_string(literal22bit, 0x400000);
      break;
    }case vm._asm_op_codes["and"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40);
      break;
    }case vm._asm_op_codes["or"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40) + " r" + get_binary_string(rc, 0x40);
      break;
    }case vm._asm_op_codes["not"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40);
      break;
    }case vm._asm_op_codes["shr"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40);
      break;
    }case vm._asm_op_codes["shl"]:{
      return "op" + get_binary_string(operation >> 28, 0x10) + " r" + get_binary_string(ra, 0x40) + " r" + get_binary_string(rb, 0x40);
      break;
    }default:{
      return "op-code:" + get_binary_string(operation >> 28, 0x10) + "(Unrecognized op code.)";
    }
  }
}

function clear_state(){
  $(".debug-view-area").hide();
  $(".fr-bits td").each(function (){
    $(this).removeClass('active');
  });
  $(".register-container").html('');
  $(".location-container").html('');
  $(".instruction-container").html('');
  $(".binary-container").html('');
  $(".value-container").html('');
  $(".stack-address-container").html('');
  $(".stack-value-container").html('');
}


function print_state(vm){
  var memoryuint32 = vm.get_memoryuint32();
  var memorysint16 = vm.get_memorysint16();
  var registeruint32 = vm.get_registeruint32();
  function disassemble_instruction(index){
    var current_inst = memoryuint32[index];
    //  TODO this is little-endian dependant
    var branch_dist = memorysint16[index*2];
    var operation = (0xF0000000 & current_inst)>>>0;
    var r = new Array();
    r[0] = (0x0FC00000 & current_inst) / 0x400000;
    r[1] = (0x003F0000 & current_inst) / 0x10000;
    r[2] = (0x0000FC00 & current_inst) / 0x400;
    r[3] = (0x000003F0 & current_inst) / 0x10;

    var op_name = "dw";
    var parts = [];
    for(var key in vm._asm_op_codes){
      if(vm._asm_op_codes[key] == operation){
        op_name = key;
        break;
      }
    }

    for(var i = 0; i < vm._asm_templates[op_name].length; i++){
      if(vm._asm_templates[op_name][i] == "register"){
        parts.push(get_register_name(r[i]));
      }else if(vm._asm_templates[op_name][i] == "hexadecimal-number"){
        if(op_name == "ll"){
          parts.push("0x" + (vm.literal22bitmask & memoryuint32[index]).toString(16));
        }else{
          alert("Unknown state.");
        }
      }else if(vm._asm_templates[op_name][i] == "decimal-number"){
        parts.push(branch_dist);
      }
    }

    return "<span class='op-name'>" + op_name + "</span> " + parts.join(" ");
  }

  var current_inst = memoryuint32[registeruint32[0] / vm.sizeof_int];
  //  TODO this is little-endian dependant
  var branch_dist = memorysint16[(registeruint32[0] / (vm.sizeof_int / 2))];
  var operation = (0xF0000000 & current_inst)>>>0;
  var r = new Array();
  r[0] = (0x0FC00000 & current_inst) / 0x400000;
  r[1] = (0x003F0000 & current_inst) / 0x10000;
  r[2] = (0x0000FC00 & current_inst) / 0x400;
  r[3] = (0x000003F0 & current_inst) / 0x10;

  var op_name = "dw";
  for(var key in vm._asm_op_codes){
    if(vm._asm_op_codes[key] == operation){
      op_name = key;
      break;
    }
  }

  var hilighted_registers = []
  var hilighted_memory_locations = []

  var output_color = "#e77e43"
  var input_color = "#73c243"

  switch (op_name){
    case "add":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_registers.push({value: r[1], color: input_color});
      hilighted_registers.push({value: r[2], color: input_color});
      break;
    }case "sub":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_registers.push({value: r[1], color: input_color});
      hilighted_registers.push({value: r[2], color: input_color});
      break;
    }case "mul":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_registers.push({value: r[1], color: output_color});
      hilighted_registers.push({value: r[2], color: input_color});
      hilighted_registers.push({value: r[3], color: input_color});
      break;
    }case "div":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_registers.push({value: r[1], color: output_color});
      hilighted_registers.push({value: r[2], color: input_color});
      hilighted_registers.push({value: r[3], color: input_color});
    }case "beq":{
      hilighted_registers.push({value: r[0], color: input_color});
      hilighted_registers.push({value: r[1], color: input_color});
      hilighted_memory_locations.push({value: (registeruint32[0] + vm.sizeof_int * branch_dist), color: input_color});
      break;
    }case "blt":{
      hilighted_registers.push({value: r[0], color: input_color});
      hilighted_registers.push({value: r[1], color: input_color});
      hilighted_memory_locations.push({value: (registeruint32[0] + vm.sizeof_int * branch_dist), color: input_color});
      break;
    }case "loa":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_memory_locations.push({value: registeruint32[r[1]]/vm.sizeof_int, color: output_color});
      break;
    }case "sto":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_memory_locations.push({value: registeruint32[r[0]]/vm.sizeof_int, color: input_color});
      break;
    }case "ll":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_memory_locations.push({value: registeruint32[0]/vm.sizeof_int, color: input_color});
      break;
    }case "and":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_registers.push({value: r[1], color: input_color});
      hilighted_registers.push({value: r[2], color: input_color});
      break;
    }case "or":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_registers.push({value: r[1], color: input_color});
      hilighted_registers.push({value: r[2], color: input_color});
      break;
    }case "not":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_registers.push({value: r[1], color: input_color});
      break;
    }case "shr":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_registers.push({value: r[1], color: input_color});
      break;
    }case "shl":{
      hilighted_registers.push({value: r[0], color: output_color});
      hilighted_registers.push({value: r[1], color: input_color});
      break;
    }default: {
      alert("Did not find op named " + op_name);
    }
  }

  clear_state();

  hilighted_registers.sort(function(a,b){return a.value-b.value});
  hilighted_memory_locations.sort(function(a,b){return a.value-b.value});

  for(var i = 0; i < 20; i++){
    var style = "";
    for(var j = 0; j < hilighted_registers.length; j++){
      if(hilighted_registers[j].value == i){
        style = hilighted_registers[j].color
        break;
      }
    }
    var text = '<div style="color: ' + style + ';">' + get_register_name(i) + "). 0x" + registeruint32[i].toString(16) + "</div>"
    $(".register-container").append(text);
  }

  var cur_pc_loc = registeruint32[vm.PC_index] / vm.sizeof_int;
  var starting_loc = (cur_pc_loc - 5) < 0 ? 0 : (cur_pc_loc - 5);

  $(".debug-view-area").show();

  if(registeruint32[vm.FR_index] & vm.HALTED_BIT) $(".fr-bits td:nth-child(11)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.GLOBAL_INTERRUPT_ENABLE_BIT) $(".fr-bits td:nth-child(10)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.RTE_BIT) $(".fr-bits td:nth-child(9)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.TIMER1_ENABLE_BIT) $(".fr-bits td:nth-child(8)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.TIMER1_ASSERTED_BIT) $(".fr-bits td:nth-child(7)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_OUT_ENABLE_BIT) $(".fr-bits td:nth-child(6)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_OUT_ASSERTED_BIT) $(".fr-bits td:nth-child(5)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_IN_ENABLE_BIT) $(".fr-bits td:nth-child(4)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_IN_ASSERTED_BIT) $(".fr-bits td:nth-child(3)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_OUT_READY_BIT) $(".fr-bits td:nth-child(2)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_IN_READY_BIT) $(".fr-bits td:nth-child(1)").addClass('active');

  for(var i = starting_loc; i < starting_loc + 20; i++){
    var css_class = "";
    if (i == cur_pc_loc){
      css_class = "pc-loc";
    }
    $(".location-container").append('<div class="' + css_class + '">0x' + ((i*4)>>>0).toString(16) + "</div>");
    $(".instruction-container").append('<div class="' + css_class + '">' + disassemble_instruction(i) + "</div>");
    $(".binary-container").append('<div class="' + css_class + '">' + binary_disassemble_instruction(memoryuint32[i], vm) + "</div>");
    $(".value-container").append('<div class="' + css_class + '">' + " 0x" + (memoryuint32[i]>>>0).toString(16) + "</div>");
  }

  for(var i = 0; i < 20; i++){
    var val = (registeruint32[vm.SP_index] + (i * 4));
    var prefix = val % 16 == 0 ? "^^" : "&nbsp;&nbsp;";
    var color = val % 32 == 0 ? "#ee4136" : "#0000FF";
    $(".stack-address-container").append('<div><span style="color: ' + color + ';">' + prefix + '</span>0x' + (registeruint32[vm.SP_index] + (i * 4)).toString(16) + "</div>");
    $(".stack-value-container").append("<div>0x" + (memoryuint32[((registeruint32[vm.SP_index] + (i * 4)) / vm.sizeof_int)]).toString(16) + "</div>");
  }
}

$(document).ready(function () {
  if(running_kernel){
    $('.debug-mode')[0].checked = true;
  }

  var closeTabFunction = function () {
    /*  If you have the special chrome plugin installed, this will close your browser. */
    $("body").prepend("<div id='close-chrome-1234'></div>");
  }

  function test_function(test_names){
    var test_name = test_names.shift();
    $.ajax({
      url: "get-test?test_name=" + test_name,
      dataType: 'jsonp',
      jsonpCallback: "jsonpCallback",
      success: function (test_data) {
        if(test_data.error){
          alert("Error: " + test_data.error);
        }else{
          var vm = new opCPU(test_data);
          var timeout = "none";
          var output = "";
          var first_tick = true; 
          is_debug_mode = $(".debug-mode").is(':checked');
          var test_exited = false;
          var next_do_f;
          var f = function () {
            if(test_exited){ return; }
            if(first_tick){
              $(".test-status").prepend("<div><strong>" + test_name + "</strong> Has started executing.</div>");
              first_tick = false;
            }
            vm.step();
            if(is_debug_mode && timeout != "skip-to-breakpoint" && timeout != "skip-to-watch-change"){
              print_state(vm);
            }
            if(input_queue.length){
              var in_chr = input_queue.shift()
              if(vm.putc(in_chr)){
                /* Item was not output, put back onto front of queue */
                input_queue.unshift(in_chr);
              }
            }
            var c = vm.getc();
            if(c != null){
              if(c == 10){
                $(".uart1-out").append("<br/>");
              }else{
                $(".uart1-out").append(String.fromCharCode(c));
              }
              output += String.fromCharCode(c);
            }
     
            if(vm.is_halted()){
              test_exited = true;
              $(".test-status").prepend("<div>Processor halted on test " + test_name + ".  Full output was: " + output + "</div>");
              $.ajax({
                type: "POST",
                url: "submit-test-result/",
                data: { test_name: test_name, test_result: output},
                success: function (result){
                  var res = eval('(' + result + ')');
                  if(res.error){
                    $(".test-status").prepend("Submission error: " + res.error);
                    alert("Submission error: " + res.error);
                  }
                 
                  if(test_names.length){
                    test_function(test_names);
                  }else{
                    $(".test-status").prepend("<div>" + test_name + " was the last test.  Closing tab now...</div>");
                    if(!is_debug_mode){
                      closeTabFunction();
                    }
                  }
                }
              });
              /* break out of the while loop */
              timeout = "stopped";
            }
          }

          function do_f(){
            /*  Only update these every time we use do_f instead of every cycle.  jQuery is expensive. */
            is_debug_mode = $(".debug-mode").is(':checked');
            vm.set_options({
              "breakpoint_address": eval($('.breakpointvalue').val().split(" ")[0])
            });

            if(timeout == "skip-to-watch-change" | timeout == "skip-to-breakpoint"){
              if(timeout == "skip-to-watch-change"){
                vm.set_options({ "watch_active": 1 });
                while (!vm.is_halted() && !vm.watch_modified() && timeout != "stopped"){
                  f();
                };
                vm.set_options({ "watch_active": 0 });
              }else if(timeout == "skip-to-breakpoint"){
                vm.set_options({ "breakpoint_active": 1 });
                while (!vm.is_halted() && !vm.breakpoint_tripped() && timeout != "stopped"){
                  f();
                };
                vm.set_options({ "breakpoint_active": 0 });
              }
              timeout = "stopped";
              if(is_debug_mode){
                print_state(vm);
              }
            }else if(timeout == "stopped"){
              f();
            }else if(is_debug_mode){
              f();
              next_do_f = setTimeout(do_f, 1);
            }else{
              for(var i = 0; i < 10000; i++){
                f();
              }
              next_do_f = setTimeout(do_f, 1);
            }
          }

          if(1 || !is_debug_mode){
            do_f();
          }

          $(".step").click(function () {
            timeout = "stopped";
            do_f();
          });

          $(".resume").click(function () {
            timeout = "none";
            do_f();
          });

          $(".stop").click(function () {
            timeout = "stopped";
          });

          $(".skip-to-watch-change").click(function () {
            clearTimeout(next_do_f);
            vm.unmodify_watch();
            timeout = "skip-to-watch-change";
            do_f();
          });

          $(".skip-to-breakpoint").click(function () {
            clearTimeout(next_do_f);
            vm.untrip_breakpoint();
            timeout = "skip-to-breakpoint";
            do_f();
          });
        }
      }
    });
  }

  $.get(
    "get-available-tests",
    function (data){
      var d = eval('(' + data + ')');
      if(d.error){
        alert("Error: " + d.error);
      }else if(d.available_tests){
        if(specific_tests == null){
          test_function(d.available_tests);
        }else{
          test_function([specific_tests]);
        }
      }
    }
  );

  $('body').bind(
    'keypress',
    function(e) {
      var code = e.keyCode || e.which;
      input_queue.push(code);
    }
  );

  $(".debug-mode").click( function(){
    if(!$(this).is(':checked')) clear_state();
  });

  $(".symbol-breakpoints").change( function(){
    $('.breakpointvalue').val($(this).val());
  });

  var blink_fcn = function () {
    $(".debug-explanation").animate({opacity:0}, 650,"linear",function(){
      $(this).animate({opacity:1},650);
    });
    if($(".debug-mode").is(':checked')){
      setTimeout(blink_fcn, 1300);
    }
  }
  blink_fcn();


});
