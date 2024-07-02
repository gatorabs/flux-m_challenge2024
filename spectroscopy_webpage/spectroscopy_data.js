import { firebaseRef3, firebaseRef4 } from "../js_scripts/config_firebase.js";
import { updateValues } from "../js_scripts/getData.js"
import {initializeChart, addDataToChart} from '../js_scripts/chart.js'


function updateChartFromFirebase(firebaseRef, chart) {
    firebaseRef.on('value', function (snapshot) {
      const data = snapshot.val();
      const label = new Date().toLocaleTimeString();
      addDataToChart(chart, label, data);
    });
  }


  

var chart_1 = initializeChart('line', 'chart1', 'LDR', 'rgba(255, 255, 0, 0.5)', 'rgba(255, 255, 0, 1)');
var chart_2 = initializeChart('line', 'chart2', 'Metan Gas', 'rgba(255, 255, 255, 0.5)', 'rgba(255, 255, 255, 1)');
var chart_3 = initializeChart('polarArea', 'chart3', 'Metan Gas & LDR', 'rgba(255, 255, 255, 0.5)', 'rgba(255, 255, 255, 1)');

updateValues(firebaseRef4, document.querySelector('.ldr-data'));
updateValues(firebaseRef3, document.querySelector('.metangas-data'));

updateChartFromFirebase(firebaseRef4, chart_1);
updateChartFromFirebase(firebaseRef3, chart_2);
updateChartFromFirebase(firebaseRef4, chart_3);

