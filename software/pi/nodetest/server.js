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
const process = require('process');

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
 * Serial Port
 */
const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')
const port = new SerialPort("/dev/serial0", {
	baudRate: 115200,
});
const parser = port.pipe(new Readline({ delimiter: '\r\n' }))

// Read port data
var dataCO2 = [];
var dataRadiation = [];
var dataAirParticles = [];

port.on("open", function() {
	console.log("Serial Port Opened");
	parser.on('data', function(data) {
		console.log("Serial Data: " + data);
		var data = JSON.parse(data);
		if ('radiation' in data) {
			dataRadiation.push(data);
		}
		if ('co2' in data) {
			dataCO2.push(data);
		}
		if ('airparticles' in data) {
			dataAirParticles.push(data);
		}
	});
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


app.get("/GetCo2", function(req, res) {
    console.log("GET request for Co2 data")

	var xData = [];
	var yData = [];
	for (var i=0; i<dataCO2.length; i++) {
		xData.push(i);
		yData.push( dataCO2[i].co2.co2ppm );
	}

	var trace1 = {
		x: xData,
		y: yData,
	 	mode: 'lines+markers',
	 	type: 'scatter'
	};

//	console.log(trace1);

    var data = [trace1];

    res.status(200).send(data)
})

app.get("/GetRadiation", function(req, res) {
    console.log("GET request for Radiation data")

	// CPM
        var xData = [];
        var yData = [];
        for (var i=0; i<dataRadiation.length; i++) {
                xData.push(i);
                yData.push( dataRadiation[i].radiation.cpm );
        }

        var trace1 = {
                x: xData,
                y: yData,
                mode: 'lines+markers',
                type: 'scatter'
        };

	// Histogram
	var histData = [];
	var histBinSize = 0;
	var histBins = [];
	if (dataRadiation.length > 1) {
		histData = dataRadiation[dataRadiation.length-1].radiation.hist;
		histBinSize = dataRadiation[dataRadiation.length-1].radiation.histBinSize;
		for (var i=0; i<histData.length; i++) {
			histBins[i] = (i+1) * histBinSize;
		}
	}


	// Pack
	var cpmData = [trace1];
	var histData = [{x: histBins, y: histData, type: 'bar'}];
	
	console.log(histData);

	var data = {cpmData:cpmData, histData:histData};

	// Send
	res.status(200).send(data)
})

app.get("/GetAirParticles", function(req, res) {
    console.log("GET request for Air Particle data")

        var xData = [];
        var yDataPM1p0 = [];
        var yDataPM2p5 = [];
        var yDataPM4p0 = [];
        var yDataPM10p0 = [];
        for (var i=0; i<dataCO2.length; i++) {
                xData.push(i);
                yDataPM1p0.push( dataAirParticles[i].airparticles.pm1p0 );
                yDataPM2p5.push( dataAirParticles[i].airparticles.pm2p5 );
                yDataPM4p0.push( dataAirParticles[i].airparticles.pm4p0 );
                yDataPM10p0.push( dataAirParticles[i].airparticles.pm10p0 );
        }

        var trace1 = {
                x: xData,
                y: yDataPM1p0,
                mode: 'lines+markers',
                type: 'scatter'
        };

        var trace2 = {
                x: xData,
                y: yDataPM2p5,
                mode: 'lines+markers',
                type: 'scatter'
        };

        var trace3 = {
                x: xData,
                y: yDataPM4p0,
                mode: 'lines+markers',
                type: 'scatter'
        };

        var trace4 = {
                x: xData,
                y: yDataPM10p0,
                mode: 'lines+markers',
                type: 'scatter'
        };


    var data = [trace1, trace2, trace3, trace4];

    res.status(200).send(data)
})





var thermalFrame = [];
var thermalStr = "";
var thermalEnabled = false;

function enableCamera() {
        console.log("GET request for Thermal Data")
        const { spawn } = require('child_process');
        const pyProg = spawn('sudo', ['python', '../thermalZero/string_outJSON.py'], {uid:process.getuid(), gid:process.getgid()});

        console.log("About to start...");

	thermalEnabled = true;
	thermalStr = "";

        pyProg.stdout.on('data', function(data) {
		thermalStr += data.toString();
//                console.log("############################");
//                console.log(data.toString());

		var lines = thermalStr.split("\n");
		if (lines.length > 2) {
			var secondLastLine = lines[lines.length-2];
//			console.log("SECOND LAST LINE: ");
//			console.log(secondLastLine);
			if (secondLastLine.startsWith("[")) {
				var imageData = JSON.parse(secondLastLine);
				thermalFrame = imageData;
			}
		}
	})

	pyProg.on('exit', function(code) {
		console.log("Exiting camera reads. ");
		thermalEnabled = false;
	});

}

app.get("/GetThermal", function(req, res) {
	console.log("GET request for Thermal Data")

	if (thermalEnabled == false) {
		enableCamera();
	}

        var data = [{
		z: thermalFrame,
		type: 'heatmap',
	}];

	res.status(200).send(data)

})

app.get("/GetThermal1", function(req, res) {
	console.log("GET request for Thermal Data")
	const { spawn } = require('child_process');
        const pyProg = spawn('sudo', ['python', '../thermalZero/string_outJSON.py'], {uid:process.getuid(), gid:process.getgid()});

        console.log("About to start...");

        pyProg.stdout.on('data', function(data) {
		console.log("############################");
                console.log(data.toString());
//              console.log("data");
//              console.log(data);


//#                var imageData = JSON.parse(data.toString());

//              console.log(imageData);
//              return imageData;

/*
                var data = [{
                        z: imageData,
                        type: 'heatmap',
                    }];
*/

//#                res.status(200).send(data)

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
