#   Copyright 2016 Robert Elder Software Inc.
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
import sys
import select
import time
import sys, tty, termios
from op_cpu_package.python_l0_module import OpCPUData
from op_cpu_package.op_cpu_module import OpCPU

#  A Linux interface to using the python implementation of the One Page CPU emulator

def main_loop():
    loader_data = OpCPUData()
    op_cpu = OpCPU(loader_data)
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    tty.setraw(sys.stdin.fileno())
    input_buffer = []  #  Characters to be sent to emulator
    while not op_cpu.is_halted():
        in_chrs = select.select([sys.stdin], [], [], 0.0001)[0]
        if not in_chrs:
            for x in range(0, 10000):
                r = op_cpu.vm_getc()
                if 'chr' in r:
                    sys.stdout.write(chr(r['chr']))
                    if r['chr'] == 10:
                        sys.stdout.write('\r')
                    sys.stdout.flush()
                if len(input_buffer):
                    inchr = input_buffer.pop()
                    if op_cpu.vm_putc(inchr):  #  Not able to input chr
                        input_buffer = [inchr] + input_buffer
                
                op_cpu.step()
        else:
            dobreak = False
            for file in in_chrs:
                c = file.read(1)
                input_buffer = input_buffer + [ord(c)]
                if ord(c) == 3:
                    dobreak = True
            if dobreak:
                break
    termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

main_loop()
