
var mqttServer = require('mqtt-server');

var servers = mqttServer({
  mqtt: 'tcp://localhost:1883',
}, {
  emitEvents: true // default
}, function(client){
  client.connack({
    returnCode: 0
  });
});

servers.listen(function(){
  console.log('listening!');
});
