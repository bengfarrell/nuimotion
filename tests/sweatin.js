/*
-- SWEATIN' HELLO WORLD ACTIVITY --
This activity doesn't do much - it simply dispatches the events that the NiTE middleware offers.
This includes Gesture_Click and Gesture_Wave.  Gesture_RaiseHand is present but commented out in the source code
because it seemed a little flaky bombarding you with raised hand events.  Gesture_Click can be a little hard to 
accomplish - but it is a result of the user extending their hand to "click" something on screen, towards the camera.
*/

var s2web = require("node-sweatintotheweb");

s2web.context.on = function(name) {
	// "Gesture_Click" and "Gesture_Wave"
    console.log(name);
};

// make sure to call this after setting the context
// I have to do runtime checking to make sure the on callback handler exists and I haven't done that yet
s2web.init();

process.on('exit', function() {
	// shuts down the NiTE middleware properly
    s2web.close();
});