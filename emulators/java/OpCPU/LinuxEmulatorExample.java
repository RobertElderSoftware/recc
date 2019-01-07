/*
    Copyright 2019 Robert Elder Software Inc.
    
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

import java.io.BufferedInputStream;
import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import OpCPU.KernelData;
import OpCPU.OpCPU;
import OpCPU.OpCPUDataInterface;

class LinuxEmulatorExample {
    public static void main(String [] args){
        String[] raw = {"/bin/sh", "-c", "stty raw -echo </dev/tty"};
        String[] cooked = {"/bin/sh", "-c", "stty cooked echo </dev/tty"};
        List<Integer> stdinChrs = new ArrayList<Integer>();
        KernelData d = new KernelData();
        System.out.print("Loading kernel image...\n");
        OpCPU opCPU = new OpCPU(d);
        try{
            System.out.print("Kernel image has been loaded. All input is now being handled by the emulator.\n");
            System.out.print("Press 'q' to quit.\n");
            Runtime.getRuntime().exec(raw);
            BufferedInputStream stdin = new BufferedInputStream(System.in);
            while(!opCPU.isHalted()){
                /*  Get output from vm */
                long [] out_chrs = opCPU.vmGetC();
                if(out_chrs[0] == 1){
                    System.out.print((char)out_chrs[1]);
                    if(out_chrs[1] == 10){
                        System.out.print((char)13);
                    }
                }
                /*  Buffer input from user */
                if(stdin.available() > 0){
                    int c = stdin.read();
                    stdinChrs.add(c);
                }
                /*  Send input from user to emulator */
                if(stdinChrs.size() != 0){
                	int c = stdinChrs.remove(0);
                	if(opCPU.vmPutC(c)){
                	    /* Weren't able to input the char, put it back. */
                	    stdinChrs.add(0, c);
                	}
                }
                opCPU.step();
            }
            Runtime.getRuntime().exec(cooked);
        }catch(Exception e){
            try{
                Runtime.getRuntime().exec(cooked);
            }catch(Exception ee){
                System.out.println("Unable to restore cooked mode.");
            }
            System.out.println("Exception:");
            e.printStackTrace();
        }
    }
}
