var mqtt = require('mqtt');

var options = {
    host: "localhost",
    port: 1883
  };

client = mqtt.connect(options);

client.subscribe('presence');

console.log('Client publishing.. ');
client.publish('presence', 'Client 1 is alive.. Test Ping! ' + Date());

client.end();

