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
int putchar(int);

int foo1(int, int, int, int, int, int, int, int, int, int, int, int);
int foo2(int, int, int, int, int, int, int, int, int, int, int, int);
int foo3(int, int, int, int, int, int, int, int, int, int, int, int);

int foo1(int i, int j, int k, int l, int m, int n, int o, int p, int q, int r, int s, int t){
	putchar(i);
	putchar(j);
	putchar(k);
	putchar(l);
	putchar(m);
	putchar(n);
	putchar(o);
	putchar(p);
	putchar(q);
	putchar(r);
	putchar(s);
	putchar(t);
	return 0;
}

int foo2(int i, int j, int k, int l, int m, int n, int o, int p, int q, int r, int s, int t){
	foo1(i, j, k, l, m, n, o, p, q, r, s, t);
	return 0;
}

int foo3(int i, int j, int k, int l, int m, int n, int o, int p, int q, int r, int s, int t){
	foo2(i, j, k, l, m, n, o, p, q, r, s, t);
	return 0;
}

int main(void){
	foo3('h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!');
	return 0;
}
