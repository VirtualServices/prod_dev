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
  <?php else: ?>
		<p align="center">###</p>

<p align="center">The mission of the Richland County Public Library is to provide experiences that Inspire, Inform and Entertain.</p>
</div>
	<?php if ($links): ?>
		<div class="line">
			<div class="links">&raquo; <?php print $links ?></div>
		</div>
	<?php endif; ?>
	<hr><span class="content"></span>
	<div class="taxonomy"><?php print $terms ?></div><br />
  <?php endif; ?>
 </div>