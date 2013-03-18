var nui = require("nuimotion");

nui.context.on = function(name) {
    console.log("Event: " + name);
};

process.on('exit', function() {
    nui.close();
});

nui.init();

nui.addGestureListener("GESTURE_CATEGORY_SWIPE", "GESTURE_SWIPE_RIGHT");