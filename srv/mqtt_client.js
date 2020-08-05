var mqtt = require('mqtt')
var MQTT_TOPIC = "sensor/teste";
var MQTT_ADDR = "mqtt://localhost:1883";
var MQTT_PORT = 80;
var client  = mqtt.connect(MQTT_ADDR,{clientId: "webClient", keeplive: 1, clean: false, debug:true});

var express = require('express');
var socket = require('socket.io');

//store the express functions to var app
var app = express();

//Create a server on localhost:5000
var server = app.listen(process.env.PORT || 5000);

//var server = app.listen((process.env.PORT || 3000, function(){
  //console.log("Express server listening on port %d in %s mode", this.address().port, app.settings.env);
//});
//host content as static on public
app.use(express.static('public'));

console.log("Node is running on port 5000...");

//MQTT
client.on('connect', function () {
    client.subscribe(MQTT_TOPIC, { qos: 2 });
    client.publish(MQTT_TOPIC, '1000');
});
client.on('message', function (topic, message) {
    // message is Buffer
    console.log(message.toString());
    client.end();
});
client.on('error', function(){
    console.log("ERROR")
    client.end()
})
client.on('offline', function() {
    console.log("offline");
});
client.on('reconnect', function() {
    console.log("reconnect");
});