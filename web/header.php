<head>
      <title>Login</title>
      <title>Login</title>
      <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css">
      <script src="https://code.jquery.com/jquery-3.4.1.slim.min.js"></script>
      <script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js"></script>
      <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js"></script>
  </head>

<style>
    .header{
        background-color: #1A2226;   
        padding:10px;
        font-size: 16pt;
    }    
</style>
<?php
if (!isset($_SESSION)) {
    session_start();
}
if(isset($_SESSION["username"])){
    $id = $_SESSION["id"];
    ?>
    <!--
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark"> 
    <div class="row" style="width: 100%;">
        <div class="col-md-4"></div>
        <div class="col-md-6">
                <div class="centered input-group" id="Search">
                    <input type="text" class="form-control" id="SearchField" placeholder="Search...">
                </div>
        </div>
        <div class="col-md-4"></div>
</div>
    </div>
    
    </nav> -->
<div class="header clearfix">
    <div class="col-md-3"> </div>
    <div class="col-md-7">
        <a class="text-center" style="color: white;">Plataforma Web de Suporte a Sensores Acústicos @ Pedro Soares 2020</a>
    </div>
    <div class="col-md-1"></div>
    <div class="col-md-1">
    <a class="btn btn-default btn-md pull-right" href="logout.php">
        <span class="glyphicon glyphicon-log-out"></span>
    </a>
    <a class="btn btn-default btn-md pull-right" href="homepage.php">
        <span class="glyphicon glyphicon-home"></span>
    </a>
    </div>
</div> 

    <?php
}
