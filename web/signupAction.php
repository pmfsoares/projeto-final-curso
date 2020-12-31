<?php


include './mysql/mysqlConnect.php';

$token = $_POST["token"];

$sql_token = "SELECT id, token FROM signup_tokens WHERE token = '" . $token . "'";
$res_token = $GLOBALS["db.connection"]->query($sql_token);

if($res_token->num_rows > 0){
    $row = $res_token->fetch_assoc();
    $token_id = $row['id'];
    $username = $_POST["username"];
    $email = $_POST["email"];
    $password = password_hash($_POST["password"], PASSWORD_BCRYPT);
    if($password == false){
        $msg_err = "Erro a criar a hash da pwd";
        $_SESSION["msg_err"] = $msg_err;
        include './mysql/mysqlClose.php';
        include './novoUser.php';
        return;
    }
    $nome = $_POST["nome"];
    $telefone = $_POST["telefone"];
    switch($token_id){
        case 1:
            $u_t = 1;
            break;
        case 2:
            $u_t = 2;
            break;
        case 3:
            $u_t = 3;
            break;
    }

    $sql = "insert into users (username, name, email, phone, hash_pwd, signup_token, user_type) 
    values ('" . $username . "','" . $nome . "','" . $email ."','" . $telefone . "','" . $password . "', '" . $token_id . "', '" . $u_t . "' );";

    if ($GLOBALS["db.connection"]->query($sql) > 0) {
        echo "Registo criado com sucesso";
        include './mysql/mysqlClose.php';
        include './index.php';
    } else {
        echo "Erro: " . $sql . "<br>" . $GLOBALS["db.connection"]->error;
    }
}else{
    $msg_err = "Token invalido.";
    $_SESSION["msg_err"] = $msg_err;
    include './mysql/mysqlClose.php';
    include './novoUser.php';
}
