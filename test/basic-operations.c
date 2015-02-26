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
int putchar(int);

int output(void);

int output(void){
	int c = 115;
	int z = 0;
	int one = 1;
	int two = 2;
	putchar(49 << 1); /* b */
	putchar(396 >> 2); /* c */
	putchar('3' - 1);
	putchar('3' + 1);
	putchar('3' + 2);
	putchar('3' + 8);
	putchar('3' + (1 | 1) + (0 | 1) + (1 | 0) + (0 | 0) + (1 | 1));
	putchar('3' + 8 - 33 + 67 - 22);
	putchar('3' + (1 & 1) + (0 & 1) + (1 & 0) + (0 & 0) + (1 & 1));
	if(one & 0){
		putchar('a');
	}else{
		putchar('b');
	}

	putchar(z ? 'a' : 'b');
	putchar(one ? 'a' : 'b');

	if(one & 1)
		putchar('b');

	if(one & 1){
		putchar('3');
	}

	if(one & 1){
		putchar('b');
	}else if (two) {
		putchar('i');
	}else if (z & z & one) {
		putchar('d');
	}

	if(one & z){
		putchar('b');
	}else if (two) {
		putchar('i');
	}else if (z & z & one) {
		putchar('d');
	}

	if(one & z){
		putchar('b');
	}else if (z) {
		putchar('i');
	}else if (2 | 4) {
		putchar('d');
	}

	if(one > 0){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one > 1){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one > 2){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one < 0){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one < 1){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one < 2){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one <= 0){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one <= 1){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one <= 2){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one >= 0){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one >= 1){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one >= 2){
		putchar('a');
	}else{
		putchar('b');
	}

	if(z || 0){
		putchar('a');
	}else{
		putchar('b');
	}

	if(0 || one){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one || 0){
		putchar('a');
	}else{
		putchar('b');
	}

	if(one || one){
		putchar('a');
	}else{
		putchar('b');
	}

	if(z && z){
		putchar('a');
	}else{
		putchar('b');
	}

	if(z && 1){
		putchar('a');
	}else{
		putchar('b');
	}

	if(1 && z){
		putchar('a');
	}else{
		putchar('b');
	}

	if(1 && 1 && z){
		putchar('a');
	}else{
		putchar('b');
	}

	putchar(c += 3);
	putchar(c -= 3);

	return 0;
}

int main(void){
	output();
	return 0;
}
