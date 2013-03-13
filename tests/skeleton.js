var nui = require("nuimotion");

var frameLoop;
var frameDelay = 50; /* milliseconds between frame loop iterations */

nui.context.on = function(name) {
    switch (name) {
        case "SKELETON_TRACKING":
            if (!frameLoop) {
                console.log("Skeleton is tracking");
                frameLoop = setInterval(onUpdate,frameDelay);
            }
            break;

        case "SKELETON_STOPPED_TRACKING":
            if (frameLoop) {
                console.log("Skeleton stopped tracking");
                clearInterval(frameLoop);
                frameLoop = null;
            }
    }
};

process.on('exit', function() {
    nui.close();
});

function onUpdate() {
   console.log(nui.getJoints("left_shoulder").left_shoulder.xRotation )
}

nui.init();