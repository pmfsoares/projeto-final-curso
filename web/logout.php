<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

if (!isset($_SESSION)) {
    session_start();
}

unset($_SESSION['username']);
unset($_SESSION['id']); //NOVO
session_destroy();

//ou

include 'index.php';

?>