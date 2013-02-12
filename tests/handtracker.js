var s2web = require("node-sweatintotheweb/handtracker");
var frameLoop = null;

process.on('exit', function() {
    s2web.close();
});

s2web.context.on = function(name) {
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
    console.log(s2web.getHands());
}

s2web.init();