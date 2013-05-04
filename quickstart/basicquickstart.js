/*

IMPORTANT: THIS JS CODE IS NOT DESIGNED TO RUN FROM THIS FOLDER
COPY AND PASTE THIS CODE TO YOUR MAIN JAVASCRIPT PROJECT

*/


var nuimotion = require("nuimotion");

// start the skeleton listener, add the joints a callback and (optionally) a frame rate in milliseconds for checking
nuimotion.startSkeletonListener( [
    nuimotion.Joints.LEFT_HAND,
    nuimotion.Joints.RIGHT_HAND ],
    onSkeletonUpdate /* , 50 (the default) */ );

nuimotion.addListener(
    [ nuimotion.Events.SKELETON_TRACKING,
        nuimotion.Events.SKELETON_STOPPED_TRACKING ],
    onEvent );

nuimotion.addGesture(
    [ nuimotion.Events.Gestures.Swipe.types.up,
        nuimotion.Events.Gestures.Swipe.types.down,
        nuimotion.Events.Gestures.Swipe.types.left,
        nuimotion.Events.Gestures.Swipe.types.right,
        nuimotion.Events.Gestures.Wave.types.hand],
    onEvent);
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
    console.log(skeleton);
}

/**
 * on general event (user/device/gesture/etc)
 * @param event
 */
function onEvent(event) {
    if (event.eventType == nuimotion.Events.GESTURE) {
        console.log("Gesture: " + event.gestureType + " Hand: " + event.hand + " State: " + event.step);
    } else {
        console.log("Event: " + event.eventType);
    }
}