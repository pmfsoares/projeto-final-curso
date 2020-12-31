<?php
if (!isset($_SESSION)) {
    session_start();
}
if (isset($_SESSION["username"])) {
    include_once './header.php';

?>
    <!DOCTYPE html>

    <head>
        <link rel="stylesheet" type="text/css" href="bootstrap/css/bootstrap.min.css">
        <link rel="stylesheet" type="text/css" href="stylesheet.css">
        <script src="bootstrap/jquery.min.js"></script>
        <script src="angular/angular.min.js"></script>
        <script src="bootstrap/js/bootstrap.min.js"></script>
        <meta http-equiv="content-type" content="text/html; charset=UTF-8">
    </head>

    <body>
        <div class="container">
            <div class="row" style="padding-top: 100px;">
                <div class="col no-gutters">
                    <div class="col-lg-3 col-md-2"></div>
                    <a href="sensors.php">
                    <div class="col-lg-6 col-md-8 login-box">
                        <div class="col-lg-12 login-form">
                            <div class="col-lg-12 login-form">
                                
                            </div>
                        </div>
                        <img class="img-fluid" src="sensors.png">
                        <div class="col-lg-12 login-form">
                        </div>
                        <div class="col-lg-12 login-title">SENSORS
                        </div>

                        
                        <div class="col-lg-3 col-md-2"></div>
                    </div>
                    </a>
                </div>
                <div class="col no-gutters">
                    <div class="col-lg-3 col-md-2"></div>
                    <a href="recordings.php">
                    <div class="col-lg-6 col-md-8 login-box">
                        <div class="col-lg-12 login-form">
                            <div class="col-lg-12 login-form">
                                
                            </div>
                        </div>
                        <img class="img-fluid" src="sensor_folder.png">
                        <div class="col-lg-12 login-form">
                        </div>
                        <div class="col-lg-12 login-title">RECORDINGS
                        </div>

                        
                        <div class="col-lg-3 col-md-2"></div>
                    </div>
                    </a>
                </div>
            </div>
        </div>
    </body>

    </html>
<?php
} else {
    include './index.php';
}
?>