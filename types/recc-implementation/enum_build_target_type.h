#ifndef TYPES_enum_build_target_type_H_
#define TYPES_enum_build_target_type_H_
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

enum build_target_type{
	BUILD_TARGET_L2_FILE,
	BUILD_TARGET_L1_FILE,
	BUILD_TARGET_L0_FILE,
	BUILD_TARGET_C_FILE,
	BUILD_TARGET_PREPROCESSED_FILE,
	BUILD_TARGET_FILESYSTEM_IMPLEMENTATION,
	BUILD_TARGET_SYMBOL_FILE
};

#endif
