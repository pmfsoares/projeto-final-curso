<html>

<head>
    <?php
    include_once './header.php';
    ?>
    <link rel="stylesheet" href="stylesheet.css">
    
</head>

<body>
    <div class="container">
        <div class="row">
            <div class="col-lg-3 col-md-2"></div>
            <div class="col-lg-6 col-md-8 login-box">
                <div class="col-lg-12 login-title">SIGN UP
                </div>

                <div class="col-lg-12 login-form">
                    <div class="col-lg-12 login-form">
                        <form method="post" action="signupAction.php" method="POST">
                            <div class="form-group">
                                <label class="form-control-label">USERNAME</label>
                                    <input type="text" name="username" class="form-control" placeholder="Coloque o username">
                            </div>

                            <div class="form-group">
                                <label class="form-control-label">EMAIL</label>
                                    <input type="text" name="email" class="form-control" placeholder="Coloque a email">
                            </div>

                            <div class="form-group">
                                <label class="form-control-label">PASSWORD</label>
                                    <input type="password" name="password" class="form-control" placeholder="Coloque a password">
                            </div>

                            <div class="form-group">
                                <label class="form-control-label">NOME</label>
                                    <input type="text" name="nome" class="form-control" placeholder="Coloque o nome">
                            </div>

                            <div class="form-group">
                                <label class="form-control-label">TELEFONE</label>
                                    <input type="text" name="telefone" class="form-control" placeholder="Coloque o telefone">
                            </div>
                            <div class="form-group">
                                <label class="form-control-label">TOKEN</label>
                                    <input type="text" name="token" class="form-control" placeholder="Coloque o token">
                            </div>
                            <div class="col-lg-12 loginbttm">
                                <div class="col-6 login-btm login-button">
                                    <button type="submit" class="btn btn-outline-login float-right">Submit</button>
                                </div>
                            </div>
                            <div class="form-group">
                                <p><?php
                                    if (isset($_SESSION["msg_err"])) {
                                        echo $_SESSION["msg_err"];
                                        unset($_SESSION["msg_err"]);
                                    }
                                    ?>
                                </p>
                            </div>
                        </form>
                    </div>
                </div>
            </div>
        </div>
    </div>
</body>

</html>