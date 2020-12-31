<?php
    if (!isset($_SESSION)) {
        session_start();
    }
    if(isset($_SESSION["manag"])){
        if(!isset($_SESSION["id"])){
            echo '<script type="text/javascript">
           window.location = "../homepage.php"
                </script>';
        }
        else if(isset($_SESSION["id"])){
            include './mysql/mysqlConnect.php';

            $delete_query = "DELETE from sensors where id = '" + $_SESSION["id"] + "';";
            if($_GLOBALS["db.connection"]->query($delete_query) > 0){
                include './mysql/mysqlClose.php';
                echo '<script type="text/javascript">
           window.location = "../sensors.php"
                </script>';
            };

            include './mysql/mysqlClose.php';
        }
    }