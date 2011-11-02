<?php
// $Id: node.tpl.php,v 1.4 2007/08/07 08:39:36 goba Exp $
?>
<script src="http://www.librarything.com/forlibraries/widget.js?id=53-1547219295" type="text/javascript"></script><noscript>This page contains enriched content visible when JavaScript is enabled or by clicking <a href="http://www.librarything.com/forlibraries/noscript.php?id=53-1547219295&accessibility=1">here</a>.</noscript>
<div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
	<?php print $picture ?>
	<?php if ($page == 0): ?>
		<h1 class="title"><a href="<?php print $node_url ?>"><?php print $title ?></a></h1>
	<?php endif; ?>
	<div class="content"><?php print $content ?>
	  <?php if ($teaser): ?>
    Read full review: <a href="<?php print $node_url ?>"><?php print $title ?></a></div>
  <?php else: ?>
		  <table><tr><td width="50%" valign="top">
		  <fieldset><div id="ltfl_similars" class="ltfl"></div></fieldset></td>
		  <td width="50%" valign="top">
		  <fieldset><div id="ltfl_tagbrowse" class="ltfl"></div></fieldset></td></tr></table>

	<?php if ($links): ?>
		<div class="line">
			<div class="links">&raquo; <?php print $links ?></div>
		</div>
	<?php endif; ?>
	<hr><span class="content">See Related Categories: </span>
	<div class="taxonomy"><?php print $terms ?></div><br />
  <?php endif; ?>
 </div>