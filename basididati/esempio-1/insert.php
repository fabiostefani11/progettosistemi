<?php

	// Connessione 
	$link = mysqli_connect("127.0.0.1", "luca", "<123>", "AZIENDA");

	// Controllo se è avvenuta la connessione al database:
	if (!$link) { // if ($link == NULL)
		echo "Si è verificato un errore: Non riesco a collegarmi al database <br/>";
		echo "Codice errore: " . mysqli_connect_errno() . "<br/>";
		echo "Messaggio errore: " . mysqli_connect_error() . "<br/>";
		exit;
	}

	// $_POST[]
	// $_GET[]

	$NOME_BATT 	= $_POST['NOME_BATT'];
	$INIZ_INT 	= $_POST['INIZ_INT'];
	$COGNOME 	= $_POST['COGNOME'];
	$SSN 		= $_POST['SSN'];
	$DATA_N 	= $_POST['DATA_N'];
	$INDIRIZZO 	= $_POST['INDIRIZZO'];
	$SESSO 		= $_POST['SESSO'];
	$STIPENDIO 	= $_POST['STIPENDIO'];
	$SUPER_SSN 	= $_POST['SUPER_SSN'];
	$N_D 		= $_POST['N_D'];
	
	$sql = "INSERT INTO IMPIEGATO 
					VALUES ('$NOME_BATT', '$INIZ_INT', '$COGNOME', '$SSN', 
							'$DATA_N', '$INDIRIZZO', '$SESSO', $STIPENDIO, 
							'$SUPER_SSN', $N_D)";
	
	$query = mysqli_query($link, $sql);

	if (!$query) {
		echo "Si è verificato un errore: " . mysqli_error($link);
		exit;
	}

?>

<html lang="it">
    
    <head>
        <meta charset="utf-8">
        <title>insert.php</title>
    </head>

    <body>
		<p>Ho inserito il nuovo impiegato <?php echo $NOME_BATT; ?></p>
    </body>

</html>

