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

int main(int, char * []);
int main_argv_indirect(void);

int main_argv_indirect(void){
	/*  Call the 'real' main method with arguments */
	int argc = 1;
	char * argv[2];
	argv[0] = (char *)"./test/c89/main_with_parameters.exec"; /*  Currently used for a specific unit test. */
	argv[1] = (char *)0;
	return main(argc, argv);
}
