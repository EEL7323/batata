<?php
	date_default_timezone_set('Brazil/East');

	$lunchBeginning = date('H:i', mktime(10,50));
	$lunchEnd = date('H:i', mktime(14, 0));
	$dinnerBeggining = date('H:i', mktime(16,50));
	$dinnerEnd = date('H:i', mktime(19,30));

	if ((date('H:i') > $lunchBeginning && date('H:i') > $lunchEnd) || (date('H:i') > $dinnerBeggining && date('H:i') > $dinnerEnd)) echo true;
	else echo false;

?>