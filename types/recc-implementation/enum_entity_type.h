#ifndef TYPES_enum_entity_type_H_
#define TYPES_enum_entity_type_H_
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

enum entity_type{
	ENTITY_TYPE_GENERATED_C_CODE,
	ENTITY_TYPE_SOURCE,
	ENTITY_TYPE_L2_FILE,
	ENTITY_TYPE_L1_FILE,
	ENTITY_TYPE_L0_FILE,
	ENTITY_TYPE_C_FILE,
	ENTITY_TYPE_OBJECT_FILE,
	ENTITY_TYPE_PREPROCESSED_FILE,
	ENTITY_TYPE_FILESYSTEM_IMPLEMENTATION,
	ENTITY_TYPE_SYMBOL_FILE
};

#endif
