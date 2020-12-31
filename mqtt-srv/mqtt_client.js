const { create } = require('domain');

const mqtt = require('mqtt'), 
    express = require('express'), 
    socket = require('socket.io'), 
    mysql = require('mysql'), 
    minifier = require('string-minify'),
    fs = require('fs'),
    WaveFile = require('wavefile').WaveFile;

/**
 * Dados do MQTT broker
 */
const MQTT_TOPIC = "sensor/#", 
    MQTT_ADDR = "mqtt://192.168.12.1:1883",
    MQTT_PORT = 80,
    client  = mqtt.connect(MQTT_ADDR,{clientId: "webClient", keeplive: 1, clean: false, debug:true});

let con, query, db;
/**
 * Define o tipo de ligacao a usar,(localhost ou site)
 */
switch(process.argv[2]){
    case 'local':
        con = mysql.createConnection({
            host: "localhost",
            user: "root",
            password: "admin"
        });
        db = "projeto";
        query = "INSERT into `projeto`.`recordings` SET ?";
        break;
}

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

/**
 * Connects to MQTT broker and subscribes to the needed topics
 */
client.on('connect', function () {
    console.log("Connected to mqtt");
    client.subscribe(MQTT_TOPIC, { qos: 2 });
    //client.subscribe("timing/#", {qos: 2}); topic para os testes de envio
});

//let query = "INSERT into `plvwfdku_projeto`.`recordings` SET ?";
let bytes_object = new Object();

/**
 * Extrai o MAC do topico individual do sensor, dependendo do subtopico 
 */
