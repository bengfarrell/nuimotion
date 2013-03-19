var nui = require("nuimotion");
nui.context.on = function(name) {
    console.log("Event: " + name);
};

process.on('exit', function() {
    nui.close();
});

nui.addGestureListener(Gestures.Swipe.SWIPE, Gestures.Swipe.types.right);
nui.addGestureListener(Gestures.Swipe.SWIPE, Gestures.Swipe.types.left);
nui.addGestureListener(Gestures.Wave.WAVE, Gestures.Wave.types.left);
nui.addGestureListener(Gestures.Wave.WAVE, Gestures.Wave.types.right);

// next line is unused if left and right waves are in play
nui.addGestureListener(Gestures.Wave.WAVE, Gestures.Wave.types.any);

nui.init();
