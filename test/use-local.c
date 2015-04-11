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

int output(void);

struct foo{
	int arr[2];
	int k;
};

void loo(struct foo);

void loo(struct foo f){
	f.arr[0] = 'e';
	f.arr[1] = 'f';
	f.k = 'g';
	(void)f;
}

struct foo makefoo(void);

struct foo makefoo(void){
	struct foo f;
	f.arr[0] = 'e';
	f.arr[1] = 'f';
	f.k = 'g';
	return f;
}

int output(void){
	struct foo foost;
	struct foo newfoo;
	int a1;
	int a = 'a';
	int b = 'b';
	int c = 'c';
	int d = 'd';
	int b2;
	int e = 'e', foo = 'f';
	int f = 'f';
	int g = 'g', boo = 'b', goo = 'g';
	int h = 'h';
	int i = 'i';
	int c3;
	putchar(a);
	putchar(b);
	a1 = '1';
	putchar(c);
	putchar(d);
	putchar(e);
	putchar(f);
	b2 = '2';
	putchar(g);
	putchar(h);
	putchar(i);
	putchar(foo);
	putchar(boo);
	putchar(goo);
	c3 = '3' - 1;
	putchar(a1);
	putchar(b2);
	putchar(c3);
	foost.arr[0] = 'a';
	foost.arr[1] = 'b';
	foost.k = 'b';
	putchar(foost.arr[0]);
	putchar(foost.arr[1]);
	putchar(foost.k);
	loo(foost);
	putchar(foost.arr[0]);
	putchar(foost.arr[1]);
	putchar(foost.k);
	newfoo = makefoo();
	putchar(newfoo.arr[0]);
	putchar(newfoo.arr[1]);
	putchar(newfoo.k);
	return 0;
}


int main(void){
	output();
	return 0;
}
