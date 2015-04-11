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

void inc_array_by_ptr(int (*)[4]);

void inc_array(int [4]);

void inc_array_by_ptr(int (*arr)[4]){
	(*arr)[0] = (*arr)[0] + 1;
	(*arr)[1] = (*arr)[1] + 1;
	(*arr)[2] = (*arr)[2] + 1;
	(*arr)[3] = (*arr)[3] + 1;
}

void inc_array(int arr[4]){
	arr[0] = arr[0] + 1;
	arr[1] = arr[1] + 1;
	arr[2] = arr[2] + 1;
	arr[3] = arr[3] + 1;
}

/*  Function that take a pointer to an array of 4 ints, then returns a pointer to an array of 4 ints */
int (*get_arr_ptr(int (*)[4]))[4];

int (*get_arr_ptr(int (*arr)[4]))[4]{
	return arr;
}

/*  Function that takes no arguments and returns (a pointer to a function that takes a pointer to an array of 4 ints, then returns (a pointer to an array of 4 ints)) */
int (*(*get_arr_ptr_function(void))(int (*)[4]))[4];

int (*(*get_arr_ptr_function(void))(int (*)[4]))[4]{
	return get_arr_ptr;
}

/*  Function that takes no arguments and returns (a pointer to a function that takes no arguments and returns (a pointer to a function that takes a pointer to an array of 4 ints, then returns (a pointer to an array of 4 ints))) */
int (*(*(*get_arr_ptr_function_ptr(void))(void))(int (*)[4]))[4];

int (*(*(*get_arr_ptr_function_ptr(void))(void))(int (*)[4]))[4]{
	return get_arr_ptr_function;
}

void t1(void);

void t1(void){
	int arr1[4];
	int (*arrof2ptr)[4]; /* pointer to array of ints */
	int (*putchar_fcns_arr[4])(int) /* array of pointers to functions */;
	int (*(*putchar_fcns)[4])(int) /* pointer to an array of pointers to functions that take an int */;
	putchar_fcns = &putchar_fcns_arr;
	arrof2ptr = &arr1;
	(*arrof2ptr)[0] = 'a';
	(*arrof2ptr)[1] = 'b';
	arr1[2] = 'c';
	arr1[3] = 'd';
	putchar(arr1[0]);
	putchar(arr1[1]);
	putchar((*arrof2ptr)[2]);
	putchar((*arrof2ptr)[3]);
	inc_array(arr1);
	putchar(arr1[0]);
	putchar(arr1[1]);
	putchar((*arrof2ptr)[2]);
	putchar((*arrof2ptr)[3]);
	inc_array_by_ptr(arrof2ptr);
	putchar((*get_arr_ptr(arrof2ptr))[0]);
	putchar((*get_arr_ptr(arrof2ptr))[1]);
	putchar((*(get_arr_ptr_function()(arrof2ptr)))[1]);
	putchar((*(get_arr_ptr_function_ptr()()(arrof2ptr)))[2]);
	(*putchar_fcns)[0] = putchar;
	(*putchar_fcns)[1] = putchar;
	(*putchar_fcns)[2] = putchar;
	(*putchar_fcns)[3] = putchar;
	(*putchar_fcns)[0]('a');
	(*putchar_fcns)[1]('b');
	(*putchar_fcns)[2]('c');
	(*putchar_fcns)[3]('d');
}

int blah3_fcn(int (*(*)[4])(int));

int blah3_fcn(int (*(*fcn_ptrs)[4])(int)){
	(*fcn_ptrs)[0]('g');
	(*fcn_ptrs)[1]('h');
	(*fcn_ptrs)[2]('i');
	(*fcn_ptrs)[3]('j');
	return 0;
}

void t2(void);

void t2(void){
	int (*blah1[4])(int (*(*)[4])(int)); /* array of pointers to functions that take a pointer to an array of functions */
	int (*(*blah2)[4])(int (*(*)[4])(int)); /* pointer to an array of pointers to functions that take a pointer to an array of functions */
	int (*putchar_fcns_arr[4])(int) /* array of pointers to functions */;
	int (*(*putchar_fcns)[4])(int) /* pointer to an array of pointers to functions that take an int */;
	blah2 = &blah1;
	putchar_fcns = &putchar_fcns_arr;
	(*putchar_fcns)[0] = putchar;
	(*putchar_fcns)[1] = putchar;
	(*putchar_fcns)[2] = putchar;
	(*putchar_fcns)[3] = putchar;
	blah1[0] = blah3_fcn;
	blah1[1] = blah3_fcn;
	blah1[2] = blah3_fcn;
	blah1[3] = blah3_fcn;
	(*blah2)[0](putchar_fcns);
	(*blah2)[1](putchar_fcns);
	(*blah2)[2](putchar_fcns);
	(*blah2)[3](putchar_fcns);
}

int main(void){
	t1();
	t2();
	return 0;
}
