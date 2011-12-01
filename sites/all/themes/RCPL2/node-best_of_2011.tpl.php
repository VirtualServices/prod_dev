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
	<?php if ($links): ?>
		<div class="line">
			<div class="links">&raquo; <?php print $links ?></div>
		</div>
	<?php endif; ?>
	<br>
	<!--<hr>
	<span class="content">Related Pages: <span class="taxonomy"><?php print $terms ?></span><br /></span>-->
<?php print check_plain($node->title) ?>
<?php print $node->name ?>
<?php print $node->picture ?><?php print $node->field_book_cover_image[0]['url'] ?><?php print $node->field_best_of_checkouts[0]['view'] ?><?php print $node->print_display_comment ?>
<?php print $node->links['statistics_counter']['title'] ?>



<?php print $node->links['comment_add']['attributes']['title'] ?><?php print $node->links['comment_add']['href'] ?><?php print $node->content['#children'] ?>

<?php print $node->content['fivestar_widget']['#title'] ?>
<?php print $node->content['fivestar_widget']['#description'] ?>
<?php print $node->content['field_best_of_checkouts']['#children'] ?>
</div>
