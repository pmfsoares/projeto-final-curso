<?php
        if(!isset($_SESSION)){
            session_start();
        }
        if(isset( $_SESSION["username"] )){
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
            <div class='container' ng-app="recordingsApp" ng-controller="recController" id="recordingsApp" ng-init="hidden_var=true">
            <div class='row'>
            <div class='col-md-1'></div>
            <div class='col-md-10'>
                <script>
                    let app = angular.module('recordingsApp', []);
                    
                    app.controller('recController', function($scope, $interval){
                        $scope.recordings = new Array();
                        $scope.maxId = 0;
                        $scope.minId = 0;
                        $scope.limit = 9;
                        $scope.loading = true;
                        $scope.servicoLeitura  = function(type_call){
                            $.getJSON(
                                "servicoLeitura/servicoLeituraRecs.php",
                                {
                                    arg: type_call,
                                    maxId: $scope.minId*1,
                                    minId: $scope.minId*1,
                                    limit: $scope.limit
                                },
                                function(jsonData){
                                    $scope.recordings = new Array();
                                    for(ele in jsonData){
                                        let rec = jsonData[ele];
                                        if(rec.id  > $scope.maxId){
                                            $scope.maxId = rec.id;
                                        }else if(rec.id < $scope.minId || $scope.minId == 0){
                                            $scope.minId = rec.id;
                                        }
                                        rec["play"] = false;
                                        let dt = new Date(1000 * parseInt(rec["timestamp"]));
                                        $scope.recordings.push(rec);
                                    }
                                    console.log($scope.recordings);
                                    $scope.loading = false;
                                    console.log("Loading: " + $scope.loading);
                                    $scope.$apply();
                                }
                            )
                        };
                        $scope.next = function(){
                            $scope.loading = true;
                            $scope.servicoLeitura(2);
                            
                        }
                        $scope.previous = function(){
                            $scope.loading = true;
                            $scope.servicoLeitura(1);
                        }
                        $scope.makeWav = function(pos){
                            let rec = $scope.recordings[pos];
                            let tmp_arr = rec["data"].split(",");
                            let tmp = new Array();
                            console.log(typeof tmp_arr);
                            for(ele in tmp_arr){
                                tmp.push(parseInt(tmp_arr[ele]));
                            }
                            let blob_arr = new Uint8Array(tmp);
                            const blob = new Blob([blob_arr], { type: 'audio/wav'});
                            const url = URL.createObjectURL(blob);
                            let audioCol = document.getElementById("audioCtrlCol_" + rec.id);
                            let au = document.createElement('audio');
                            let hf = document.createElement('div');
                            au.controls = true;
                            au.src = url;
                            hf.id = "player";
                            let btn = document.getElementById(rec["id"] + "_btn").remove();
                            audioCol.appendChild(au);
                            au.load();
                        };
                        $scope.remove = function(){
                            $scope.recordings.shift();
                        }
                        $scope.servicoLeitura(1);
                    });
                </script>
                <div class="recordings_panel" infinite-scroll="loadMore()">
                    <div class="row flex recordings_row login-box">
                            <div class="col-md-3 recordings_col">
                                <strong>Sensor MAC</strong>
                            </div>
                            <div class="col-md-2 recordings_col">
                                <strong>Timestamp</strong>
                            </div>
                            <div class="col-md-2 recordings_col">
                                <strong>Sample Rate/Size</strong>
                            </div>
                            <!--<div class="col-md-1 recordings_col">
                                <strong>Sample Size</strong>
                            </div>-->
                            <div class="col-md-1 recordings_col"></div>
                            <div class="col-md-2 recordings_col">
                                <button class="btn btn-md" ng-click="previous()" style="color:white;">
                                    <span class="glyphicon glyphicon-chevron-left"></span>
                                </button>
                                <button class="btn btn-md" ng-click="next()" style="color: white;">
                                    <span class="glyphicon glyphicon-chevron-right"></span>
                                </button>
                            </div>
                            <div class="col-md-1 recordings_col"></div>
                            <div class="col-md-1 recordings_col">
                                <button class="btn btn-md" ng-model="hidden_var" ng-click="hidden_var = !hidden_var" style="color: white!important;">
                                    <span ng-hide="!hidden_var" class="glyphicon glyphicon-menu-down"> </span>
                                    <span ng-hide="hidden_var" class="glyphicon glyphicon-menu-up"> </span>
                                </button>
                            </div>
                    </div>
                    <div class="row flex recordings_row login-box" ng-hide="hidden_var">
                        <div class="col-md-4" id="search_select">
                            <select ng-model="queryBy" class="form-control login-box">
                                <option value="sensor_mac">Sensor Mac</option>
                                <option value="timestamp">Date</option>
                                <option value="sample_size">Sample Size</option>
                                <option value="sample_rate">Sample Rate</option>
                            </select>
                        </div>
                        <div class="col-md-8" id="search_input">
                            <input ng-model="query[queryBy]" type="text" class="form-control" name="password" placeholder="Pesquisa">
                        </div>
                    </div>
                    <div class="row flex recordings_row login-box" ng-hide="!loading">
                        <div class="col-md-5"></div>
                        <div class="col-md-2">
                            <div class="row loading_circle">
                                <label for="check"></label>
                            </div>
                        </div>
                        <div class="col-md-5"></div>
                    </div>
                    <div class="row flex recordings_row login-box" ng-hide="loading" ng-repeat="rec in recordings | filter: query | limitTo: limit">
                            <div class="col-md-3 recordings_col">
                                <p>
                                    <a class="btn btn-md" style="color: white;" href="sensorConfig.php?s_id={{rec.id_sensor_t}}">
                                    {{rec.sensor_mac}}
                                    </a>
                                </p>
                            </div>
                            <div class="col-md-2 recordings_col">
                                <p>{{rec.timestamp}}</p>
                            </div>
                            <div class="col-md-2 recordings_col">
                                <p>{{rec.sample_rate}}</p>
                                 <p>{{rec.sample_size}}</p>
                            </div>
                            <div class="col-md-5 recordings_player" id="audioCtrlCol_{{rec.id}}">
                                <button class="btn btn-default btn-md " id="{{rec.id}}_btn" ng-click="makeWav($index);">
                                    <span class="glyphicon glyphicon-play"></span>
                                </button>
                            </div>
                    </div>
                </div>
                </div>
                </div>
            </div>
            <div class='col-md-1'></div>
        </body>
</html>
<?php 
}
        else{
            include './index.php';
        }
        ?>