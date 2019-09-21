// Import Angular module for the grid controller
var InferenceGraph = angular.module("InferenceGraph", [])

console.log("test1");

InferenceGraph.controller("MainController", function($scope, $compile, $http, $window, $timeout) {
	console.log("Start");


	/*
	 * Main
	 */

	function refreshRadiation() {

		var layout = {};

		$http.get("/GetRadiation").then(function(resp) {
//		$http.get("/GetThermal").then(function(resp) {
        		var data1 = resp.data;
			console.log(data1);

			var cpmData = data1.cpmData;
			var histData = data1.histData;

//			Plotly.plot('myDiv', data1, layout, {showSendToCloud: true});
			Plotly.newPlot('myDiv1', cpmData, layout, {showSendToCloud: false});

			Plotly.newPlot('myDiv2', histData);

			console.log(histData);
			console.log("Plot completed");
	        });

		setTimeout(refreshRadiation, 1000);
	}

	// Main
	refreshRadiation();


})
