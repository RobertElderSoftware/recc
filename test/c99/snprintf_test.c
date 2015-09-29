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

#include <stdio.h>

int main(void){
	char c[40];
	printf("Num bytes needed is %d\n", snprintf(&c[0], 40, "%d", 99));
	printf("Num bytes needed is %d\n", snprintf(&c[0], 2, "%d %d", 99, 54353));
	printf("Num bytes needed is %d\n", snprintf(&c[0], 40, "%d", 333934));
	printf("%s\n", &c[0]);
	return 0;
}
