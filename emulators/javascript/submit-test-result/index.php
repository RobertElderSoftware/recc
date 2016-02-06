<?php 
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

$response_object = Array();

if(isset($_REQUEST['test_name']) && isset($_REQUEST['test_result'])){

	$test_name = $_REQUEST['test_name'];

        //  TODO:  this should be safer if the endpoint is ever on a server
	$result = file_put_contents(realpath(dirname(__FILE__))."/../../../{$test_name}.jsoutput",urldecode($_REQUEST['test_result']));

        if($result === false){
		$response_object['error'] = "Failed to submit test case $test_name.  Are the permissions set on the 'test' directory?";
	}else{
		$response_object['success'] = "Success";
	}
}else{
	$response_object['error'] = "test_name or test_result was not set.";
}

echo json_encode($response_object);

?>
