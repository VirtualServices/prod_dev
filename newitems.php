<?php

	require('sycom.inc');
	require('feedcreator.class.php');

	# RSS file name
	define('RSS_FILENAME', 'newitems');

	# get today's date in correct format for query

	$ret_date = strftime('%e %B %Y', time());

	# date function

	$date_string = gmstrftime('%Y-%m-%dT%T-08:00', gmmktime());

	# end date function

	$rss = new UniversalFeedCreator();

	# Prep the RSS.

	$rss->title = 'Most recently checked-in items at the Richland County Public Library';
	$rss->description = 'Most recently checked-in items in our catalog';
	$rss->link = 'http://www.myrcpl.com';
	$rss->lastBuildDate = $date_string;
	$rss->pubDate = $date_string;
	$rss->copyright = 'Copyright 2008-2010, Richland County Public Library';

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
	$image->url = 'http://www.ylpl.lib.ca.us/images/bookfeed.gif';
	$image->link = 'http://hip.richland.lib.sc.us/catalog_redirect.php?menu=search&submenu=advanced_search&profile=c--2';
	$image->width = 88;
	$image->height = 15;
	$rss->image = $image;
    */
	# Database query
	
	sycom('SET ROWCOUNT 300');

	$sdbh = sycom("select text, bib.bib#, convert(varchar(7),dateadd(dd,cki_date,'1 Jan 1970'),9),convert(varchar(8),dateadd(ss,cki_time,'1 Jan 1970'),14) from bib inner join item on bib.bib# = item.bib# inner join circ_history on item.item# = circ_history.item# and tag='245' and item.item_status='n' and not (text is null");

	if($sdbh != NULL)
	{
    	while($srow = sybase_fetch_assoc($sdbh))
		{
			# Replace '&' characters with '&amp;'
			$srow = str_replace('&', '&amp;', $srow);
			foreach($srow as $scolumn)
			{
				# Remove non-text characters

	   			$scolumn = preg_replace('/\x1f[a-z]|\x1e/', '', $scolumn);
				$scolumn = preg_replace('/[\x00-\x1F\x80-\xFF]/', '', $scolumn);
			}
			$item = new FeedItem();
			$item->title = $srow[0];
			$item->link = 'http://hip.richland.lib.sc.us/catalog_redirect.php?menu=search&profile=c--2&aspect=advanced&index=BIB&term=' . $srow[1];
			$item->description = 'checked in ' . $srow[2] . ' ' . substr($srow[3], 3, 5);
			$rss->addItem($item);
		}
	}
	$rss->saveFeed('RSS0.92', RSS_FILENAME . '.rss');

?>
