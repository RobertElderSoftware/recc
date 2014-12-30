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
int signed_division(unsigned int, unsigned int);

int signed_division(unsigned int a, unsigned int b){
	unsigned int high_a = a & (0x80000000);
	unsigned int high_b = b & (0x80000000);
	unsigned int size_a;
	unsigned int size_b;
	if(high_a){
		size_a = ~a + 1; /* For negatives the magnitude is given from the two's complement */
	}else{
		size_a = a;
	}
	if(high_b){
		size_b = ~b + 1; /* For negatives the magnitude is given from the two's complement */
	}else{
		size_b = b;
	}
	if(high_a && high_b){
		return (int)(size_a / size_b);
	}else if(!high_a && !high_b){
		return (int)(size_a / size_b);
	}else if(!high_a && high_b){
		return -(int)(size_a / size_b);
	}else{
		return -(int)(size_a / size_b);
	}
}
