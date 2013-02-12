/*
-- HAND TRACKER ACTIVITY --
In this activity, unlike "Follow the Hand", the user is not required to wave to the 
camera for their hand to be tracked.  In fact, this activity tracks both hands.  
Simply by entering the scene, if the user is visible and their skeleton is able to be tracked, 
both left and right hand coordinates can be obtained.
*/
var s2web = require("node-sweatintotheweb/handtracker");
var frameLoop = null;

process.on('exit', function() {
    // shuts down the NiTE middleware properly
    s2web.close();
});

s2web.context.on = function(name) {
    /*
        The following events are dispatched:
        "NEW_USER", "USER_IS_VISIBLE", "USER_IS_OUT_OF_SCENE", "USER_IS_LOST",
        "SKELETON_STOPPED_TRACKING", "SKELETON_CALIBRATING",
        "SKELETON_TRACKING", "SKELETON_CALIBRATION_FAILED",
        "DEVICE_INITIALIZED", "DEVICE_ERROR"
     */
    switch (name) {
        case "SKELETON_TRACKING":
            if (!frameLoop) {
                console.log("Start tracking");
                frameLoop = setInterval(onUpdate,500);
            }
            break;

        case "SKELETON_STOPPED_TRACKING":
            if (frameLoop) {
                console.log("Stop tracking");
                clearInterval(frameLoop);
                frameLoop = null;
            }
    }
};

function onUpdate() {
    hands = s2web.getHands();
    if (hands.right_hand.active) {
        console.log( "Right x: "+ hands.right_hand.x + ", Right y: " + hands.right_hand.y + ", Right z: " + hands.right_hand.z);
    }

    if (hands.left_hand.active) {
        console.log( "Left x: "+ hands.left_hand.x + ", Left y: " + hands.left_hand.y + ", Left z: " + hands.left_hand.z);
    }
}

// make sure to call this after setting the context.on event
// I have to do runtime checking to make sure the on callback handler exists and I haven't done that yet
s2web.init();