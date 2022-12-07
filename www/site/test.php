
<html>
<head>
  <meta charset="utf-8">
</head>
<body>
<?php
// Vérifier si le formulaire a été soumis
if($_SERVER["REQUEST_METHOD"] == "POST"){
    // Vérifie si le fichier a été uploadé sans erreur.
        $filename = $_FILES["photo"]["name"];
        $filesize = $_FILES["photo"]["size"];
        // Vérifie l'extension du fichier


        // Vérifie la taille du fichier - 5Mo maximum
        $maxsize = 5 * 1024 * 1024;
        if($filesize > $maxsize) die("Error: La taille du fichier est supérieure à la limite autorisée.");

        // Vérifie le type MIME du fichier

            // Vérifie si le fichier existe avant de le télécharger.
         // if(file_exists("www/site/" . $_FILES["photo"]["name"])){
         //       echo $_FILES["photo"]["name"] . " existe déjà.";
         // }

            if (move_uploaded_file($_FILES["photo"]["tmp_name"], "www/site/".$_FILES["photo"]["name"]))
            {
               echo "Votre fichier a été téléchargé avec succès.";
            } else {
               echo "NON";
            }
         }
?>
</body>
</html>
