<?
define('KEYID','0QYW7KKXSTCTMSZ9CG82');
define('AssocTag','YourAssociateTagHere');
define('NYT_FICTION_URL','http://www.amazon.com/exec/obidos/tg/feature/-/239232/');
define('NYT_NONFICTION_URL','http://www.amazon.com/exec/obidos/tg/feature/-/239332/');
define('NYT_CHILDREN_URL','http://www.amazon.com/exec/obidos/tg/feature/-/239365/');
define('YOUR_WEBSITE_URL','http://www.myrcpl.com');

// End Definations 
function grabData($source){
	$ch = curl_init();
	curl_setopt ($ch, CURLOPT_URL,$source);
	curl_setopt ($ch, CURLOPT_USERAGENT, 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.1.4322)');
	curl_setopt ($ch, CURLOPT_REFERER, 'http://www.amazon.com');
	curl_setopt ($ch, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);
	curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 1);
	curl_setopt ($ch, CURLOPT_FOLLOWLOCATION,0);
	curl_setopt ($ch, CURLOPT_HEADER, 0);
	curl_setopt ($ch, CURLOPT_TIMEOUT, 300);
	$str = curl_exec ($ch);
	curl_close($ch);

	$dataz = "";
	$GrabStart = strpos($str,'<b>Posted:');  
	$GrabEnd = strpos($str,'</b><br clear="all"/>');
	$Output = substr($str,$GrabStart,$GrabEnd-$GrabStart);
	$dataz['listDate']=trim(str_replace('<b>Posted:','', $Output));



	$datas = split('/dp/customer-reviews/',$str);
	
	
	$i=0;
	foreach ($datas as $data){

		$data2 = split('/ref=',$data);
		if($i!=0){
			$dataz['infoz'][] = $data2[0];
		}
		$i++;
	}
	return $dataz;
}


function ItemLookup($asin){
	$request = "http://ecs.amazonaws.com/onca/xml?Service=AWSECommerceService&AWSAccessKeyId=".KEYID."&AssociateTag=".AssocTag."&Version=2006-09-11&Operation=ItemLookup&ItemId=$asin&ResponseGroup=Medium,Offers";
	$session = curl_init($request);
	curl_setopt($session, CURLOPT_HEADER, false);
	curl_setopt($session, CURLOPT_RETURNTRANSFER, true);
	$response = curl_exec($session);
	curl_close($session);
	$parsed_xml = simplexml_load_string($response);
	return "<item>\n" .
	"\t<title><![CDATA[".$parsed_xml->Items->Item->ItemAttributes->Title."]]></title>\n" .
	"\t<author><![CDATA[".$parsed_xml->Items->Item->ItemAttributes->Author."]]></author>\n" .
	"\t<isbn><![CDATA[".$parsed_xml->Items->Item->ItemAttributes->ISBN."]]></isbn>\n" . 
	"\t<link><![CDATA[http://www.myrcpl.com/ipac20/ipac.jsp?&index=.ek&term=".$parsed_xml->Items->Item->ItemAttributes->ISBN."]]></link>\n" . 
	"\t<rank><![CDATA[".$parsed_xml->Items->Item->SalesRank."]]></rank>\n" . 
	"\t<cover><![CDATA[http://www.syndetics.com/index.aspx?type=xw12&isbn=".$parsed_xml->Items->Item->ItemAttributes->ISBN."/SC.GIF&client=richlandcpubl]]></cover>\n" . 
	"\t<guid isPermaLink='false'>".$parsed_xml->Items->Item->ItemAttributes->ISBN."</guid>\n" . 
	"</item>";
	}


function ItemLookupReturnArray($asin){
	$request = "http://ecs.amazonaws.com/onca/xml?Service=AWSECommerceService&AWSAccessKeyId=".KEYID."&AssociateTag=".AssocTag."&Version=2006-09-11&Operation=ItemLookup&ItemId=$asin&ResponseGroup=Medium,Offers";
	$session = curl_init($request);
	curl_setopt($session, CURLOPT_HEADER, false);
	curl_setopt($session, CURLOPT_RETURNTRANSFER, true);
	$response = curl_exec($session);
	curl_close($session);
	$parsed_xml = simplexml_load_string($response);

	$itemsXml['title'] = (String)$parsed_xml->Items->Item->ItemAttributes->Title;
	$itemsXml['author'] = (String)$parsed_xml->Items->Item->ItemAttributes->Author;
	$itemsXml['isbn'] = (String)$parsed_xml->Items->Item->ItemAttributes->ISBN;
	$itemsXml['rank'] = (String)$parsed_xml->Items->Item->SalesRank;
	return $itemsXml;
}
?>