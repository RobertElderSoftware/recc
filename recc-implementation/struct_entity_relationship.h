#ifndef TYPES_struct_entity_relationship_H_
#define TYPES_struct_entity_relationship_H_
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
#ifndef enum_entity_relation_type_H_
#include "enum_entity_relation_type.h"
#endif

struct entity_relationship{
	struct entity * entity;
	enum entity_relation_type type;
};

#endif
