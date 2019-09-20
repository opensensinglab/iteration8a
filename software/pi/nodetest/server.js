'use strict';

/* 
 * Include Statements
 */
const execSync = require('child_process').execSync;
var async = require('async');
var express = require("express");
var _ = require("lodash");
var bodyParser = require('body-parser');
var fs = require('fs');
var dateTime = require('node-datetime');
var url = require('url');

/*
 * Express Configurations
 */
var app = express();
app.use("/", express.static(__dirname, +"/www"));
app.use("/css", express.static(__dirname + "/node_modules/ui-bootstrap/vendor/ui-bootstrap"));
app.use("/js", express.static(__dirname + "/node_modules/angular"));
app.use("/js", express.static(__dirname + "/js"));
app.use(bodyParser.urlencoded({
    limit: '1mb',
    extended: true,
}));
app.use(bodyParser.json({
    limit: '50mb' // Use this to limit bandwidth on server (graph currently needs min 10MB)
}))

/*
 * Server Variables/Data 
 */
var PORT = 8080;            // HTTP port for server (usually 80 or 8080 -- must match client)

// Start listening
var server = app.listen(PORT, function () {
    var host = server.address().address;
    var port = server.address().port;
    console.log("App listening at http://%s:%s", host, port);
});


/* 
 * Utilities
 */ 
        function mkRandom2DArray(xSize, ySize) {
                var out = [];
                for (var i=0; i<ySize; i++) {
                        out[i] = [];
                        for (var j=0; j<xSize; j++) {
                                out[i][j] = (Math.random() * 100);
                        }
                }
                return out;
        }


/*
 * Server requests
 */
app.get("/GetRandom", function(req, res) {
    console.log("GET request for Random Data")

//    var thermalImage = [[1, 20, 30], [20, 1, 60], [30, 60, 1]];
	var thermalImage = mkRandom2DArray(80, 60);

    var data = [{
        z: thermalImage,
	type: 'heatmap',
    }];

    res.status(200).send(data)
})


app.get("/GetThermal", function(req, res) {
	console.log("GET request for Thermal Data")
	const { spawn } = require('child_process');
        const pyProg = spawn('python', ['../thermalZero/string_outJSON.py']);

        console.log("About to start...");

        pyProg.stdout.on('data', function(data) {
                console.log(data.toString());
//              console.log("data");
//              console.log(data);
                var imageData = JSON.parse(data.toString());
//              console.log(imageData);
//              return imageData;

                var data = [{
                        z: imageData,
                        type: 'heatmap',
                    }];

                res.status(200).send(data)

        });
})


app.get("/GetMagnetic", function(req, res) {
    console.log("GET request for Magnetic Data")

	const { spawn } = require('child_process');
	const pyProg = spawn('python', ['../magtilev3/readmagtile.py']);
	
	console.log("About to start...");

	pyProg.stdout.on('data', function(data) {
		console.log(data.toString());
//		console.log("data");
//		console.log(data);
		var imageData = JSON.parse(data.toString());
//		console.log(imageData);
//		return imageData;

		var data = [{
		        z: imageData,
			type: 'heatmap',
		    }];

		res.status(200).send(data)

	});

//	console.log("Completed");
})


app.get("/GetMagnetic1", function(req, res) {
    console.log("GET request for Magnetic Data")

//    var thermalImage = [[1, 20, 30], [20, 1, 60], [30, 60, 1]];
	var magneticImage = readMagneticImage();
console.log("Out:");
console.log(magneticImage);

//	var magneticImage = mkRandom2DArray(8, 8);
    var data = [{
        z: magneticImage,
	type: 'heatmap',
    }];

    res.status(200).send(data)

})
