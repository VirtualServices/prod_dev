<?php

	define('SYHOST', 'hp_horizon');
  define('SYUSER', 'sa');
  define('SYPASS', 'hto227');
  define('SYDB', 'horizon');


	function sycom($query = NULL)
	{
		if(!is_null($query))
		{
			$sl = @sybase_connect(SYHOST, SYUSER, SYPASS);
			@sybase_select_db(SYDB, $sl);
			$result = sycom($query, $sl);
	##		@sybase_close($sl);
		}
		if(!is_null($result))
		{
			return $result;
		}
		else
		{
			return NULL;
		}
	}

?>
