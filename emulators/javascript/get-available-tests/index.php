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

$directories = Array(
	"c89/",
	"c99/"
);

$all_files = Array();

foreach($directories as $directory){
	$dir = realpath(dirname(__FILE__))."/../../../test/".$directory;
	$current_files = scandir($dir);
	if($current_files === false){
		$response_object['error'] = "Unable to find test cases in ".$dir;
		break;
	}
	$current_files = array_filter($current_files, function($a) { return ($a != "." && $a != ".." && preg_match('/\.c$/', $a));});
	$current_files = array_map(function($a) { global $directory; $s = explode(".",$a); return $directory.$s[0]; }, $current_files);
	$all_files = array_merge($all_files, $current_files);
}

$response_object['available_tests'] = array_values($all_files);

echo json_encode($response_object);

?>
