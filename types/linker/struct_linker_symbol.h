

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

struct linker_symbol{
	unsigned int original_l2_item_index;
	unsigned int is_implemented;
	unsigned int is_required;
	unsigned int is_external;
	struct l2_item * l2_item; /* The pointer to the l2_item of the instruction in its parent linker object. */
	unsigned int observed_as_implemented;  /*  indicates if a corresponding label for that linker object has been found in the file */
	unsigned int pad;
	unsigned int id;
	struct linker_file * parent_linker_file;  /*  External symbols need to remember which linker object they belong to. */
	unsigned char * identifier;
};
