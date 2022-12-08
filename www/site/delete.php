<!DOCTYPE html>
<HTML lang="fr">
    <HEAD>
        <meta charset="utf-8" />
    </HEAD>

    <BODY>

        <div id="bloc_page">
        <?php
            //Liste les fichiers d'un répertoire
            $folderpath = "../uploads/";
            $path = $folderpath;

            if (file_exists($folderpath) && is_dir($folderpath)) {
                $PointeurVersRepertoire = opendir($folderpath);
                while (($file = readdir($PointeurVersRepertoire)) !== false ) //Lecture d une entree du repertoire
                {
                    // enleve les fichiers . et .. et index.php et .htaccess et les répertoires
                    if ($file != '.' && $file != '..' && $file != 'index.php' && $file != '.htaccess' && !is_dir($file))
                    {
                        $path .= $file;
                        echo $file."<br />\n";
                        unlink($path);
                    }
                }
                closedir($PointeurVersRepertoire);
            }
            else{echo "Le répertoire $folderpath n'existe pas ou ce n'est pas un répertoire.";}
        ?>
        <div style="text-align : center">
            <H1>File deleted</H1>
            <!-- <p>Delete</p> -->
            <iframe src="https://giphy.com/embed/vKHKDIdvxvN7vTAEOM" width="480" height="400" frameBorder="0" class="giphy-embed" allowFullScreen></iframe>
        </div>

        </div> <!-- div bloc_page -->
    </BODY>

</HTML>