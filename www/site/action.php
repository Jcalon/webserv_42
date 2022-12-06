<!DOCTYPE html>
<html>
<head>
<title>2 seconds load time</title>
</head>
<body style="text-align: center;">
<?php
echo date('h:i:s') . "<br>";
sleep(2);
echo date('h:i:s');
?>


        <p>title is: <?=$_POST['title'];?></p>
        <p>content is: <?=$_POST['content'];?></p>
<?php
$array = array(
    "foo" => "bar",
    "bar" => "foo",
);
$key = $_POST['title'];
$value = $_POST['content'];
$array[$key] = $value;
?>


<p><?=$_POST['title'] . " " . $_POST['content']?></p>

<?php
$NbrCol = 2;
$NbrLigne = var_dump(count($array));
// -------------------------------------------------------
echo '<table>';
for ($i=1; $i<=$NbrLigne; $i++) {
   echo '<tr>';
   for ($j=1; $j<=$NbrCol; $j++) {
         echo '<td>';
          echo $array[$i];
         echo '</td>';
   }
   echo '</tr>';
   $j=1;
}
echo '</table>';
?>
</body></html>
