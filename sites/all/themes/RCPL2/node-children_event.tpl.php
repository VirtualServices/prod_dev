<?php
// $Id: node.tpl.php,v 1.4 2007/08/07 08:39:36 goba Exp $
?>
<div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
	<?php print $picture ?>
	<?php if ($page == 0): ?>
		<h1 class="title"><a href="<?php print $node_url ?>"><?php print $title ?></a></h1>
	<?php endif; ?>
	<span class="submitted"><?php print $submitted ?></span>
	<div class="content"><?php print $content ?></div>
	<br />
<b>Location:</b><br />
<?php print RCPL2_taxonomy_links($node, 4); ?>

	<?php if ($links): ?>
			<div class="content"><?php print $links ?></div>
	<?php endif; ?>
	<br>

</div>
