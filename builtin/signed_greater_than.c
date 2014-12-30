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
int signed_greater_than(unsigned int, unsigned int);

int signed_greater_than(unsigned int a, unsigned int b){
	/* Takes a and b and treats their bit patters as twos complement signed integers and performs 32 bit signed a >= b on them */
	unsigned int high_a = a & (0x80000000);
	unsigned int high_b = b & (0x80000000);
	unsigned int low_a = a & (~(0x80000000));
	unsigned int low_b = b & (~(0x80000000));
	if(high_a && high_b){ /* both negative numbers */
		return low_a > low_b; /*  In twos complement, the smallest negatives have the 'largest' bit pattern (after the sign bit) */
	}else if(!high_a && !high_b){ /* both positive numbers */
		return low_a > low_b;
	}else if(!high_a && high_b){
		return 1;
	}else{ /*(high_a && !high_b)*/
		return 0;
	}
}
