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

/*  If you want to create your own data packages, just subclass the interface, and you can pass it into the OpCPU Class */

public interface OpCPUDataInterface{
    /*  Returns an array of 2 longs.  The first memeber is zero if the instruction is a machine instruction,
    and 1 if it is a skip word directive.  The second item is the value of the instruction or directive. */
    public long [] getData(Long index);
    /*  The starting address where the data should be loaded into memory */
    public Long getDataStart();
    /*  The ending address where the data should be loaded into memory */
    public Long getDataEnd();
}
