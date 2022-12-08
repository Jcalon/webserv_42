<!DOCTYPE html>

<head>
    <script src="https://kit.fontawesome.com/191e243f28.js"></script>
    <title>What time</title>
</head>

<body style="text-align: center;">
<h1>Date</h1>
<i class="fa-solid fa-calendar-days"></i>
<?php
// Set the new timezone
    date_default_timezone_set('Europe/Paris');
    $date = date('d-m-y');
    echo $date;
?>
<h1>Time</h1>
<i class="fa-regular fa-clock"></i>
<?php
// Set the new timezone
    date_default_timezone_set('Europe/Paris');
    $date = date('h:i:s');
    echo $date;
?>

</body>
</html>
