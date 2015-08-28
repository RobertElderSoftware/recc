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

extern int globe;
int globe;

int output(void);

char get_a(void);
char get_b(void);
void * get_ptr(void);

char get_a(void){
	return 'a';
}

char get_b(void){
	return 'b';
}

void * get_ptr(void){
	return &globe;
}

enum enum1 {en_1, en_2};

int output(void){
	int c = 115;
	int z = 0;
	int one = 1;
	int two = 2;
	int changes1 = 1;
	int changes2 = 1;
	int changes3 = 1;
	enum enum1 en1_ex = en_1;
	void * test;
	void * test1 = &c;
	putchar(49 << 1); /* b */
	putchar(396 >> 2); /* c */
	putchar('3' - 1);
	putchar('3' + 1);
	putchar('3' + 2);
	putchar('3' + 8);
	putchar('3' + (1 | 1) + (0 | 1) + (1 | 0) + (0 | 0) + (1 | 1));
	putchar('3' + 8 - 33 + 67 - 22);
	putchar('3' + (1 & 1) + (0 & 1) + (1 & 0) + (0 & 0) + (1 & 1));
	if(one & z){
		putchar('a');
	}else{
		putchar('b');
	}

	test = en1_ex == en_1 ? get_ptr() : test1;
	if(test == (void *)&globe){
		putchar('d');
	}

	putchar(z ? get_a() : get_b());
	putchar(one ? get_a() : get_b());
	putchar(z ? 'a' : 'b');
	putchar(z ? 'a' : 'b');
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

	putchar('\n');

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

	putchar('\n');

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

	if(one && one && z){
		putchar('a');
	}else{
		putchar('b');
	}

	putchar(c += 3);
	putchar(c -= 3);

	/* Test short circuit evaluation */
	if((changes1 = one) || (changes2 = z)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = z) || (changes2 = one)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = z) || (changes2 = z) || (changes3 = z)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = z) || (changes2 = z) || (changes3 = one)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = z) || (changes2 = one) || (changes3 = z)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = z) || (changes2 = one) || (changes3 = one)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = one) || (changes2 = z) || (changes3 = z)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = one) || (changes2 = z) || (changes3 = one)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = one) || (changes2 = one) || (changes3 = z)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = one) || (changes2 = one) || (changes3 = one)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = one) && (changes2 = z)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = z) && (changes2 = one)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = z) && (changes2 = z) && (changes3 = z)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = z) && (changes2 = z) && (changes3 = one)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = z) && (changes2 = one) && (changes3 = z)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = z) && (changes2 = one) && (changes3 = one)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = one) && (changes2 = z) && (changes3 = z)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = one) && (changes2 = z) && (changes3 = one)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = one) && (changes2 = one) && (changes3 = z)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	/* Test short circuit evaluation */
	if((changes1 = one) && (changes2 = one) && (changes3 = one)){
		putchar('a' + changes1);
		putchar('a' + changes2);
		putchar('a' + changes3);
	}

	return 0;
}

int main(void){
	output();
	return 0;
}
