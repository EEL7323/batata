<?php
// Function to get the index of a given registry number in an 2D array
function getIndexOfRegistry($registryNumber, $array) {
    $i = 0;
	$index = -1;
	for ($i = 0; $i < sizeof($array); $i++) {
		if ($array[$i][0] == $registryNumber) {
			$index = $i;
			break;
		}
	} 
	return $index;
}

?>