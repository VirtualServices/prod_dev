<?php

	require('sycom.inc');
	require('feedcreator.class.php');

	# RSS file name
	define('RSS_FILENAME', 'checkin-w');

	# get today's date in correct format for query

	$ret_date = strftime('%e %B %Y', time());

	# date function

	$date_string = gmstrftime('%Y-%m-%dT%T-08:00', gmmktime());

	# end date function

	$rss = new UniversalFeedCreator();

	# Prep the RSS.

	$rss->title = 'Most recently checked-in items at the Wheatley Branch, Richland County Public Library';
	$rss->description = 'Most recently checked-in items at the Wheatley Branch. Click a title to place a hold request.';
	$rss->link = 'http://www.myrcpl.com';
	$rss->lastBuildDate = $date_string;
	$rss->pubDate = $date_string;
	$rss->copyright = 'Copyright 2009-2011, Richland County Public Library';

	/* These options are not supported by RSS 0.92
	syn => {
		updatePeriod     => "daily",
		updateFrequency  => "1",
		updateBase       => "2005-11-21T12:00:00+00:00",
	}
	*/

	# RSS feed image

	/*$image = new FeedImage();
	$image->title = 'Book Feed';
	$image->url = '';
	$image->link = 'http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp';
	$image->width = 88;
	$image->height = 15;
	$rss->image = $image;
    */
	# Database query
	
sycom('SET ROWCOUNT 50');

	$sdbh = sycom("select distinct bib.text, 
bib.bib#, 
convert(varchar(7),dateadd(dd,cki_date,'1 Jan 1970'),9), 
convert(varchar(8),dateadd(ss,cki_time,'1 Jan 1970'),14), 
cki_location, 
convert(varchar(8),
dateadd(mi,cki_time,'1 Jan 1970'),17), 
processed 'isbn',
item.collection, 
collection.descr
from bib
inner join item on bib.bib# = item.bib# 
inner join circ_history on item.item# = circ_history.item# 
inner join rv_isbn on bib.bib# = rv_isbn.bib_id 
inner join collection on item.collection = collection.collection
and tag='245' 
and cki_location='wh'
and not (text is null) 
and cki_date=datediff(dd,'1 Jan 1970','$ret_date')
and processed = (select min(processed) from rv_isbn where bib.bib# = rv_isbn.bib_id)
order by cki_time desc");

	if($sdbh != NULL)
	{
    	while($srow = sybase_fetch_array($sdbh))
		{var_dump($srow);
			# Replace '&' characters with '&amp;'
			/*$srow = str_replace('&', '&amp;', $srow);*/
			/*foreach($srow as $scolumn)
			{
				# Remove non-text characters

	   			$scolumn = preg_replace('/\x1f[a-z]|\x1e/', '', $scolumn);
				$scolumn = preg_replace('/[\x00-\x1F\x80-\xFF]/', '', $scolumn);
			} */
			$srow[0] = preg_replace('/\x1f[a-z]|\x1e/', '', $srow[0]);
			$srow[0] = ucwords($srow[0]);
			$item = new FeedItem();
			$item->title = trim($srow[0], " /");
			$item->link = 'http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&full=3100001~!' . $srow[1].'~!3';
			$item->description = '<img src="http://www.syndetics.com/index.aspx?type=xw12&isbn=' . trim($srow[6]) . '/SC.GIF&client=richlandcpubl" align="left"><p>Checked in ' . $srow[2] .'<br>at '. $srow[5]. '</p><p><i>'. $srow[8].'</i></p><p class="clear"></p>';
			$rss->addItem($item);
		}
	}
	$rss->saveFeed('RSS0.92', RSS_FILENAME . '.rss');

?>
