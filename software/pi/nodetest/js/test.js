// Import Angular module for the grid controller
var InferenceGraph = angular.module("InferenceGraph", [])

console.log("test1");

InferenceGraph.controller("MainController", function($scope, $compile, $http, $window, $timeout) {
	console.log("Start");


	/*
	 * Main
	 */

	var data = [
	  {
	    z: [[1, 20, 30], [20, 1, 60], [30, 60, 1]],
	    type: 'heatmap'
	  }
	];

//	var data1 = [{}];

//	Plotly.plot('myDiv', data, {}, {showSendToCloud: true});

	function refreshThermal() {

		var layout = {
		  title: 'Annotated Heatmap',
		  annotations: [],
		  xaxis: {
		    ticks: '',
		    side: 'top'
		  },
		  yaxis: {
		    ticks: '',
		    ticksuffix: ' ',
		    width: 700,
		    height: 700,
		    autosize: false
		  }
		};

		$http.get("/GetThermal").then(function(resp) {
        		var data1 = resp.data;
			console.log(data1);

//			Plotly.plot('myDiv', data1, layout, {showSendToCloud: true});
			Plotly.plot('myDiv', data1, layout, {showSendToCloud: false});
			console.log("Plot completed");
	        });

		setTimeout(refreshThermal, 1000);
	}

	// Main
	refreshThermal();


})
