var nui = require("nuimotion");
nui.context.on = function(name) {
    console.log("Event: " + name);
};

process.on('exit', function() {
    nui.close();
});

nui.addGestureListener(Events.Gestures.Swipe.SWIPE, Events.Gestures.Swipe.types.right);
nui.addGestureListener(Events.Gestures.Swipe.SWIPE, Events.Gestures.Swipe.types.left);
nui.addGestureListener(Events.Gestures.Wave.WAVE, Events.Gestures.Wave.types.left);
nui.addGestureListener(Events.Gestures.Wave.WAVE, Events.Gestures.Wave.types.right);

// next line is unused if left and right waves are in play
nui.addGestureListener(Events.Gestures.Wave.WAVE, Events.Gestures.Wave.types.any);

// important to init last, because anything after this declaration will not run
// though - event listeners will continue to fire
nui.init();
