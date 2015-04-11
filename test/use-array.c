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

void inc(int * i);

void inc(int * i){
	(*i)++;
}

struct food{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
	unsigned int e;
};

int output(void){
	int multi[2][3], multi2[2][3][4][5][6];
	int i, arr[3];
	int b[4];
	int d;
	multi[0][0] = 'a';
	multi[0][1] = 'b';
	multi[0][2] = 'c';
	multi[1][0] = 'd';
	multi[1][1] = 'e';
	multi[1][2] = 'f';
	multi2[0][1][2][3][4] = '1';
	multi2[0][0][0][0][0] = '2';
	multi2[0][0][1][2][3] = '3';
	multi2[0][1][1][2][3] = '4';
	putchar(multi2[0][1][2][3][4]);
	putchar(multi2[0][0][0][0][0]);
	putchar(multi2[0][0][1][2][3]);
	putchar(multi2[0][1][1][2][3]);
	putchar(multi[0][0]);
	putchar(multi[0][1]);
	putchar(multi[0][2]);
	putchar(multi[1][0]);
	putchar(multi[1][1]);
	putchar(multi[1][2]);
	i = 'e';
	d = 'i';
	arr[0] = '7';
	arr[1] = '6';
	arr[2] = '5';
	putchar(i);
	putchar(arr[0]);
	putchar(arr[1]);
	putchar(arr[2]);
	putchar(d);
	b[0] = '7';
	b[1] = '8';
	b[2] = '9';
	b[3] = 'e';
	putchar(b[0]);
	putchar(b[1]);
	putchar(b[2]);
	putchar(b[3]);
	inc(&b[3]);
	putchar(b[3]);
	inc(b);
	putchar(b[0]);
	inc(&b[3]);
	putchar(b[3]);
	inc(b);
	putchar(b[0]);
	inc(&b[3]);
	putchar(b[3]);
	inc(b);
	putchar(b[0]);
	return 0;
}

struct food get_food(struct food);

struct food get_food(struct food in){
	struct food k;
	k.a = in.a + 1;
	k.b = in.b + 1;
	k.c = in.c + 1;
	k.d = in.d + 1;
	k.e = in.e + 1;
	return k;
}


void inc_array(int [4]);

void inc_array(int arr1[4]){
	arr1[0] = arr1[0] + 1;
	arr1[1] = arr1[1] + 1;
	arr1[2] = arr1[2] + 1;
	arr1[3] = arr1[3] + 1;
}

int main(void){
	struct food f;
	struct food g;
	int arr1[4];
	f.a = 'a';
	f.b = 'b';
	f.c = 'c';
	f.d = 'd';
	f.e = 'e';
	g = get_food(f);
	putchar((int)f.a);
	putchar((int)f.b);
	putchar((int)f.c);
	putchar((int)f.d);
	putchar((int)f.e);
	putchar((int)g.a);
	putchar((int)g.b);
	putchar((int)g.c);
	putchar((int)g.d);
	putchar((int)g.e);
	output();
	arr1[0] = 'a';
	arr1[1] = 'b';
	arr1[2] = 'c';
	arr1[3] = 'd';
	inc_array(arr1);
	putchar(arr1[0]);
	putchar(arr1[1]);
	putchar(arr1[2]);
	putchar(arr1[3]);
	return 0;
}
