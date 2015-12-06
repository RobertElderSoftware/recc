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
