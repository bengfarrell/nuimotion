/*
-- FOLLOW THE HAND ACTIVITY --
This activity requires the user to wave at the camera.  
Once this gesture is captured ("Gesture_Wave"), the hand used to wave is registered as the active hand.  
Coordinates for this hand can be obtained by getHand().  
When the hand is lost, an event is dispatched to Node: "Gesture_LostHand".  
The user must then wave again for the hand to be re-registered for use.
*/

var s2web = require("node-sweatintotheweb/followthehand");
var frameLoop;

s2web.context.on = function(name) {
    /*
        The following events are dispatched:
        "Gesture_Wave", "Gesture_LostHand"
     */
    console.log("Gesture: " + name);

    if (!frameLoop) {
        frameLoop = setInterval(onUpdate,500);
    }
};

function onUpdate() {
    var hand = s2web.getHand();
    // x,y,z coordinates will return -9999 each if hand is not currently tracking
    console.log( "x: "+ hand.x + ", y: " + hands.y + ", z: " + hand.z);
}

process.on('exit', function() {
    // shuts down the NiTE middleware properly
    s2web.close();
});

// make sure to call this after setting the context.on event
// I have to do runtime checking to make sure the on callback handler exists and I haven't done that yet
s2web.init();