#ifndef STDARG_H_DEFINED_
#define STDARG_H_DEFINED_
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
#define va_list unsigned int *
#define va_start(a,b) a = (unsigned int*)&b
#define va_arg(a,b) *((b*)(a=(unsigned int *)((unsigned int)a - sizeof(unsigned int))))
#define va_end(a) 
#define va_copy(a,b) a = b
#endif
