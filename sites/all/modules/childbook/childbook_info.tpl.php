<?php
module_load_include('inc','childbook','childbook');
$el=unserialize($elements);

if ($mtype==1) {
	if ($el[1]) {
		$childbook_image='<img src="'.get_image_url($catalog).'" alt="image" class="library_img" title="'.get_sumarry($catalog).'" />';
		$childbook_image_2='<img style="float:left; margin:2px;" class="library_img" src="'.get_image_url($catalog).'" alt="image" />';
	}
	if ($el[2]) {
		$childbook_summary=get_sumarry($catalog); $sum=1;
	}
	if ($el[3]) {
		$childbook_title=get_title($catalog); $titl=1;
	}
}

if ($mtype==2) {
	$catalog=explode('@',$catalog);
	$childbook_image='<img class="library_img" src="'.get_img_amazon($catalog[1]).'" alt="image" />';
	if ($el[3]) { 
		$childbook_title=get_title_2($catalog[0]); $titl=1;
	}
}

if ($mtype==3) {
		$catalog=explode('@',$catalog);
	$childbook_image='<img class="library_img" src="'.get_image_magazine($catalog[1]).'" alt="image" />';
	if ($el[3]) { 
		$childbook_title=get_title($catalog[0]); $titl=1;
	}
}

?>
<?php
if($mtype==1){
if ($position==1) {
?>
<div style="float:left; margin:2px;" class="library_info">
<?php print '<a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=.ek&term='.$catalog.'">'.$childbook_image_2.'</a>'; ?>
</div><br />
<?php
}
if ($position==2) {
?>
<div style="float:left; margin:2px;" class="library_info">
<?php print '<h3><a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=.ek&term='.$catalog.'">'.$childbook_title.'</a></h3>'; ?>
<?php print '<a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=.ek&term='.$catalog.'">'.$childbook_image.'</a>'; ?>
</div><br />
<?php
}
if ($position==3) {
?>
<div style="float:left; margin:2px;" class="library_info">
<?php print '<h3 class="library_title"><a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=.ek&term='.$catalog.'">'.$childbook_title.'</a></h3>'; ?>
<?php print '<a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=.ek&term='.$catalog.'">'.$childbook_image_2.'</a>'; ?>
<div class="library_sum"><?php print get_sumarry($catalog) ?></div>
</div><br />
<?php
}
if ($position==4) {
?>
<div style="float:left; margin:2px;" class="library_info">
<?php print '<a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=.ek&term='.$catalog.'">'.$childbook_image.'</a>'; ?>
<?php print '<br /><h3><a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=.ek&term='.$catalog.'">'.$childbook_title.'</a></h3>'; ?>
</div><br />
<?php
}
}
if ($mtype==2) {
?>
<div style="float:left; margin:2px;" class="library_info">
<?php print '<h3 class="library_title"><a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?index=OA&term='.$catalog[0].'">'.$childbook_title.'</a></h3>'; ?>
<?php print '<a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?&index=OA&term='.$catalog[0].'">'.$childbook_image.'</a>'; ?>
</div>
<?php
}
if ($mtype==3) {
?>
<div style="float:left; margin:2px;" class="library_info">
<?php print '<h3 class="library_title"><a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=.ek&term='.$catalog[0].'">'.$childbook_title.'</a></h3>'; ?>
<?php print '<a href="http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=.ek&term='.$catalog[0].'">'.$childbook_image.'</a>'; ?>
</div>
<?php
}
?>