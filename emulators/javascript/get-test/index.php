<?php 
/*
    Copyright 2015 Robert Elder Software Inc.
    
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

if(isset($_REQUEST['test_name'])){

	$test_name = $_REQUEST['test_name'];

        //  TODO:  this should be safer if the endpoint is ever on a server
	$test_code = file_get_contents(realpath(dirname(__FILE__))."/../../../$test_name.l0.js");

	if($test_code === false){
		$response_object['error'] = "Test with name $test_name was not found.  Did you assemble the test case?  This can be caused by a permission issue sometimes.";
                echo "jsonpCallback(".json_encode($response_object).");";
	}else{
		echo $test_code;
	}
}else{
	$response_object['error'] = "Test name was not set.";
        echo "jsonpCallback(".json_encode($response_object).");";
}


?>
