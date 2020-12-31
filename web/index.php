<html>
  <head>
      <title>Login</title>
      <link rel="stylesheet" href="stylesheet.css">
  </head>
  <body>
<?php
        if(!isset($_SESSION)){
            session_start();
        }
        if(isset( $_SESSION["username"] )){
            include_once'./homepage.php';

        }
        else{
            include './header.php';
?>
<div class="container">
    <div class="row">
        <div class="col-lg-3 col-md-2"></div>
        <div class="col-lg-6 col-md-8 login-box">
            <div class="col-lg-12 login-key">
                <i class="fa fa-key" aria-hidden="true"></i>
            </div>
            <div class="col-lg-12 login-title">LOGIN
            </div>

            <div class="col-lg-12 login-form">
                <div class="col-lg-12 login-form">
                    <form method="post" action="login.php" method="POST">
                        <div class="form-group">
                            <label class="form-control-label">USERNAME</label>
                            <input type="text" class="form-control" name="username">
                        </div>
                        <div class="form-group">
                            <label class="form-control-label">PASSWORD</label>
                            <input type="password" class="form-control" name="password">
                        </div>

                        <div class="col-lg-12 loginbttm">
                            <div class="col-6 login-btm login-button ">
                               <button type="button" class="btn btn-outline-signup float-left">
                                   <a href="signup.php">Sign up</a>
                                </button>
                            </div>
                            <div class="col-6 login-btm login-button">
                                <button type="submit" class="btn btn-outline-login float-right">Login</button>
                            </div>
                        </div>
                        <div class="form-group">
                            <p><?php
                                if(isset($_SESSION["msg_err"])){
                                    echo $_SESSION["msg_err"];
                                    unset($_SESSION["msg_err"]);
                                }
                                ?>
                            </p>
                        </div>
                    </form>
                </div>
            </div>
            <div class="col-lg-3 col-md-2"></div>
        </div>
    </div>
</div>
      <?php
        }
        //include "listaUtilizadores.php";
        ?>


  </body>
</html>

