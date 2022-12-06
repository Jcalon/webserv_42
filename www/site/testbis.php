<?php
$uploaddir = 'www/site/';
$uploadfile = $uploaddir . basename($_FILES['mon_fichier']['name']);

echo '<pre>';
if (move_uploaded_file($_FILES['mon_fichier']['tmp_name'], $uploadfile)) {
    echo "Le fichier est valide, et a été téléchargé
           avec succès. Voici plus d'informations :\n";
} else {
    echo "Attaque potentielle par téléchargement de fichiers.
          Voici plus d'informations :\n";
}

echo 'Voici quelques informations de débogage :';
print_r($_FILES);

echo '</pre>';

?>
