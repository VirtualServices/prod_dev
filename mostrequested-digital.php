<?php

	require('sycom.inc');
	require('feedcreator.class.php');

	# RSS file name
	define('RSS_FILENAME', 'mostrequested-digital');

	# date function

	$date_string = gmstrftime('%Y-%m-%dT%T-08:00', gmmktime());

	# end date function

	$rss = new UniversalFeedCreator();

	# Prep the RSS.

	$rss->title = 'Most requested digital items at the Richland County Public Library';
	$rss->description = 'List of the digital items from our catalog with the most hold requests';
	$rss->link = 'http://hip.myrcpl.com';
	$rss->lastBuildDate = $date_string;
	$rss->pubDate = $date_string;
	$rss->copyright = 'Copyright 2009-2011, Richland County Public Library';

sycom('SET ROWCOUNT 50');

	$sdbh = sycom("select distinct a15.title title,
                a11.author author, 
                a14.processed processed, 
                a12.creation_date creation_day, 
                a13.collection collection,
                a13.item_status status,
                a16.descr descr
from    rv_bib_author a11, 
        rv_bib_control a12, 
        rv_item a13, 
        rv_isbn a14, 
        rv_title_inverted a15, 
        collection a16 
 
where   a11.bib_id = a12.bib_id and 
        a11.bib_id = a13.bib_id and 
        a11.bib_id = a14.bib_id and 
        a11.bib_id = a15.bib_id and 
        a13.collection = a16.collection and 
        (a13.collection in('bd','c','cd','cf','dvd','lebd','ledvd','letb','letbf', 'mp3cd','mp3cdf','ply','romc','tb','tbf','tcd','tcdf','vc','vg')and 
        a12.creation_date between '10/01/2009' and '05/30/2019') and
        a14.processed = (select min(processed) from rv_isbn where bib_id = a11.bib_id) and
        a13.item_status = (select min(item_status) from rv_item where bib_id = a11.bib_id)
        
order by a12.creation_date desc
");

	if($sdbh != NULL)

	{
    	while($srow = sybase_fetch_array($sdbh))
		{   var_dump($srow);
			# Replace '&' characters with '&amp;'
			$srow = str_replace('&', '&amp;', $srow);
		 	/* foreach($srow as $scolumn)
			
			
			{
				# Remove non-text characters

	   			$scolumn = ereg_replace('[^[:xdigit:]]', '', $scolumn); 
				var_dump($scolumn);
			} */
			$srow[3] = preg_replace('/\x1f[a-z]|\x1e/', '', $srow[3]);
			$item = new FeedItem();
			if($srow[3] == '') 
{
	$item->title = 'Title Not Available';
}
else
{
	 $item->title = $srow[3];
}
			$item->link = 'http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&full=3100001~!' . $srow[4].'~!3';
			$item->description = '<img src="http://www.syndetics.com/index.aspx?type=xw12&isbn=' . trim($srow[5]) . '/SC.GIF&client=richlandcpubl" align="left"><p>&nbsp;&nbsp; '.$srow[1] . ' requests,</p><p>&nbsp;&nbsp; ' . $srow[0] . ' copies. </p><p class="clear"></p>';
			$rss->addItem($item);
		}
	}
	$rss->saveFeed('RSS0.92', RSS_FILENAME . '.rss');

?>
