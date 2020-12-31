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
        <script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js"></script>
        <meta http-equiv="content-type" content="text/html; charset=UTF-8">
    </head>

    <body>
        <div ng-app="sensorsApp" ng-controller="sensorsController" id="sensorsApp" ng-init="hidden_var=true">
            <script>
                let app = angular.module('sensorsApp', []);
                app.controller('sensorsController', function($scope, $interval) {
                    $scope.sensors = [];

                    $scope.servicoLeitura = function() {
                        $.getJSON(
                            "servicoLeitura/servicoLeituraSensors.php", {
                                arg: 1
                            },
                            function(jsonData) {
                                for (ele in jsonData) {
                                    let tmp_sensors = jsonData[ele];
                                    /**
                                     * 1 - registered
                                     * 2 - not registered:w
                                     * 
                                     * 3 - 
                                     * */
                                    tmp_sensors["led_class"] = "led_" + tmp_sensors["status"];
                                    $scope.sensors.push(tmp_sensors);
                                    console.log(tmp_sensors);
                                }
                                $scope.$apply();
                            }
                        )
                    };
                    $scope.servicoLeitura();
                });
            </script>
                <div class="recordings_panel">
                <div class="header_panel">
                <div class="row flex recordings_row login-box">
                    <div class="col-md-11">
                            <div class="col-sm-1 recordings_col">
                                <div class="col-sm-1"></div>
                                <div class="col-sm-6">
                                    <strong>Estado</strong>
                                </div>
                                <div class="col-sm-5"></div>
                               
                            </div>
                            <div class="col-sm-2 recordings_col ">
                                <strong>MAC</strong>
                            </div>
                            <div class="col-sm-1 recordings_col">
                                <strong>Descriptor</strong>
                            </div>
                            <div class="col-sm-1 recordings_col ">
                                <strong>Modelo</strong>
                            </div>
                            <div class="col-sm-2 recordings_col">
                                <strong>Sample Rate</strong>
                            </div>
                            <div class="col-sm-2 recordings_col">
                                <strong>Sample Size</strong>
                            </div>
                            <div class="col-md-3 recordings_col">
                                <strong>Coordenadas</strong>
                            </div>
                    </div>
                    <div class="col-md-1">
                        <button class="btn btn-md" ng-model="hidden_var" ng-click="hidden_var = !hidden_var" style="color: white!important;">
                            <span ng-hide="!hidden_var" class="glyphicon glyphicon-menu-down"> </span>
                            <span ng-hide="hidden_var" class="glyphicon glyphicon-menu-up"> </span>
                        </button>
                    </div>
                </div>
                <div class="row flex recordings_row login-box" ng-hide="hidden_var" style="height: 54px;">
                    <div class="col-md-2" id="search_select">
                        <select ng-model="queryBy" class="form-control login-box">
                            <option value="sensor_mac">Sensor MAC</option>
                            <option value="model">Model</option>
                            <option value="coord_lat">Latitude</option>
                            <option value="sample_size">Sample Size</option>
                            <option value="sample_rate">Sample Rate</option>
                        </select>
                    </div>
                    <div class="col-md-6">
                        <div class="col-md-8" id="search_input">
                            <input ng-model="query[queryBy]" type="text" class="form-control" name="password" placeholder="Pesquisa">
                        </div>
                    </div>
                    <div class="col-md-3"> </div>
                    <div class="col-md-1"> 
                        <?php
                        if($_SESSION["manag"]){ ?>
                            <a class="btn btn-md glyphicon glyphicon-plus" style="color: white;" href="sensorConfig.php">
                              <!--  <img class="img-fluid" style="max-width:25%; padding: 0;!important" src="sensors.png"> 
                                <span class="glyphicon glyphicon-plus"></span> -->
                            </a>
                        <?php }
                        ?>
                    </div>
                </div>
                </div>
                <div class="row flex recordings_row login-box" ng-repeat="sensor in sensors | filter: query">
                    <div class="col-md-11">
                        <div class="row">
                            <div class="col-sm-1 recordings_col">
                                <div class="col-sm-5"></div>
                                <div class="col-sm-2">
                                    <div ng-class="'{{sensor.led_class}}'">

                                    </div>
                                </div>
                                <div class="col-sm5"></div>
                            </div>
                            <div class="col-sm-2 recordings_col ">
                                <p>{{sensor.sensor_mac}}</p>
                            </div>
                            <div class="col-sm-1 recordings_col ">
                                <p>{{sensor.sensor_descriptor}}</p>
                            </div>
                            <div class="col-sm-1 recordings_col ">
                                <p>{{sensor.model}}</p>
                            </div>
                            <div class="col-sm-2 recordings_col">
                                <p>{{sensor.sample_rate}} </p>
                            </div>
                            <div class="col-sm-2 recordings_col">
                                <p>{{sensor.sample_size}}</p>
                            </div>
                            <div class="col-md-3 recordings_col">
                                <a href="https://maps.google.com/?q={{sensor.coord_lat}},{{sensor.coord_long}}&z=12&t=k">
                                    <p>{{sensor.coord_lat}} N° {{sensor.coord_long}} E° </p>
                                </a>
                            </div>
                        </div>
                    </div>
                    <div class="col-sm-1">
                    <?php

                                if($_SESSION["manag"]){

                            ?>
                        
                        <div class="col-sm-6">
                            <a class="btn btn-md" style="color: white;" href="sensorConfig.php?s_id={{sensor.id}}">
                                    <span class="glyphicon glyphicon-pencil"></span>
                            </a>
                        </div>
                        <div class="col-sm-6">
                            <a class="btn btn-md" style="color: white;" href="deleteSensor.php?id={{sensor.id}}">
                                <span class="glyphicon glyphicon-remove"></span>
                            </a> 
                        </div>

                    <?php } ?>
                    </div>
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