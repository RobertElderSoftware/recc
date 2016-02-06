#ifndef BUILTIN_C99LIBC_IN_C89_
#define BUILTIN_C99LIBC_IN_C89_
/*
    Copyright 2016 Robert Elder Software Inc.
    
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
#include <stddef.h>
#include <stdarg.h>
#include "print_buff_add.h"
#include "print_string.h"
#include "print_decimal.h"
#include "print_hexadecimal.h"
#include "../types/builtin/struct_printing_state.h"

unsigned int c89_vsnprintf(char *, size_t, const char *, va_list);
#endif
