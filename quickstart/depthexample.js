var nuimotion_d = require("nuimotion/depth");
nuimotion_d.init();

var i = setInterval(function() {
    var frame = nuimotion_d.getDepthFrame(); // or getRGBFrame
    writeImage(frame);
}, 100);

function writeImage(frame) {
    var Canvas = require('canvas')
        , canvas = new Canvas(320,240)
        , ctx = canvas.getContext('2d');

    clrs = 3;
    if(frame.type == "depth") {
        clrs = 1;
    }
    var buf = frame.data;
    var y = 0;
    var x = 0;
    var c = 0;
    for (ii = 0; ii < buf.length; ii+=clrs) {
        if (c >= frame.width) {
            c = 0;
            x = 0;
            y ++;
        }

        var r = buf.readUInt8(ii) *25;
        var g = r;
        var b = r;

        // if RGB, then 3x the buffer read for each color
        if (clrs == 3) {
            g = buf.readUInt8(ii +1) *25;
            b = buf.readUInt8(ii +2) *25;
        }

        // looks like depth data is interlaced (I think, so take every other line for a picture)
        if (y % 2 == 0 || clrs == 3) {
            ctx.fillStyle = 'rgba('+ r + ',' + g + ',' + b + ', 1)';
            ctx.beginPath();
            ctx.arc(x, y, 25, 0, 2 * Math.PI, true);
            ctx.fill();
        }
        c++;
        x++;
    }

    var fs = require('fs')
        , out = fs.createWriteStream(__dirname + '/depth.png')
        , stream = canvas.pngStream();

    stream.on('data', function(chunk){
        out.write(chunk);
    });

    stream.on('end', function(){
        console.log('saved png');
        setInterval(function() { nuimotion_d.close(); }, 500)
    });

}