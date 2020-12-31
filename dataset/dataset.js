const fs = require("fs");
const wav = require("node-wav");

const {original_file, distances} = require('./distances.json');
const file_prefix = original_file.split("_original")[0],
      file_ext = original_file.split("_original")[1];
let original_audio_buffer = load_wav(original_file);
if(original_audio_buffer != null){
  distances.forEach(ele => mk_wav(ele));
}

function mk_wav(dist){
    let new_file_name = file_prefix + "_" + dist.id;
    let new_audio_buffer = add_distance(dist, original_audio_buffer);
    //new_audio_buffer.channelData[0] = add_distance(dist, original_audio_buffer);
    //let new_wav = wav.encode(new_audio_buffer.channelData, {sampleRate: original_audio_buffer.sampleRate, float:true, bitDepth: 16});
    
    save_wav(new_file_name, new_audio_buffer);

}
function add_distance(dist, original){
    let conversionPa = 101.325;
    let tmp_arr = [];
    original.channelData[0].forEach(function(ele){
        let tmp = ele * 101.325;
        let tmp_dist = tmp * ( 1 / dist.value);
        tmp_arr.push(tmp_dist);
    });
    return tmp_arr;
}
function load_wav(filename){
     let buff = fs.readFileSync(filename);
     return wav.decode(buff);
 }
function save_wav(filename, audioBuff){
    let writeStream = fs.createWriteStream(filename + "_" + audioBuff.length + ".csv");

    writeStream.write(filename + "\n");
    for(sample in audioBuff){
        writeStream.write('"' + audioBuff[sample] + '",\n');
    }
    /*
    fs.writeFile(filename, audioBuff, function(err){
        if(err) return console.log(err);
        console.log(filename + " Written to disk.");
        return 1;
    });*/
}
