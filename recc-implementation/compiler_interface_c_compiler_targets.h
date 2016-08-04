#ifndef COMPILER_INTERFACE_C_COMPILER_TARGETS_H_
#define COMPILER_INTERFACE_C_COMPILER_TARGETS_H_
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

#include "../recc-implementation/compiler_interface_common.h"
#ifndef PREPROCESSOR_H_
#include "preprocessor.h"
#endif
#ifndef LEXER_H_
#include "lexer.h"
#endif
#ifndef PARSER_H_
#include "parser.h"
#endif
#ifndef LINKER_H_
#include "linker.h"
#endif
#ifndef GENERATE_CODE_ 
#include "code_generator.h"
#endif
#ifndef L0_GENERATOR_H_
#include "l0_generator.h"
#endif
#ifndef FILESYSTEM_COMPILER_H_
#include "../recc-implementation/filesystem_compiler.h"
#endif

unsigned int make_c_compiler_target(struct build_state *, struct entity *);

#endif
