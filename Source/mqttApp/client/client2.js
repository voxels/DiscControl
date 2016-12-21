var mqtt = require('mqtt')
 
var options = {
    host: "localhost",
    port: 1883
  };

client = mqtt.connect(options);
client.subscribe('presence');
 
client.on('message', function(topic, message) {
  console.log(message);
});
 
console.log('Client started...');