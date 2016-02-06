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
var running_kernel = window.top.location.href.indexOf("kernel_mode") > -1 ? true : false;
var input_queue = [];
var memory_size_ints;
var is_debug_mode = running_kernel ? true : false;
var num_special_registers = 6;
var begin_paused = false;

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

function get_binary_string(int32bit, base){
  var str = "";
  do{
    base /= 2;
    str += (int32bit & base) ? "1" : "0";
  }while (base != 1);
  return str;
}

function binary_disassemble_instruction(instruction, vm){
  var branch_binary = instruction & vm.BRANCH_DISTANCE_MASK;
  var operation = (vm.OP_CODE_MASK & instruction)>>>0;
  var literal = (vm.LITERAL_MASK & instruction)>>>0;
  var ra = (vm.ra_MASK & instruction) / (1 << vm.ra_OFFSET);
  var rb = (vm.rb_MASK & instruction) / (1 << vm.rb_OFFSET);
  var rc = (vm.rc_MASK & instruction) / (1 << vm.rc_OFFSET);
  var op_code_str = get_binary_string(operation >> vm.OP_CODE_OFFSET, 1 << vm.BITS_PER_OP_CODE);
  var ra_str = get_binary_string(ra, 1 << vm.BITS_PER_REGISTER);
  var rb_str = get_binary_string(rb, 1 << vm.BITS_PER_REGISTER);
  var rc_str = get_binary_string(rc, 1 << vm.BITS_PER_REGISTER);
  switch(operation){
    case vm._asm_op_codes["add"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str + " r" + rc_str;
      break;
    }case vm._asm_op_codes["sub"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str + " r" + rc_str;
      break;
    }case vm._asm_op_codes["mul"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str + " r" + rc_str;
      break;
    }case vm._asm_op_codes["div"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str + " r" + rc_str;
      break;
    }case vm._asm_op_codes["beq"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str + " i" + get_binary_string(branch_binary, 1 << vm.BITS_PER_BRANCH_DIST);
      break;
    }case vm._asm_op_codes["blt"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str + " i" + get_binary_string(branch_binary, 1 << vm.BITS_PER_BRANCH_DIST);
      break;
    }case vm._asm_op_codes["loa"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str;
      break;
    }case vm._asm_op_codes["sto"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str;
      break;
    }case vm._asm_op_codes["ll"]:{
      return "op" + op_code_str + " r" + ra_str + " l" + get_binary_string(literal, 1 << vm.BITS_PER_LITERAL);
      break;
    }case vm._asm_op_codes["and"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str + " r" + rc_str;
      break;
    }case vm._asm_op_codes["or"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str + " r" + rc_str;
      break;
    }case vm._asm_op_codes["not"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str;
      break;
    }case vm._asm_op_codes["shr"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str;
      break;
    }case vm._asm_op_codes["shl"]:{
      return "op" + op_code_str + " r" + ra_str + " r" + rb_str;
      break;
    }default:{
      return "op-code:" + get_binary_string(operation >> vm.OP_CODE_OFFSET, 1 << vm.BITS_PER_OP_CODE) + "(Unrecognized op code.)";
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
  $(".inspect-memory-address-container").html('');
  $(".inspect-memory-value-container").html('');
}


function print_state(vm){
  var memoryuint32 = vm.get_memoryuint32();
  var registeruint32 = vm.get_registeruint32();
  function disassemble_instruction(index){
    var current_inst = memoryuint32[index];
    var low_bits = (current_inst & vm.BRANCH_DISTANCE_MASK) >>> 0;
    var branch_dist = (current_inst & vm.BRANCH_DISTANCE_SIGN_BIT) >>> 0 ? -((((vm.BRANCH_DISTANCE_MASK & ~(low_bits)) >>> 0) + 1) >>> 0) : low_bits;
    var operation = (vm.OP_CODE_MASK & current_inst)>>>0;
    var r = new Array();
    r[0] = (vm.ra_MASK & current_inst) / (1 << vm.ra_OFFSET);
    r[1] = (vm.rb_MASK & current_inst) / (1 << vm.rb_OFFSET);
    r[2] = (vm.rc_MASK & current_inst) / (1 << vm.rc_OFFSET);

    var op_name = null;
    var parts = [];
    for(var key in vm._asm_op_codes){
      if(vm._asm_op_codes[key] == operation){
        op_name = key;
        break;
      }
    }

    if(op_name != null){
      for(var i = 0; i < vm._asm_templates[op_name].length; i++){
        if(vm._asm_templates[op_name][i] == "register"){
          parts.push(get_register_name(r[i]));
        }else if(vm._asm_templates[op_name][i] == "hexadecimal-number"){
          if(op_name == "ll"){
            parts.push("0x" + (vm.LITERAL_MASK & memoryuint32[index]).toString(16));
          }
        }else if(vm._asm_templates[op_name][i] == "decimal-number"){
          parts.push(branch_dist);
        }
      }
      return "<span class='op-name'>" + op_name + "</span> " + parts.join(" ");
    }else{
      return "<span class='op-name'>Not an instruction</span> ";
    }

  }

  var current_inst = memoryuint32[registeruint32[0] / vm.sizeof_int];
  var low_bits = (current_inst & vm.BRANCH_DISTANCE_MASK) >>> 0;
  var branch_dist = (current_inst & vm.BRANCH_DISTANCE_SIGN_BIT) >>> 0 ? -((((vm.BRANCH_DISTANCE_MASK & ~(low_bits)) >>> 0) + 1) >>> 0) : low_bits;
  var operation = (vm.OP_CODE_MASK & current_inst)>>>0;
  var r = new Array();
  r[0] = (vm.ra_MASK & current_inst) / (1 << vm.ra_OFFSET);
  r[1] = (vm.rb_MASK & current_inst) / (1 << vm.rb_OFFSET);
  r[2] = (vm.rc_MASK & current_inst) / (1 << vm.rc_OFFSET);

  var op_name = null;
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

  if(registeruint32[vm.FR_index] & vm.HALTED_BIT) $(".fr-bits td:nth-child(12)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.GLOBAL_INTERRUPT_ENABLE_BIT) $(".fr-bits td:nth-child(11)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.RTE_BIT) $(".fr-bits td:nth-child(10)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.TIMER1_ENABLE_BIT) $(".fr-bits td:nth-child(9)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.TIMER1_ASSERTED_BIT) $(".fr-bits td:nth-child(8)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_OUT_ENABLE_BIT) $(".fr-bits td:nth-child(7)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_OUT_ASSERTED_BIT) $(".fr-bits td:nth-child(6)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_IN_ENABLE_BIT) $(".fr-bits td:nth-child(5)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_IN_ASSERTED_BIT) $(".fr-bits td:nth-child(4)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_OUT_READY_BIT) $(".fr-bits td:nth-child(3)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.UART1_IN_READY_BIT) $(".fr-bits td:nth-child(2)").addClass('active');
  if(registeruint32[vm.FR_index] & vm.DIV_ZERO_BIT) $(".fr-bits td:nth-child(1)").addClass('active');

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

  var inspectmemoryvalue;
  try { inspectmemoryvalue = eval($('.inspectmemoryvalue').val()); } catch (e) { inspectmemoryvalue = 0; }
  if(isNaN(inspectmemoryvalue) || inspectmemoryvalue % 4 != 0) {inspectmemoryvalue = 0;}
  for(var i = 0; i < 20; i++){
    $(".inspect-memory-address-container").append('<div><span style=";"></span>0x' + (inspectmemoryvalue + (i * vm.sizeof_int)).toString(16) + "</div>");
    $(".inspect-memory-value-container").append("<div>0x" + memoryuint32[(inspectmemoryvalue + (i * vm.sizeof_int)) / vm.sizeof_int].toString(16) + "</div>");
  }

}

$(document).ready(function () {
  $('.debug-mode')[0].checked = is_debug_mode;

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
              output += "%" + (c < 16 ? "0" + c.toString(16) : c.toString(16)).toUpperCase(); /* URL encode */
            }
     
            if(vm.is_halted()){
              test_exited = true;
              $(".test-status").prepend("<div>Processor halted on test " + test_name + ".  Full output was: " + decodeURIComponent(output)+ "</div>");
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
                while (!vm.is_halted() && !vm.is_watch_modified() && timeout != "stopped"){
                  f();
                };
                vm.set_options({ "watch_active": 0 });
              }else if(timeout == "skip-to-breakpoint"){
                vm.set_options({ "breakpoint_active": 1 });
                while (!vm.is_halted() && !vm.is_breakpoint_tripped() && timeout != "stopped"){
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

          if(!begin_paused){
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
        test_function(d.available_tests);
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
