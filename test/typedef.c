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

typedef enum {en1, en2} t_enum;

typedef enum {en3, en4} t_enum_1, t_enum_2;

struct node;

typedef struct node {
	struct node * next;
	struct node *prev;
	int i;
	unsigned int pad;
} node;

typedef int an_integer;

t_enum_2 f21(t_enum_2);

t_enum_2 f21(t_enum_2 e){
	return e;
}

t_enum_1 f20(t_enum_1);

t_enum_1 f20(t_enum_1 e){
	return e;
}

t_enum f19(t_enum);

t_enum f19(t_enum e){
	return e;
}

node * f18(node *);

node * f18(node * n){
	putchar(n->i);
	n->next = n;
	return n;
}

void f17(an_integer);

void f17(an_integer foo){
	putchar(foo);
}

an_integer f16(void);

an_integer f16(void){
	int i = 'd';
	return i;
}

void f15(void);

void f15(void){
	typedef struct {int a; int b;} abc;
	abc i;
	i.a = 'a';
	i.b = 'b';
	i.a = 'a';
	i.b = 'b';
	i.a = 'a';
	i.b = 'b';
	putchar(i.a);
	putchar(i.b);
	putchar(i.a);
	putchar(i.b);
	putchar(i.a);
	putchar(i.b);
}

void f14(void);

void f14(void){
	struct boo {int a; int b;};
	typedef struct boo named[3];
	named i;
	i[0].a = 'a';
	i[0].b = 'b';
	i[1].a = 'a';
	i[1].b = 'b';
	i[2].a = 'a';
	i[2].b = 'b';
	putchar(i[0].a);
	putchar(i[0].b);
	putchar(i[1].a);
	putchar(i[1].b);
	putchar(i[2].a);
	putchar(i[2].b);
}

void f13(void);

void f13(void){
	struct foo {int a; int b;};
	typedef struct foo named;
	named i;
	i.a = 'a';
	i.b = 'b';
	putchar(i.a);
	putchar(i.b);
}

void f12(void);

void f12(void){
	int k;
	int * parray[3];
	typedef int *arr[3];
	arr b;
	parray[0] = &k;
	b[0] = parray[0];
	*(parray[0]) = 'd';
	putchar(*(b[0]));
}

void f11(void);

void f11(void){
	int m;
	int * g = &m;
	int ** array[3];
	typedef int ** arr[3];
	arr * b;
	b = &array;
	(*b)[0] = &g;
	*(*((*b)[0])) = 'd';
	putchar(*(*((*b)[0])));
}

void f10(void);

void f10(void){
	int m;
	int *array[3];
	typedef int * arr[3];
	arr * b;
	b = &array;
	(*b)[0] = &m;
	*((*b)[0]) = 'd';
	putchar(*((*b)[0]));
}

void f9(void);

void f9(void){
	int array[3];
	typedef int arr[3];
	arr * b;
	b = &array;
	(*b)[0] = 'a';
	putchar((*b)[0]);
}

void f8(void);

void f8(void){
	int k = 'a'; 
	int * nn = &k; 
	typedef int * ptr;
	ptr * j = &nn;
	int ** p;
	*j = &k;
	p = j;
	putchar(**p);
	putchar(**j);
}

void f7(void);

void f7(void){
	int i; 
	typedef int * ptr;
	ptr arr[3];
	arr[0] = &i;
	(void)arr[0];
}

void f6(void);

void f6(void){
	int t[4][3];
	typedef int arr[3];
	typedef arr (*brr)[4];
	int (*a)[4][3];
	brr b = &t;
	a = b;
	(*a)[0][0] = 'd';
	putchar((*a)[0][0]);
}

void f5(void);

void f5(void){
	typedef int (*arr)[4];
	typedef arr brr[3];
	int (*(*b)[3])[4];
	brr c;
	b = &c;
	(void)b;
}

void f4(void);

void f4(void){
	typedef int (*a1)[1];
        typedef a1 (*a2)[2];
        typedef a2 (*a3)[3];
        typedef a3 (*a4)[4];
        int (*(*a)[2])[1];
        int (*(*(*(*foo)[4])[3])[2])[1];
        a4 boo;
        int k[1];
        int (*l)[1];
        int (*m[2])[1];
        a2 b;
        l = &k;
        m[0] = l;
        a = &m;
        b = a;
        a = (int (*(*)[2])[1])0;
        foo = (int (*(*(*(*)[4])[3])[2])[1])0;
        boo = foo;
        (void)b;
        (void)boo;
}

void f3(void);

void f3(void){
	typedef int a1[1];
	typedef a1 a2[2];
	typedef a2 a3[3];
	typedef a3 a4[4];
	a4 b;
	b[0][0][0][0] = 'a';
	b[0][0][1][0] = 'b';
	b[0][1][0][0] = 'c';
	b[0][1][1][0] = 'd';
	b[0][2][0][0] = 'e';
	b[0][2][1][0] = 'f';
	putchar(b[0][0][0][0]);
	putchar(b[0][0][1][0]);
	putchar(b[0][1][0][0]);
	putchar(b[0][1][1][0]);
	putchar(b[0][2][0][0]);
	putchar(b[0][2][1][0]);
}

void f2(void);

void f2(void){
	typedef int int_arr[3];
	typedef int (*int_arr_ptr)[3];
	int_arr arr1;
	int_arr_ptr arr1_ptr;
	arr1_ptr = &arr1;
	(*arr1_ptr)[0] = 'a';
	(*arr1_ptr)[1] = 'b';
	(*arr1_ptr)[2] = 'c';
	putchar(arr1[0]);
	putchar(arr1[1]);
	putchar(arr1[2]);
}

void f1(void);

void f1(void){
	typedef int INT;
	typedef INT JNT;
	typedef JNT KNT;
	INT i = 'i';
	JNT j = 'j';
	KNT k = 'k';
	typedef int int_arr[3];
	int_arr foo_array;
	int_arr foo_array_arr[2];

	foo_array_arr[0][0] = 'a';
	foo_array_arr[0][1] = 'b';
	foo_array_arr[1][0] = 'c';
	foo_array_arr[1][1] = 'd';
	foo_array_arr[2][0] = 'e';
	foo_array_arr[2][1] = 'f';


	putchar(foo_array_arr[0][0]);
	putchar(foo_array_arr[0][1]);
	putchar(foo_array_arr[1][0]);
	putchar(foo_array_arr[1][1]);
	putchar(foo_array_arr[2][0]);
	putchar(foo_array_arr[2][1]);
	foo_array[0] = 'c';
	foo_array[1] = 'd';
	foo_array[2] = 'e';
	putchar('a');
	putchar(foo_array[0]);
	putchar(foo_array[1]);
	putchar(foo_array[2]);
	putchar(i);
	putchar(j);
	putchar(k);
}

int main(void){
	node n;
	n.i = 'r';
	f1();
	f2();
	f3();
	f4();
	f5();
	f6();
	f7();
	f8();
	f9();
	f10();
	f11();
	f12();
	f13();
	f14();
	f15();
	putchar(f16());
	f17('g');
	putchar(f18(&n)->i);
	putchar(f19(en1) + 'a');
	putchar(f20(en4) + 'a');
	putchar(f21(en4) + 'a');
	return 0;
}
