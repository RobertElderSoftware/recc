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

/*  Account for API changes. */
var theRuntime = (typeof chrome.runtime === 'undefined') ? chrome.extension : chrome.runtime;

function chromePluginCloseCurrentChromeTab(){
  if(!(document.getElementById('close-chrome-1234') == null)){
    /*  The constant below is found by loading this unpacked extension, then copying the "ID" field found in the browser extensions list */
    theRuntime.sendMessage(theRuntime.id, {greeting: "hello"}, function(response) {
    });
  }
  setTimeout(chromePluginCloseCurrentChromeTab, 100);
}
chromePluginCloseCurrentChromeTab();
