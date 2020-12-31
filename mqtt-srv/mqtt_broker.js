const aedes = require('aedes')()
const server = require('net').createServer(aedes.handle)
const port = 1883

server.listen(port, function () {
  console.log('server started and listening on port ', port)
});
aedes.on('connectionError', function(client, error){
  console.log(client);
  console.log(error);
});
aedes.on('client', function(client){
  console.log('New client' + client.id);
});