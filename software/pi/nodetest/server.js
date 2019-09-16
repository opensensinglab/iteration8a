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
app.get("/GetThermal", function(req, res) {
    console.log("GET request for Thermal Data")

//    var thermalImage = [[1, 20, 30], [20, 1, 60], [30, 60, 1]];
	var thermalImage = mkRandom2DArray(80, 60);

    var data = [{
        z: thermalImage,
	type: 'heatmap',
    }];

    res.status(200).send(data)
})
