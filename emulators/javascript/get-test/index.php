<?php 
/*
	Copyright 2015 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
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
