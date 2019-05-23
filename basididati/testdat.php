<?php

$link = mysqli_connect("127.0.0.1", "luca", "<123>", "AZIENDA");
if (!$link) {
    echo "Connessione fallita al database";
    echo "Codice errore:" . mysqli_connect_errno();
    echo "Errore:" . mysqli_connect_error();
    exit;
}
echo "Connessione riuscita";
mysqli_close($link);



$Nome = $_POST['Nome'];
$Iniziale = $_POST['Iniziale'];
$Cognome = $_POST['Cognome'];
$SSN = $_POST['SSN'];
$Data = $_POST['Data'];
$Indirizzo = $_POST['Indirizzo'];
$Sesso = $_POST['Sesso'];
$Stipendio = $_POST['Stipendio'];
$Super = $_POST['Super'];
$Numero = $_POST['Numero'];



$sql = "INSERT INTO IMPIEGATO VALUES('$Nome','$Iniziale','$Cognome','$SSN','$Data','$Indirizzo','$Sesso','$Stipendio','$Super','$Numero')";
$query = mysqli_query($link, $sql);
if (!$query) {
    echo "errore query" . mysqli_error($link);
    exit;
}
?>





<html>

<head>
    <title>testdat.php</title>
</head>

<body>
    <p> ho inserito un nuovo impiegato </p>

</body>

</html>