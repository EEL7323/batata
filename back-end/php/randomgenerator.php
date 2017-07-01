<?php

function random($digits) {
    return (rand(10**($digits-1), 10**($digits)-1));
}
?>