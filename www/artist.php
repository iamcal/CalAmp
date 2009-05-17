<?
	include('lib.txt');

	stop_overruns();

	$artist = AddSlashes($_GET[artist]);

	$result = mysql_query("SELECT *, MAX(date_start) AS date_start2 FROM amp_log WHERE artist LIKE '$artist' GROUP BY track ORDER BY date_start2 DESC", $db);

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
		<td colspan="5"><b>Artist entries for &quot;<i><?=StripSlashes(htmlentities($artist))?></i>&quot;:</b></td>
	</tr>
<?
	while($row = mysql_fetch_array($result)){
		$row[date_start] = $row[date_start2];
		insert_track_row($row, 0, 1);
	}
?>
</table>

<?
	include('foot.txt');
?>
