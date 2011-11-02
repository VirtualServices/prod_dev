<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">

<html>
<head>
	<title>Untitled</title>
</head>

<body>
<?php
$XMLFILE = "http://www.myrcpl.com/mostRecentlyCheckedInRSS.rss";
$TEMPLATE = "rsstohtml/template.html";
$MAXITEMS = "20";
$ALLOWXMLCACHE = "1";
$XMLCACHETTL = "86400";
$OUTCACHETTL = "86400";
$OUTCACHENAME = "http://www.myrcpl.com/onorder.htm";
include("rsstohtml/rss2html.php");
?>


</body>
</html>
