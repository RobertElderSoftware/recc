/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
<?php  
 
$response_object = Array();

$files = scandir('/home/robert/git-projects/os/test/');

if($files === false){
	$response_object['error'] = "Unable to find test cases.";
}else{
	$files = array_filter($files, function($a) { return ($a != "." && $a != ".." && preg_match('/\.c$/', $a));});

	$files = array_map(function($a) { $s = explode(".",$a); return $s[0]; }, $files);

	$response_object['available_tests'] = array_values($files);
}

echo json_encode($response_object);

?>