function getMacFromTopic(full_topic, type){
    let mac_str = String(full_topic).replace(/sensor\//g, "");
    switch(type){
        //registration
        case 1:
            mac_str = String(mac_str).replace(/\/registration/g, "");
            break;

        //json event
        case 2:
            mac_str = String(mac_str).replace(/\/event\/data/g, "");
            break;
        
        //double array topic
        case 3:              
            mac_str = String(mac_str).replace(/\/event\/binary/g, "");
            break;

        //array sending complete topic
        case 4:
            mac_str = String(mac_str).replace(/\/event\/done/g, "");
            break;
        case 5:
            mac_str = String(mac_str).replace(/\/config\/ack/g, "");
    }
    mac_str = String(mac_str).replace(/\//g, ":");
    return mac_str;

}


client.on('message', function (topic, message) {
    console.log("Received msg on topic: " + topic);

    /**
     * Subtopics possiveis
     */
    let rec_topic = /^sensor\/([0-9A-F]{2}\/){5}([0-9A-F]{2})\/event\/data/;
    let conf_topic = /^sensor\/([0-9A-F]{2}\/){5}([0-9A-F]{2})\/registration/;
    let ack_topic = /^sensor\/([0-9A-F]{2}\/){5}([0-9A-F]{2})\/event\/ack/;
    let ack_conf_topic  =/^sensor\/([0-9A-F]{2}\/){5}([0-9A-F]{2})\/config\/ack/;
    let bin_topic = /^sensor\/([0-9A-F]{2}\/){5}([0-9A-F]{2})\/event\/binary/;
    let event_done_topic = /^sensor\/([0-9A-F]{2}\/){5}([0-9A-F]{2})\/event\/done/;
    
    //event topic
    if(String(topic).match(rec_topic)){
        let topic_sensor = getMacFromTopic(topic, 2);
        let payload_json = messageJSON(message);
        console.log(payload_json);
        if(payload_json["data"] == null){
            console.log("SENSOR");                
            if(bytes_object[topic_sensor] == null){               
                bytes_object[topic_sensor] = new Object();
                bytes_object[topic_sensor]["json"] = payload_json;
                bytes_object[topic_sensor]["arr"] = new Array();
                bytes_object[topic_sensor]["byte_arr"] = new Uint8Array(8);
                bytes_object[topic_sensor]["hex_string"] = "";
                bytes_object[topic_sensor]["count"] = 0;       
            }
            else if(bytes_object[topic_sensor] != null){
                bytes_object[topic_sensor]["json"] = payload_json;
                bytes_object[topic_sensor]["arr"] = new Array();
                bytes_object[topic_sensor]["byte_arr"] = new Uint8Array(8);
                bytes_object[topic_sensor]["hex_string"] = "";
                bytes_object[topic_sensor]["count"] = 0;     
            }
            //client.publish(topic.replace(/data/g, "ack"), "ok", {qos : 2});
            return;
        }
        //SIMULATION Exception
        let tmp_json = new Object();
        try{
            tmp_json = createWav(payload_json);
        }catch(err){
            console.log(err);
        }
        
        let values = {
        sensor_id: payload_json["sensor_id"],
        timestamp: new Date(payload_json["timestamp"] * 1000).toISOString().slice(0, 19).replace('T', ' '),
        sample_rate: payload_json["sample_rate"],
        sample_size: payload_json["sample_size"],
        data: tmp_json["wav"].toBuffer(),
        filename: tmp_json["filename"]
        };
        return;
        con.query(query, values, function(error, results){
            if(error){
                console.log(error);
                throw error;
                console.log("ERRO");
                return;
            }
            return;
        });
    //config topic
    }else if(String(topic).match(conf_topic)){
        let topic_sensor = getMacFromTopic(topic, 1);
        let payload_json = messageJSON(message);
        console.log("Config msg");
        let conf = new Object();
        if(payload_json["data"] == null){
            console.log("SENSOR");                
            if(bytes_object[topic_sensor] == null){               
                bytes_object[topic_sensor] = new Object();
                bytes_object[topic_sensor]["json"] = payload_json;
                bytes_object[topic_sensor]["arr"] = new Array();
                bytes_object[topic_sensor]["byte_arr"] = new Uint8Array(8);
                bytes_object[topic_sensor]["hex_string"] = "";
                bytes_object[topic_sensor]["count"] = 0;       
            }
        }
        console.log("Payload: " + JSON.stringify(payload_json));
        sel_query = "select * from `" + db + "`.`sensors` where sensor_mac = '" + payload_json["sensor_mac"] + "';";
        con.query(sel_query, function(err, rows){
            if(err){
                console.log(err);
                return;
            }
            else if(rows.length > 0){
                conf["sensor_mac"] = rows[0]["sensor_mac"];
                conf["sample_rate"] = rows[0]["sample_rate"];
                conf["sample_size"] = rows[0]["sample_size"];
                console.log("selected conf: " + JSON.stringify(conf));
                client.publish(topic.replace(/registration/g, "config"), JSON.stringify(conf), {qos : 2});
                console.log("Conf sent");
            }else{
                return;
            }
        });
        turn_on_query = "UPDATE `" + db + "`.`sensors` SET status = 2 where sensor_mac = '" + payload_json["sensor_mac"] + "';";
        con.query(turn_on_query, function(err, rows){
            if(err){
                console.log("Turn on query error");
                console.log(err);
                throw err;
                return;
            }
        });
    //double array topic
    }else if(String(topic).match(bin_topic)){
        let topic_sensor = getMacFromTopic(topic, 3);
        
        let tmp = Buffer.from(message);
        bytes_object[topic_sensor]["hex_string"] = message.toString('hex');
        for (var i = 0; i < bytes_object[topic_sensor]["hex_string"].length; i += 2) {
            bytes_object[topic_sensor]["byte_arr"][i / 2] = parseInt(bytes_object[topic_sensor]["hex_string"].substring(i, i + 2), 16)
        }
        tmp_buf = new Buffer.from(bytes_object[topic_sensor]["byte_arr"].buffer);
        //console.log(tmp_buf.readDoubleLE());
        bytes_object[topic_sensor]["arr"].push(tmp_buf.readDoubleLE());
        bytes_object[topic_sensor]["count"]++;
        bytes_object[topic_sensor]["hex_string"] = "";
        bytes_object[topic_sensor]["hex_string"] += message.toString('hex');
    
    //double array send complete topic
    }else if(String(topic).match(event_done_topic)){
        let topic_sensor = getMacFromTopic(topic, 4); 
        console.log("Done Received[" + topic.replace(/done/g, "ack") + "]{" + topic_sensor + "}");
        client.publish(topic.replace(/done/g, "ack"), "ok", {qos : 2});
        console.log(bytes_object);
        if(bytes_object[topic_sensor] == null){
            console.log("EXITING");
            return;
        }else if(bytes_object[topic_sensor] != null){
            console.log("Going to inserted");
            bytes_object[topic_sensor]["data"] = new Array();
            bytes_object[topic_sensor]["data"] = bytes_object[topic_sensor]["arr"];
            console.log("Lenght: " + bytes_object[topic_sensor]["data"].length);
            bytes_object[topic_sensor]["json"]["data"] = bytes_object[topic_sensor]["data"];
            console.log(bytes_object[topic_sensor]["json"]);
            insertDB(bytes_object[topic_sensor]["json"]);
            console.log("Inserted")
            return;
        }
    }else if(String(topic).match(ack_conf_topic)){
        let mac_str = getMacFromTopic(topic, 5);
        console.log("Validated: " + mac_str);
        turn_on_query = "UPDATE `" + db + "`.`sensors` SET status = 1 where sensor_mac = '" + mac_str + "';";
        con.query(turn_on_query, function(err, rows){
            if(err){
                console.log("Turn on query error");
                console.log(err);
                throw err;
                return;
            }
        });
    //Topico dos testes de envio
    }else if(String(topic).match(/^timing\//)){
        console.log("Timing event");
        let msg_json = messageJSON(message);
        let values = {
            time: msg_json["time"],
        };
        let query_time = "INSERT INTO `projeto`.`timing_" + msg_json["numb_samples"] + "` SET ?";
        con.query(query_time, values, function(error, results){
            if(error){
                console.log(error);
                throw error;
                console.log("ERRO");
                return;
            }
            return;
        });
    }
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
/**
 * Parses the sensor message and extracts the json 
 */
function messageJSON(message){
    console.log("Parsing JSON");
    let payload = message.toString('utf8');
    let msg_json = new Object();
    try{
      msg_json = JSON.parse(payload.slice(0, payload.length-1));
    }catch(err){
        if(err instanceof SyntaxError){
            try{
                msg_json = JSON.parse(payload.slice(0, payload.length));
            }catch(err){
                console.log(payload);
                console.log(err);
            }
        }
    }
    return msg_json;
}
/**
 * Creates the wav file from the received event 
 */
function createWav(event){
    console.log("Creating WAV");
    let wav = new WaveFile();
    let pas_val = 101.325;
    let arr = [];
    for(val in event["data"]){
        arr.push(parseFloat(event["data"][val]) / pas_val );
    }
    
// Create a mono wave file with the desired sample rate, 32-bit and 4 samples
    wav.fromScratch(1, parseInt(event["sample_rate"]), '32f', arr);
    fs.writeFileSync("./wav_files/" + event["sensor_mac"] + "_" + event["timestamp"] + ".wav", wav.toBuffer());
    let tmp = new Object();
    tmp["wav"] = wav;
    tmp["filename"] = event["sensor_mac"] + "_" + event["timestamp"] + ".wav";
    return tmp;
}
/**
 * Inserts the event in the database 
 */
function insertDB(payload_json){
    let tmp_json = new Object();
        try{
            tmp_json = createWav(payload_json);
        }catch(err){
            console.log(err);
        }
        
        let values = {
        sensor_descriptor: "d1",
        sensor_mac: payload_json["sensor_mac"],
        timestamp: new Date(payload_json["timestamp"] * 1000).toISOString().slice(0, 19).replace('T', ' '),
        sample_rate: payload_json["sample_rate"],
        sample_size: payload_json["sample_size"],
        data: tmp_json["wav"].toBuffer(),
        filename: tmp_json["filename"]
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
}
