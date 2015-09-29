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

int main(int, char * []);
int main_argv_indirect(void);

int main_argv_indirect(void){
	/*  Call the 'real' main method with arguments */
	int argc = 1;
	char * argv[2];
	argv[0] = (char *)"./test/c89/main_with_parameters.exec"; /*  Currently used for a specific unit test. */
	argv[1] = (char *)0;
	return main(argc, argv);
}
