<?php

	require('sycom.inc');
	require('feedcreator.class.php');

	# RSS file name
	define('RSS_FILENAME', 'onorder-blu-ray');

	# get today's date in correct format for query

	$ret_date = strftime('%e %B %Y', time());

	# date function

	$date_string = gmstrftime('%Y-%m-%dT%T-08:00', gmmktime());

	# end date function

	$rss = new UniversalFeedCreator();

	# Prep the RSS.

	$rss->title = 'Most recently ordered Blu-ray discs at the Richland County Public Library';
	$rss->description = 'Most recently ordered Blu-ray discs for our collection.  Some items show duplicates because they have more than one isbn number.  Click on a title to place a hold request on that item.';
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
	$image->link = 'http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp';
	$image->width = 88;
	$image->height = 15;
	$rss->image = $image;
    */
	# Database query
	
sycom('SET ROWCOUNT 50');

	$sdbh = sycom("select distinct a15.title title, a11.author author, a14.processed  processed, a12.creation_date  creation_day, a13.collection  collection, a16.descr  descr
from rv_bib_author a11, rv_bib_control a12, rv_item   a13, rv_isbn   a14, rv_title_inverted  a15, collection  a16 where a11.bib_id = a12.bib_id and a11.bib_id = a13.bib_id and a11.bib_id = a14.bib_id and a11.bib_id = a15.bib_id and a13.collection = a16.collection and (a13.collection in ('bd','lebd')and (a14.processed IS NOT NULL) and a12.creation_date between '01/01/2009' and '05/30/2019') order by a12.creation_date desc");

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
			$item->link = ' http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?&index=ISBNEX&term=' . trim($srow[2]).'&profile=int';
			$item->description = '<img src="http://www.syndetics.com/index.aspx?type=xw12&isbn=' . $srow[2] . '/SC.GIF&client=richlandcpubl" align="left"><p>&nbsp;&nbsp;<i>' .'</i><br> &nbsp; Ordered on-' .substr($srow[3], '0', '6'). '<p class="clear"></p>' ;
			$rss->addItem($item);
		}
	}
	$rss->saveFeed('RSS0.92', RSS_FILENAME . '.rss');

?>
