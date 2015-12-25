#ifndef ASSERT_H_DEFINED_
#define ASSERT_H_DEFINED_
/*
    Copyright 2015 Robert Elder Software Inc.
    
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


#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif

#define assert_stringify_indirect(a) #a
#define assert_stringify(a) assert_stringify_indirect(a)

#define assert(a) if(!(a)){printf("Assertion failure of expression '"assert_stringify_indirect(a)"' in file "assert_stringify(_FILE_)" on line "assert_stringify(_LINE_)".\n");}

#endif
