import { firebaseRef3, firebaseRef4,firebaseRef10 } from "../../infrastructure/firebase/firebaseConfig.js";
import { updateValues } from "../../domain/services/DataFetcher.js"
import {initializeChart, addDataToChart} from '../../infrastructure/chart/ChartUpdater.js'


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
updateValues(firebaseRef10, document.querySelector('.non-fluid-data'));
updateValues(firebaseRef10, document.querySelector('.water-fluid-data'));
updateValues(firebaseRef10, document.querySelector('.diesel-fluid-data'));
updateValues(firebaseRef10, document.querySelector('.oil-fluid-data'));



updateChartFromFirebase(firebaseRef4, chart_1);
updateChartFromFirebase(firebaseRef3, chart_2);
updateChartFromFirebase(firebaseRef3, chart_3);

