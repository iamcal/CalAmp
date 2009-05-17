<?
	include('lib.txt');

	stop_overruns();

	$current = mysql_fetch_array(mysql_query("SELECT * FROM amp_log WHERE date_stop=0 ORDER BY date_start DESC LIMIT 1", $db));
	$result = mysql_query("SELECT * FROM amp_log WHERE date_stop > 0 ORDER BY date_start DESC LIMIT 10", $db);

	$home = 1;
	include('head.txt');
?>

<table cellpadding="4" cellspacing="2" border="0">
	<tr class="entry">
		<td><b>Track</b></td>
		<td><b>Artist</b></td>
		<td><b>Album</b></td>
		<td><b>Last Played</b></td>
		<td><b>Played</b></td>
	</tr>
	<tr class="heading">
		<td colspan="5"><b>Currently Playing:</b></td>
	</tr>
<?
	if ($current[id]){
		insert_track_row($current);
	}else{
?>
	<tr class="entry">
		<td colspan="5"><i>none</i></td>
	</tr>
<?
	}
?>
	<tr class="heading">
		<td colspan="5"><b>Recently Played:</b></td>
	</tr>
<?
	while($row = mysql_fetch_array($result)){
		insert_track_row($row);
	}
?>
</table>

<?
	include('foot.txt');
?>
