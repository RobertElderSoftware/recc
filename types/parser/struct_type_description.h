

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

/*GETS_REPLACED_WITH_INCLUDES*/

/*  A type description is used to keep track of the abstract idea of a single specific 'type' in C 
    as you would see it on any variable that had been declared.  This type description is used to check
    for compatibility between two types, or as a starting point for modifying a type into related types
    (such as pointed to by, address type, array type etc.).  Because it is used to keep track of what types
    are stored in the programs stack (at compile time), it also describes whether a value is an LVALUE or
    RVALUE.  In addition, for types declared as anonymous enums, structs or untions, we need a reference
    to the normalized declaration element that it was declared from.  Also, because non-file scoped typedefs
    will have a local scope to the block that they are declared in, a type description will also need to
    know what scope level it belongs to.  A context variable is also used to keep track of where the type
    description was created.
*/

struct type_description{
        struct normalized_declaration_element * source_element; /* Needed for anonymous struct/union/enum */
        struct normalized_declarator * declarator;
        struct struct_normalized_specifier_ptr_list * specifiers;
        struct parser_node * context;
        struct scope_level * source_scope_level;
        enum value_type value_type;
        unsigned int pad;
};
