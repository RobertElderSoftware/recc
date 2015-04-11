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
/*  A very simple linear congruential generator */

unsigned int x_i = 7;

unsigned int get_random(void);
unsigned int get_input_space_partitioned_random(void);

unsigned int get_random(void){
	unsigned int a = 1103515245;
	unsigned int c = 12345;
	unsigned int m = (unsigned int)0x1 << (unsigned int)31;
	x_i = (a * x_i + c) % m;
	return x_i;
}

unsigned int get_input_space_partitioned_random(void){
	unsigned int class = get_random() % (unsigned int)5;
	switch (class){
		case 0:{
			return get_random();
		}case 1:{
			return (unsigned int)0;
		}case 2:{
			return (unsigned int)1;
		}case 3:{
			return (unsigned int)4294967295;
		}case 4:{
			return get_random() % (unsigned int)300;
		}default:{
			return 0;
		}
	}
}
