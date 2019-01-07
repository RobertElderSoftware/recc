<?php  
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
 
$response_object = Array();

$all_files = Array();
$location = realpath(dirname(__FILE__))."/../../../test/tests_to_run";
$data = file_get_contents($location);
if($data === false){
	$response_object['error'] = "Unable to find test cases in ".$location."  Could it be a permission problem?";
}else{
	$all_files = array_merge($all_files, explode(' ', str_replace("\n",'',str_replace("\r",'',$data))));
}
$response_object['available_tests'] = array_values($all_files);

echo json_encode($response_object);

?>
