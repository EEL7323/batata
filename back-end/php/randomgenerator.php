<?php
// Generate a random with the length of parameter digits
function random($digits) {
    return (rand(10**($digits-1), 10**($digits)-1));
}
?>