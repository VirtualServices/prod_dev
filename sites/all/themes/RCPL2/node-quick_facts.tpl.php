<?php
// $Id: node.tpl.php,v 1.4 2007/08/07 08:39:36 goba Exp $
?>
<div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
	<?php print $picture ?>
	<?php if ($page == 0): ?>
		<h1 class="title"><a href="<?php print $node_url ?>"><?php print $title ?></a></h1>
	<?php endif; ?>
	<div class="content"><?php print $content ?>
	  <?php if ($teaser): ?>
    Read full description: <a href="<?php print $node_url ?>"><?php print $title ?></a></div>
  <?php else: ?>
<a href="/quick-facts/home">See all Quick Facts</a>
	<?php if ($links): ?>
		<div class="line">
			<div class="links">&raquo; <?php print $links ?></div>
		</div>
	<?php endif; ?>

  <?php endif; ?>
 </div>