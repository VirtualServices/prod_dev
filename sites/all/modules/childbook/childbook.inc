<?php
// $Id$
/**
 * Template to display children's book nodes.
 */

//Returns the image url for books
function get_image_url($id) {
	$url="http://www.syndetics.com/index.aspx?type=xw12&client=richlandcpubl&isbn=".$id."/SC.GIF";
	return $url;
}

function get_book_url($id){
	//Forming the link
  $term = $id;
  $link = "http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=ISBNEX&term=".$term;
  return $link;	
}

//Returns ths ISBN number
function get_isbn_number($id) {
//Forming the link
  $term = $id;
  $link = "http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=ISBNEX&term=".$term;

//Getting the content
  $continut = file_get_contents($link);

//Parsing
  $aux1 = explode('<a class="normalBlackFont1">ISBN:&nbsp;</a></td><td valign="top" width="99%"><table class="tableBackground" cellpadding="0" cellspacing="0"><tr><td valign="top"><a class="normalBlackFont1">', $continut);
  $aux2 = explode('</a>', $aux1[1]);
  $isbn = $aux2[0];
  $isbn = str_replace('<font style="background-color: rgb(255, 255, 128);">', '', $isbn);
  $isbn = str_replace('<font style="background-color: #FFFF80">', '', $isbn);
  $isbn = str_replace('</font> :', '', $isbn);
  $isbn = str_replace('</font>', '', $isbn);
  $isbn = str_replace(' (hc)', '', $isbn);
  
//Voila  
  return $isbn;	
}

function get_raw_title_author($id) {
//Forming the link
	$term = $id;
	$link = "http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?profile=int&index=ISBNEX&term=".$term;

//Getting the content
	$continut = file_get_contents($link);

//Parsing
	$aux1 = explode('by title:</a></td></tr><tr><td>&nbsp;</td><td valign="top"><li>&nbsp;</li></td>', $continut);
	$aux2 = explode('href', $aux1[1]);
	$text = $aux2[0];
	$text = str_replace('<td valign="top"><a class="smallAnchor" title="', '', $text);
	$text = str_replace('<font style="background-color: #FFFF80">', '', $text);
	$text = str_replace('</font> :', '', $text);
	$text = str_replace('</font>', '', $text);
	$text = str_replace('"', '', $text);
	
//Voila
	return $text;
	
}

function get_raw_title_author_2($id) {
//Forming the link
	$term = $id;
	$link = "http://myrcpl.ipac.sirsidynix.net/ipac20/ipac.jsp?&index=OA&term=".$term;

//Getting the content
	$continut = file_get_contents($link);

//Parsing
	$aux1 = explode('by title:</a></td></tr><tr><td>&nbsp;</td><td valign="top"><li>&nbsp;</li></td>', $continut);
	$aux2 = explode('href', $aux1[1]);
	$text = $aux2[0];
	$text = str_replace('<td valign="top"><a class="smallAnchor" title="', '', $text);
	$text = str_replace('<font style="background-color: #FFFF80">', '', $text);
	$text = str_replace('</font> :', '', $text);
	$text = str_replace('</font>', '', $text);
	$text = str_replace('"', '', $text);
	
//Voila
	return $text;
	
}


function get_title_2($id) {
	$text=get_raw_title_author_2($id);
	$text=explode("/",$text);
	$title=$text[0];
	return $title;
}

//Returns the author
function get_author($id) {
	$text=get_raw_title_author($id);
	$text=explode("/",$text);
	$author=$text[1];
	$auth=explode(";",$author);
	$author=$auth[0];
	$author=str_replace(" by "," ",$author);
	if($author[strlen($author)-2]=='.') $author[strlen($author)-2]=' ';
	return $author;
}

//Returns the title
function get_title($id) {
	$text=get_raw_title_author($id);
	$text=explode("/",$text);
	$title=$text[0];
	return $title;
}

//Returns the sumarry
function get_sumarry($id) {
  $file="http://www.syndetics.com/index.aspx?type=xw12&isbn=".$id."/SC.GIF&client=richlandcpubl";
  $xml=simplexml_load_file($file);
  $summary=$xml->Review[0]->ReviewText;
  $summary=str_replace('ï¿½',"'",$summary);
  $summary=str_replace('ï¿½',"'",$summary);
  $summary=str_replace("*Starred Review*","",$summary);
  return $summary;
}

function get_img_amazon($id){
	$url=" http://ecx.images-amazon.com/images/I/".$id."._SY100_.jpg";
	return $url;
}

function get_image_magazine($id){
	$url="http://ecx.images-amazon.com/images/I/".$id."._SL500_SS75_.jpg";
	return $url;
}
?>