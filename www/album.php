<?
	include('lib.txt');

	stop_overruns();

	$album = AddSlashes($_GET[album]);

	$result = mysql_query("SELECT *, MAX(date_start) AS date_start2 FROM amp_log WHERE album LIKE '$album' GROUP BY track ORDER BY date_start2 DESC", $db);

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
		<td colspan="5"><b>Album entries for &quot;<i><?=StripSlashes(htmlentities($album))?></i>&quot;:</b></td>
	</tr>
<?
	while($row = mysql_fetch_array($result)){
		$row[date_start] = $row[date_start2];
		insert_track_row($row, 0, 0, 1);
	}
?>
</table>

<?
	include('foot.txt');
?>
