<?php

// Connessione 
$link = mysqli_connect("127.0.0.1", "username", "password", "database");

// Controllo se è avvenuta la connessione al database:
if (!$link) { // if ($link != NULL)
    echo "Si è verificato un errore: Non riesco a collegarmi al database <br/>";
    echo "Codice errore: " . mysqli_connect_errno() . "<br/>";
    echo "Messaggio errore: " . mysqli_connect_error() . "<br/>";
    exit;
}

echo "OK!";

mysqli_close($link);


?>

