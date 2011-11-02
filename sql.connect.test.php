<?php

  define('HOST', ' sdsql01.dynixasp.com');
  define('USER', ' rcplstaff ');
  define('PASS', ' C0lumbi@803');
  define('DB', 'rcpl');


	function sycom($query = NULL)
	{
		if(!is_null($query))
		{
			$sl = @sybase_connect(HOST, USER, PASS);
			@sybase_select_db(DB, $sl);
			$result = sycom($query, $sl);
                        print 'Inside Loop1<br>';
	##		@sybase_close($sl);
		}
		if(!is_null($result))
		{
                        print 'Inside Loop2<br>';
			return $result;
		}
		else
		{
                        print 'Inside Loop3<br>';
			return NULL;
		}
	}

?>
