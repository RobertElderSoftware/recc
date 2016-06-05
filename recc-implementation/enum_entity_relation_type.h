#ifndef enum_entity_relation_type_H_
#define enum_entity_relation_type_H_
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

enum entity_relation_type{
	RELATION_TYPE_DEPENDS_ON_AT_BUILD_TIME,
	RELATION_TYPE_IS_DEPENDED_ON_AT_BUILD_TIME_BY,
	RELATION_TYPE_CONTAINS,
	RELATION_TYPE_IS_CONTAINED_BY,
	RELATION_TYPE_INCLUDES,
	RELATION_TYPE_IS_INCLUDED_BY
};

#endif
