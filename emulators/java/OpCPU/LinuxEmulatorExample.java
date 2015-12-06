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
        System.out.print("Loading kernel image.  Sorry, but due to limitations in Java class sizes, this is currently rather slow.\n");
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
