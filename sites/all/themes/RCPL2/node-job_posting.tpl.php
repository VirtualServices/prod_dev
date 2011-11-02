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
  <p></p>
  <hr>
	<p></p>	<p><b>TO APPLY:</b> Submit a completed <a href="/pdf/EmpApp.pdf">RCPL employment application</a> at any RCPL location or to Personnel Office, RCPL, 1431 Assembly St., Columbia, SC 29201. Specify both the job title and the vacancy number on application. 
</p>
<h1 align="center">EQUAL OPPORTUNITY EMPLOYER</h1>
<p align="center"><font size="-1">1431 Assembly St  Columbia, South Carolina 29201<br> [phone] 803.799.9084  [fax] 803.929.3448  <a href="http://www.myrcpl.com">www.myRCPL.com</a></font></p>
</div>
	<?php if ($links): ?>
		<div class="line">
			<div class="links">&raquo; <?php print $links ?></div>
		</div>
	<?php endif; ?>
	<hr><span class="content">See Related Categories: </span>
	<div class="taxonomy"><?php print $terms ?></div><br />
  <?php endif; ?>
 </div>