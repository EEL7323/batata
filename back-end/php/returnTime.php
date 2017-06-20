<?php
	date_default_timezone_set('Brazil/East');

	$lunchBeginning = date('H:i', mktime(10,40));
	$lunchEnd = date('H:i', mktime(14, 10));
	$dinnerBeggining = date('H:i', mktime(16,40));
	$dinnerEnd = date('H:i', mktime(19,40));

	if ((date('H:i') > $lunchBeginning && date('H:i') < $lunchEnd) || (date('H:i') > $dinnerBeggining && date('H:i') < $dinnerEnd)) echo true;
	else echo false;

?>