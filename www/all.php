<?
	include('lib.txt');

	stop_overruns();

	$result = mysql_query("SELECT * FROM amp_log GROUP BY trackname ORDER BY date_start DESC", $db);

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
		<td colspan="5"><b>All entries:</b></td>
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
