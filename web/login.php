<?php

if (!isset($_SESSION)) {
    session_start();
}

$username = $_POST["username"];
$password = $_POST["password"];

$found = false;

include './mysql/mysqlConnect.php';
$sql = "SELECT id, username, hash_pwd, user_type FROM users where username = '" . $username . "'";
$result = $GLOBALS["db.connection"]->query($sql);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    if(password_verify($password, $row["hash_pwd"])){
        $found = true;
        if (!isset($_SESSION)) {
            session_start();
        }
        $_SESSION["username"] = $row["username"];
        $_SESSION["id"] = $row["id"];
        switch($row["user_type"]){
            case 1:
            case 2:
                $_SESSION["manag"] = true;
                break;
            case 3:
                $_SESSION["manag"] = false;
                break;
        }
    } else{
        $_SESSION["msg_err"] = 'Utilizador ou palavra-passe incorrectos.';
    }
}else{
    $_SESSION["msg_err"] = 'Utilizador ou palavra-passe incorrectos.';
}
include './mysql/mysqlClose.php';

header('Location: homepage.php');


?>
