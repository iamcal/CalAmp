<?
	include("lib.txt");

	$time = time();
	$track = urldecode($HTTP_SERVER_VARS[QUERY_STRING]);

	stop_overruns();
	stop_current();

	if ($track){
		insert_track($track);
	}

	echo "ok";
?>