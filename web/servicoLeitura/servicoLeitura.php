<?php

//necessário para dizer ao receptor que o conteudo é json
header("Content-type: application/json");

include '../mysql/mysqlConnect.php';

$amigoDeConversaId = $_GET["amigoDeConversaId"];

session_start();
$id = $_SESSION["id"];
$result = $GLOBALS["db.connection"]->query(
    "select m.texto as texto, m.data as data, autor.nome as autor, autor.id as idAutor, destinatario.id as idDestinatario 
                           from facebook.mensagem m 
                           join facebook.utilizador autor on autor.id = m.idAutor 
                           join facebook.utilizador destinatario on destinatario.id = m.idTarget where autor.id in ($id, $amigoDeConversaId) and destinatario.id in ($id, $amigoDeConversaId)  order by data"
);
if($result == false)
{
    echo $GLOBALS["db.connection"]->error;
}

$todos = array();
while ($row = $result->fetch_assoc()) {
    $todos[] = $row; //atribui o array do user à ultima prosicao do array geral
}
echo json_encode($todos);

include '../mysql/mysqlClose.php';

?>

