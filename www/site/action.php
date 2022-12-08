<!DOCTYPE html>

<head>
    <script src="https://kit.fontawesome.com/191e243f28.js"></script>
    <title>POST</title>
</head>


<h1>Comment is POST !</h1>
<i class="fa-regular fa-paper-plane"></i>
<body style="text-align: center;">
    <h3>title:</h3> <p><?=$_GET["title"];?></p>
    <h3>content:</h3> <p><?=$_GET["content"];?></p>

    <!-- <p><?=$_GET["title"] . " " . $_GET["content"]?></p> -->

</body>

</html>