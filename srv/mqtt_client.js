const { create } = require('domain');

const mqtt = require('mqtt'), 
    express = require('express'), 
    socket = require('socket.io'), 
    mysql = require('mysql'), 
    minifier = require('string-minify'),
    fs = require('fs'),
    WaveFile = require('wavefile').WaveFile;

const MQTT_TOPIC = "sensor/#", MQTT_ADDR = "mqtt://192.168.1.80:1883", MQTT_PORT = 80,
    client  = mqtt.connect(MQTT_ADDR,{clientId: "webClient", keeplive: 1, clean: false, debug:true});

//let json_fs = JSON.parse(fs.readFileSync('json_output.json', 'utf8')); 
//let json_fs = require('../json_output.json');
//createWav(json_fs);

let con = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "admin"
});

con.connect(function(err){
    if(err) throw err;
    console.log("Connected to mysql.");
})

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
    console.log("Connected to mqtt");
    client.subscribe(MQTT_TOPIC, { qos: 2 });
});

let query = "INSERT into `projeto`.`recordings` SET ?";

client.on('message', function (topic, message) {
    let payload_json = messageJSON(message);
    let wav_file;
    try{
        wav_file = createWav(payload_json);
    }catch(err){
        console.log(err);
    }
    let values = {
      sensor_id: payload_json["sensor_id"],
      timestamp: new Date(payload_json["timestamp"] * 1000).toISOString().slice(0, 19).replace('T', ' '),
      sample_rate: payload_json["sample_rate"],
      sample_size: payload_json["sample_size"],
      data: wav_file.toBuffer()
    };
    con.query(query, values, function(error, results){
        if(error){
            console.log(error);
            throw error;
            console.log("ERRO");
            return;
        }
        return;
    });
    return;
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

function messageJSON(message){
    let payload = message.toString('utf8');
    let msg_json = new Object();
    try{
      msg_json = JSON.parse(payload.slice(0, payload.length-1));
    }catch(err){
        console.log(err);
    }
    return msg_json;
}

function createWav(event){
    console.log("Creating WAV");
    let wav = new WaveFile();
    let pas_val = 101.325;
    let arr = [];
    console.log(event["data"].length);
    for(val in event["data"]){
        arr.push(parseFloat(event["data"][val]) / pas_val );
    }
    let d = new Date(1000 * event["timestamp"]);
    
// Create a mono wave file, 44.1 kHz, 32-bit and 4 samples
    wav.fromScratch(1, parseInt(event["sample_rate"]), '32f', arr);
    fs.writeFileSync("./wav_files/" + event["sensor_id"] + "_" + d.toISOString().replace(/T/, ' ').replace(/\..+/, '') + ".wav", wav.toBuffer());
    return wav;
}
