<!DOCTYPE html>

<head>
    <title>What time</title>
</head>

<body style="text-align: center;">
<?php
// Set the new timezone
date_default_timezone_set('Europe/Paris');
$date = date('d-m-y h:i:s');
echo $date;
?>

</body>
</html>


