var nui = require("nuimotion");

nui.context.on = function(name) {
    console.log("Event: " + name);
};

process.on('exit', function() {
    nui.close();
});

nui.init();