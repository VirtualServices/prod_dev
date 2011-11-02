<?php
// $Id: block.tpl.php,v 1.2 2007/08/07 08:39:36 goba Exp $
?>

<div class="<?php print "block block-$block->module" ?>" id="<?php print "block-$block->module-$block->delta"; ?>">
	<table width="100%" class="box">
	  <tr>
		<td width="100%" class="block-top">
			<div class="title"><h3><?php print $block->subject ?></h3></div>
		</td>
	  </tr>
	  <tr>
		<td width="100%" class="block-mid">
			<div class="block-m">
				<div class="content"><?php print $block->content ?></div>
			</div>
		</td>
	  </tr>
	  <tr>
		<td width="100%" class="block-bot"><img alt="" src="<?=base_path().path_to_theme()?>/images/spacer.gif" width="1" height="8" /></td>
	  </tr>
	</table>
</div> 