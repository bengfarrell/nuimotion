// Skeleton Update Test
//
// Activity will query both hands (left and right)

var nuimotion = require("nuimotion");

// start the skeleton listener, add the joints a callback and (optionally) a frame rate in milliseconds for checking
nuimotion.startSkeletonListener( [
    nuimotion.Joints.LEFT_HAND,
    nuimotion.Joints.RIGHT_HAND ],
    onSkeletonUpdate /* , 50 (the default) */ );

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