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
import sys
import select
import time
import sys, tty, termios
from python_op_cpu_package.python_l0_module import OpCPUData
from python_op_cpu_package.python_op_cpu_module import OpCPU

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
