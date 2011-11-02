<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
<head>
	<title>Untitled</title>
</head>

<body>
<?php
/*
* [RPORT] This PHP snippet displays NODE-X where the node meets the criteria;
* n.promote = '1' - this is a node that is promoted to the front page AND;
* n.sticky = '1' - this is a node that is marked as sticky.
*/
$list_qty = 2;
$sql = "SELECT n.nid, n.vid, n.type, n.created, r.body, r.format FROM {node} n INNER JOIN {node_revisions} r ON r.vid = n.vid WHERE n.promote = '1' AND n.sticky = '1' ORDER BY n.created DESC";
      
$result = db_query_range(db_rewrite_sql($sql), 0, $list_qty);
      
while ($node = db_fetch_object($result)) {
  // print your markup here
  print check_markup($node->body, $node->format, false);
}
?>

</body>
</html>
