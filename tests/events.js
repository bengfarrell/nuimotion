// Events/Gestures test
//
// Activity will listen for skeleton start/stop tracking
// Also listen for left/right swipes and waves

var nuimotion = require("nuimotion");

nuimotion.addListener(
    [ nuimotion.Events.SKELETON_TRACKING,
        nuimotion.Events.SKELETON_STOPPED_TRACKING ],
    onEvent );

nuimotion.addGesture(
    [ nuimotion.Events.Gestures.Swipe.types.left,
        nuimotion.Events.Gestures.Swipe.types.right,
        nuimotion.Events.Gestures.Wave.types.left,
        nuimotion.Events.Gestures.Wave.types.right,
        nuimotion.Events.Gestures.Wave.types.any /* will not fire if left and right waves are present */ ],
    onGesture);

nuimotion.init();

/**
 * listen for Node process shutdown and close NUIMotion appropriately
 */
process.on('exit', function() {
    nuimotion.close();
});

/**
 * on general event (user/device/etc)
 * @param eventType
 */
function onEvent(eventType) {
    console.log(eventType);
}

/**
 * on gesture event
 * @param gesture
 */
function onGesture(gesture) {
    console.log(gesture)
}