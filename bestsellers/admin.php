<?
include("config.inc.php");
?>
<fieldset>
<legend>New York Times Bestseller List Fiction Hardback</legend>
<?
$dataz = grabData(NYT_FICTION_URL);
echo "<table>\n";
echo "<tr><td>".$dataz['listDate']."</td></tr>\n";
		

$i=0;
	foreach($dataz['infoz'] as $item){
		$itemsXml[$i] = ItemLookupReturnArray($item);
		$i++;
	}


//Sorting by rank filed
//foreach($itemsXml as $res){ $sortAux[] = $res['rank']; }
//array_multisort($sortAux, SORT_ASC, $itemsXml);
//
$i=1;
foreach($itemsXml as $item){
	echo "<tr><td>";
		echo "<table cellpadding='10' cellspacing='10'>";
			echo "<tr>";
				echo "<td><img src='http://www.syndetics.com/index.aspx?type=xw12&isbn=".$item['isbn']."/SC.GIF&client=richlandcpubl'></td>";
				echo "<td>";
					echo "<table>";
						echo "<tr>";
							echo "<td>Title </td>";
							echo "<td><a href='http://hip.richland.lib.sc.us/ipac20/ipac.jsp?menu=search&aspect=basic_search&npp=20&ipp=20&spp=20&profile=int&ri=&index=.EK&term=".$item['isbn']."&x=12&y=17&aspect=basic_search' target='_blank'>".$item['title']."</a></td>";
						echo "</tr>";
						echo "<tr>";
							echo "<td>Author </td>";
							echo "<td><a href='http://hip.richland.lib.sc.us/ipac20/ipac.jsp?menu=search&aspect=basic_search&npp=20&ipp=20&spp=20&profile=int&ri=1&index=.NW&term=".$item['author']."'>".$item['author']."</a></td>";
						echo "</tr>";
						echo "<tr>";
							echo "<td>ISBN </td>";
							echo "<td>".$item['isbn']."</td>";
						echo "</tr>";
						echo "<tr>";
							echo "<td>Rank </td>";
							echo "<td>".$i."</td>";
						echo "</tr>";
					echo "</table>";
				echo "</td>";
			echo "</tr>";
		echo "</table>";
	echo "</td></tr>\n";
	$i++;
}
echo "</table>";
?>
</fieldset>

<br/><br/>
<fieldset>
<legend>New York Times Bestseller List Non-Fiction Hardback</legend>
<?
unset($sortAux);
unset($itemsXml);

$dataz = grabData(NYT_NONFICTION_URL);
echo "<table>\n";
echo "<tr><td>".$dataz['listDate']."</td></tr>\n";
		

$i=0;
	foreach($dataz['infoz'] as $item){
		$itemsXml[$i] = ItemLookupReturnArray($item);
		$i++;
	}


//Sorting by rank filed
//foreach($itemsXml as $res){ $sortAux[] = $res['rank']; }
//array_multisort($sortAux, SORT_ASC, $itemsXml);
//
$i=1;
foreach($itemsXml as $item){
	echo "<tr><td>";
		echo "<table>";
			echo "<tr>";
				echo "<td><img src='http://www.syndetics.com/index.aspx?type=xw12&isbn=".$item['isbn']."/SC.GIF&client=richlandcpubl'></td>";
				echo "<td>";
					echo "<table>";
						echo "<tr>";
							echo "<td>Title</td>";
							echo "<td><a href='http://hip.richland.lib.sc.us/ipac20/ipac.jsp?menu=search&aspect=basic_search&npp=20&ipp=20&spp=20&profile=int&ri=&index=.EK&term=".$item['isbn']."&x=12&y=17&aspect=basic_search' target='_blank'>".$item['title']."</a></td>";
						echo "</tr>";
						echo "<tr>";
							echo "<td>Author</td>";
														echo "<td><a href='http://hip.richland.lib.sc.us/ipac20/ipac.jsp?menu=search&aspect=basic_search&npp=20&ipp=20&spp=20&profile=int&ri=1&index=.NW&term=".$item['author']."'>".$item['author']."</a></td>";
						echo "</tr>";
						echo "<tr>";
							echo "<td>ISBN</td>";
							echo "<td>".$item['isbn']."</td>";
						echo "</tr>";
						echo "<tr>";
							echo "<td>Rank</td>";
							echo "<td>".$i."</td>";
						echo "</tr>";
					echo "</table>";
				echo "</td>";
			echo "</tr>";
		echo "</table>";
	echo "</td></tr>\n";
	$i++;
}


echo "</table>";
?>
</fieldset>

