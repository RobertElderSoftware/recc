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
package OpCPU;

/*  If you want to create your own data packages, just subclass the interface, and you can pass it into the OpCPU Class */

public interface OpCPUDataInterface{
    /*  Returns an array of 2 longs.  The first memeber identifies what type of instruction the second long is. */
    public long [] getData(Long index);
}
