// Import Angular module for the grid controller
var InferenceGraph = angular.module("InferenceGraph", [])

console.log("test1");

InferenceGraph.controller("MainController", function($scope, $compile, $http, $window, $timeout) {
	console.log("Start");


	/*
	 * Main
	 */

	function refreshCo2() {

		var layout = {};

		$http.get("/GetCo2").then(function(resp) {
//		$http.get("/GetThermal").then(function(resp) {
        		var data1 = resp.data;
			console.log(data1);

//			Plotly.plot('myDiv', data1, layout, {showSendToCloud: true});
			Plotly.newPlot('myDiv', data1, layout, {showSendToCloud: false});
			console.log("Plot completed");
	        });

		setTimeout(refreshCo2, 1000);
	}

	// Main
	refreshCo2();


})
