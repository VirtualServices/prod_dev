<?php

/**
*	powered by @cafewebmaster.com
*	free for private use
*	please support us with donations
*/


?>

<form method="post">
<input name="db_host" value="localhost" />
<input name="db_user">
<input name="db_password" type="password" />
<input type="submit" value="List_DBs_and_Tables" />
<br>
<input name="do_list" type="checkbox" value="on" checked="checked" disabled="disabled" />List 
<input name="do_analyze" type="checkbox" value="on" />Analize 
<input  name="do_repair" type="checkbox" value="on" />Repair
<br>
</form>


<?php


$db_host = $_POST['db_host'];
$db_user = $_POST['db_user'];
$db_password = $_POST['db_password'];

$do_analyze = $_POST['do_analyze'];
$do_repair = $_POST['do_repair'];
$db_ignore = ($_POST['db_ignore'])  ? $_POST['db_ignore'] : "nodbignore" ;



if(!$db_host || !$db_user){ die(); } 


mysql_connect("$db_host","$db_user","$db_password") or die("Error: No BD Connection");


$rs = mysql_query("show databases");

while($arr=mysql_fetch_array($rs)){


	echo "<h2>$arr[0]</h2><ol>";
	
	mysql_select_db("$arr[0]");
	$rs2 = mysql_query("show tables");
	while($arr2=mysql_fetch_array($rs2)){

 

		if($do_analyze){
			$rs3 = mysql_query("analyze table `$arr2[0]`"); echo mysql_error();
			$arr3=mysql_fetch_array($rs3);
		}

		if($do_repair){
			$rs4 = mysql_query("repair table `$arr2[0]`"); echo mysql_error();
			$arr4=mysql_fetch_array($rs4);
		}

		echo "<li>$arr2[0] <i>$arr3[3]</i> <b>$arr4[3]</b>";


		
		
	}
	echo "</ol>"; 


}

