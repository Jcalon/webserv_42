<?php

	if (isset($_SERVER['UPLOAD_DIR']) && !empty($_SERVER['UPLOAD_DIR']))
	{
		$uploadfile = $_SERVER['UPLOAD_DIR'] . basename($_FILES['userfile']['name']);

		if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile)) {
			echo "<script>window.location.href = '/success.html';</script>";
			return ;
		}
	}
	echo "<script>window.location.href = '/failed.html';</script>";
	return ;
?>
