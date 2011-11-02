<?php
// $Id: page.tpl.php,v 1.25 2008/01/24 09:42:53 goba Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <title><?php print $head_title ?></title>
 <script src="http://www.libanywhere.com/check.js?auto=1" type="text/javascript"></script> <meta http-equiv="Content-Style-Type" content="text/css" />
  <?php print $head ?>
  <?php print $styles ?>
  <?php print $scripts ?>
</head>

<body>
	<table id="main" align="center" border="0">
	<!-- Top row, navigation and search box -->
	  <tr>
		<td width="100%" colspan="3">
			<table width="930" style="height:30px;">
			  <tr>
				<td width="725">
			<?php if (isset($primary_links)) : ?>
				<div class="pr-menu">
					<?php print theme('links', $primary_links, array('class' => 'links primary-links')) ?>
				</div>
			<?php endif; ?>
				</td>
				<td width="205"><div id="edit-search-theme-form-1-wrapper" class="form-item">
<form action="/search/"  accept-charset="UTF-8" method="post" id="search-form" class="search-form">
<input type="text" maxlength="128" name="keys" onClick="if (this.value == '...') { this.value = ''; } " id="edit-keys" value="" title="Enter the terms you wish to search for." class="form-text" />
<input type="submit" name="op" class="form-submit" value="Search" />
<input type="hidden" name="form_build_id" id="form-3551ff463130f15eb1e514c55e1a86d9" value="form-3551ff463130f15eb1e514c55e1a86d9"  />
<input type="hidden" name="form_token" id="edit-search-form-form-token" value="<?=drupal_get_token('search_form');?>"  />
<input type="hidden" name="form_id" id="edit-search-form" value="search_form"  />
</form>
							</div>
						</td>
					  </tr>
			 </table>
		</td>
	  </tr>
	  <!-- Second row, logo and image -->
	  <tr>
	  	<td width="192" id="logo">
			  	<a href="/"><img src="/sites/all/themes/RCPL2/images/logo.gif" width="180" height="195" border="0" alt="Richland County Public Library"></a></td>
				<td width="515" class="headerimage"><?php print $headerimage ?></td>
				<td  width="192" class="featureimage"><?php print $feature ?></td>
	  </tr>
	  <tr>
    		<td>
					<div id="left2">
						<?php if ($left != ""): ?>
						<table width="100%">
						  <tr>
							<td width="100%">
								<?php print $left ?>
							</td>
						  </tr>
						</table>
						<?php endif; ?>
					</div>
				</td>
				<td class="cent">
					<?php if (isset($welcome)) : ?>
										<div id="welcome"><?php print $welcome ?></div>
					<?php endif; ?>
					<div class="tall-l4">
						<div class="tall-r4">
							<div class="tall-t4">
								<div class="tall-b4">
									<div class="k-tl4">
										<div class="k-tr4">
											<div class="k-bl4">
												<div class="k-br4">
													<?php if ($mission != ""): ?>
													  <div id="mission"><?php print $mission ?></div>
													<?php endif; ?>
																
													<?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
													<?php if ($title): print '
														<h2'. ($tabs ? ' class="with-tabs title"' : '') .'>'. $title .'</h2>
													'; endif; ?>
													<?php if ($tabs): print '<ul class="tabs primary">'. $tabs .'</ul></div>'; endif; ?>

																	 
													<?php if ($show_messages && $messages != ""): ?>
														  <?php print $messages ?>
													<?php endif; ?>
												
													<?php if ($help != ""): ?>
														<div id="help"><?php print $help ?></div>
													<?php endif; ?>												
													  <!-- start main content -->
													  <?php print $breadcrumb ?>
													  <?php print $content; ?>
													  <?php print $feed_icons ?>
												</div>
											</div>
										</div>
									</div>
								</div>
							</div>
						</div>
					</div>
				</td>
				<td>
					<div id="right2">
						<?php if ($right != ""): ?>
						<table width="100%">
						  <tr>
							<td width="100%">
								<?php print $right ?>
							</td>
						  </tr>
						</table>
						<?php endif; ?>
					</div>
				</td>
	  </tr>
	</table>
	<div id="footer">
		<div class="foot">
			<?php if ($footer_message || $footer) : ?>
			<?php print $footer ?>
				<?php print $footer_message;?>
				<?php endif; ?>
		</div>
	</div>
	<?php print $closure;?>
	<script type="text/javascript">
var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
</script>
<script type="text/javascript">
try {
var pageTracker = _gat._getTracker("UA-3434445-1");
pageTracker._trackPageview();
} catch(err) {}</script>
</body>
</html>
