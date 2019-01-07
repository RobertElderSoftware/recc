/*
    Copyright 2019 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/

#include <stdio.h>

enum en_one_two {ONE, TWO};

static enum en_one_two one_two_array[2] = {ONE, TWO};

extern unsigned char data[6][5];

unsigned char data[6][5] = {
	{0x03, 0x00, 0x00, 0x00, 0x00},
	{0x05, 0x00, 0x00, 0x00, 0x00},
	{0x04, 0x6E, 0x65, 0x5F, 0x5F},
	{0x04, 0x62, 0x61, 0x5F, 0x64},
	{0x04, 0x62, 0x61, 0x5F, 0x64},
	{0x04, 0x6E, 0x65, 0x5F, 0x5F}
};

int output(void);
void test_data(void);

void test_data(void){
	unsigned int x = 5;
	unsigned int y = 6;
	unsigned int i;
	for(i = 0; i < x; i++){
		unsigned int j;
		for(j = 0; j < y; j++){
			unsigned char c;
			goto assign;
			assign:
			c = data[i][j];
			printf("%d,%d: %X\n", i, j, c);
		}
	}
}

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
	char c[20];
	struct food f;
	struct food g;
	int arr1[4];
	putchar('a' + one_two_array[0]);
	putchar('a' + one_two_array[1]);
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
	putchar('a' + (int)(&arr1[1] - &arr1[0]));
	(void)c;
	{
		char d[20];
		(void)d;
		{
			char e[40];
			(void)e;
		}
	}

	{
		char try1[12];
		char *result1 = &try1[0];
		unsigned int chars_required = 99;
		unsigned int i;
		for(i = 0; i < 12; i++){
			try1[i] = 'a' + (char)i;
		}
		{
			char try2[200];
			unsigned int chars_required2 = chars_required;
			for(i = 0; i < 200; i++){
				try2[i] = 'a' + (char)(i%26);
			}
			try2[199] = '\0';
			result1 = &try2[0];
			if(chars_required2 < 200){
				while(*result1){
					putchar(*result1);
					result1++;
				}
			}
		}
		for(i = 0; i < 12; i++){
			putchar(try1[i]);
		}
	}
	test_data();
	return 0;
}
