<?php

	require('sycom.inc');
	require('feedcreator.class.php');

	# RSS file name
	define('RSS_FILENAME', 'mostrequested-lp');

	# date function

	$date_string = gmstrftime('%Y-%m-%dT%T-08:00', gmmktime());

	# end date function

	$rss = new UniversalFeedCreator();

	# Prep the RSS.

	$rss->title = 'Most requested large print books at the Richland County Public Library';
	$rss->description = 'List of the large print books from our catalog with the most hold requests.';
	$rss->link = 'http://hip.myrcpl.com';
	$rss->lastBuildDate = $date_string;
	$rss->pubDate = $date_string;
	$rss->copyright = 'Copyright 2009-2011, Richland County Public Library';

sycom('SET ROWCOUNT 50');

	$sdbh = sycom("select 
convert(varchar(3), (select count(*) from item i where i.bib# = t.bib#)) 'copies',
(Select count(*) from request r where r.bib# = t.bib# having count(*)>1) 'reqs',
t.bib# 'BIB',
ti.processed 'title', 
t.bib#, 
isbn.processed 'isbn' 
from bib t, title ti, rv_isbn isbn 
where t.bib# = ti.bib# 
and t.bib# = isbn.bib_id 
and t.tag='245' 
and t.bib# in (Select bib# from request) 
and convert(int, (Select count(*) from item i where i.bib# = t.bib# having count(*)>0)) < convert(int, (Select count(*) from request r where r.bib# = t.bib#))
and isbn.processed = (select min(processed) from rv_isbn where bib_id = t.bib#)
and t.bib# in (Select bib# from item_with_title where collection in ('lelp','lp','lpb','lpf','lpm','lpsf','lpw')) 
order by Reqs desc
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
