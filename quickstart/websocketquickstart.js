/*

 IMPORTANT: THIS JS CODE IS NOT DESIGNED TO RUN FROM THIS FOLDER
 COPY AND PASTE THIS CODE TO YOUR MAIN JAVASCRIPT PROJECT

 TO RUN THIS, THE WEBSOCKET NODE ADDON IS REQUIRED

 */

var nuimotion = require("nuimotion");
var WebSocketServer = require('websocket').server;
var http = require('http');
var connections = [];

// start the skeleton listener, add the joints a callback and (optionally) a frame rate in milliseconds for checking
nuimotion.startSkeletonListener( [], onSkeletonUpdate /* , 50 (the default) */ );
nuimotion.init();

/**
 * listen for Node process shutdown and close NUIMotion appropriately
 */
process.on('exit', function() {
    nuimotion.close();
});

/**
 * skeleton update callback
 * @param skeleton
 */
function onSkeletonUpdate(skeleton) {
    for (var c in connections) {
        connections[c].sendUTF(JSON.stringify(skeleton));
    }
}

/**
 * on general event (user/device/etc)
 * @param event
 */
function onEvent(event) {
    if (event.eventType == nuimotion.Events.GESTURE) {
        console.log("Gesture: " + event.gestureType + " Hand: " + event.hand + " State: " + event.step);
    } else {
        console.log("Event: " + event.eventType);
    }
    for (var c in connections) {
        connections[c].sendUTF(JSON.stringify(event));
    }
}

/**
 * on message recieved from client
 * @param data
 */
function onMessage(data) {
    if (data.type == "utf8") {
        message = JSON.parse(data.utf8Data);
        switch (message.command) {
            case "setJoints":
                console.log("Client has request to add joints: " + message.data);
                nuimotion.jointsTracking = message.data;
                break;

            case "addListener":
                console.log("Client has request to add events: " + message.data);
                if (!nuimotion._eventCallbackDict[message.data]) {
                    nuimotion.addListener(message.data, onEvent);
                }
                break;

            case "addGesture":
                console.log("Client has request to add gestures: " + message.data);
                if (!nuimotion._gestureCallbackDict[message.data]) {
                    nuimotion.addGesture(message.data, onEvent);
                }
                break;
        }
    }
}

/**
 * listen for Node process shutdown and close NUIMotion appropriately
 */
process.on('exit', function() {
    nuimotion.close();
});


var server = http.createServer(function(request, response) {
    console.log((new Date()) + ' Received request for ' + request.url);
    response.writeHead(404);
    response.end();
});

server.listen(8080, function() {
    console.log((new Date()) + ' Server is listening on port 8080');
});

wsServer = new WebSocketServer({httpServer: server});
wsServer.on('request', function(request) {
    var cnct = request.accept();
    connections.push( cnct );
    console.log((new Date()) + ' Connection accepted.');
    cnct.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer disconnected.');
    });
    cnct.on('message', onMessage)
});