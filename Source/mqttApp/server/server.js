var mosca = require('mosca')

var settings = {
	port:1883
};

var server = new mosca.Server(settings)
server.on('ready', setup);

function setup(){
	console.log('Mosca server is up and running');
}

server.on('clientConnected', function(client){
	console.log('client connected', client.id);
});

server.on('published', function(packet, client){
	console.log('published :', packet.payload );
});

server.on('subscribed', function(topic, client){
	console.log('subscribed: ', topic);
});

server.on('unsubscribed', function(topic, client){
	console.log('unsubscribed: ', topic);
});

server.on('clientDisconnecting', function(client){
	console.log('clientDisconnecting: ', client.id);
});

server.on('clientDisconnected', function(client){
	console.log('clientDisconnected: ', client.id);	
});

server.published = function(packet, client, cb) {
  if (packet.topic.indexOf('echo') === 0) {
    return cb();
  }
 
  var newPacket = {
    topic: 'echo/content',
    payload: packet.payload,
    retain: packet.retain,
    qos: packet.qos
  };
 
  console.log('received topic:', newPacket.topic, '\nmessage:', newPacket.payload.toString());
  
  server.publish(newPacket, cb);
}