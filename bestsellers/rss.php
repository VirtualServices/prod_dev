<? echo '<?xml version="1.0" encoding="ISO-8859-1" ?>'; ?>

<rss version="2.0" xmlns:media="http://search.yahoo.com/mrss/">
<?
include("config.inc.php");

	if($_GET['NYT_TYPE']=="fiction"){
		$titlez = "fiction";
	}else if($_GET['NYT_TYPE']=="nonfiction"){
		$titlez = "nonfiction";
	}else if($_GET['NYT_TYPE']=="children"){
		$titlez = "children\'s";
	}
?>
  <channel>
  <title>New York Times Bestseller List <?=$titlez?> Hardback</title>
  <link>http://www.myrcpl.com/</link>
  <description>New York Times Bestseller List <?=$titlez?> Hardback for <?=$dataz?></description> 
  <language>en-us</language> 
<?
if($_GET['NYT_TYPE']){

	if($_GET['NYT_TYPE']=="fiction"){
		$source = NYT_FICTION_URL;
	}else if($_GET['NYT_TYPE']=="nonfiction"){
		$source = NYT_NONFICTION_URL;
	}else if($_GET['NYT_TYPE']=="children"){
		$source = NYT_CHILDREN_URL;
	}

$dataz = grabData($source);

echo   "<pubDate>".$dataz['listDate']."</pubDate>\n";
echo   "<lastBuildDate>".$dataz['listDate']."</lastBuildDate>\n"; 

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
	echo "<item>\n" .
	"\t<title><![CDATA[".$item['title']."]]></title>\n" .
	"\t<description><![CDATA[".$item['author']."]]></description>\n" .
//	"\t<category domain=![CDATA[http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?menu=search&aspect=basic_search&npp=20&ipp=20&spp=20&profile=int&ri=1&index=.NW&term=".$item['author']."]]><![CDATA[".$item['author']."]]></category>\n" .
	"\t<isbn><![CDATA[".$item['isbn']."]]></isbn>\n" . 
	"\t<link><![CDATA[http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?menu=search&aspect=basic_search&profile=int&index=ISBNEX&term=".$item['isbn']."]]></link>\n" . 
	"\t<rank><![CDATA[".$i."]]></rank>\n" . 

	"\t<guid isPermaLink='false'>".$item['isbn']."</guid>\n" . 
	"</item>\n";
	$i++;
	}

}
?>
</channel>
</rss>
