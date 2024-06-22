import {initializeChart, addDataToChart} from './js_scripts/chart.js'
import {firebaseRef1, firebaseRef2, firebaseRef3, firebaseRef4, firebaseRef5, firebaseRef6, firebaseRef7} from './js_scripts/config_firebase.js'
import {updateValues} from './js_scripts/getData.js'
import {listen_gyro} from './js_scripts/getGyroscopeData.js'

function updateChartFromFirebase(firebaseRef, chart) {
  firebaseRef.on('value', function (snapshot) {
    const data = snapshot.val();
    const label = new Date().toLocaleTimeString();
    addDataToChart(chart, label, data);
  });
}

  var chart2 = initializeChart('chart_2', 'Humidity', 'rgba(0, 255, 255, 0.5)', 'rgba(0, 255, 255, 1)');
  var chart3 = initializeChart('chart_3', 'Metan Gas', 'rgba(255, 255, 255, 0.5)', 'rgba(255, 255, 255, 1)');
  var chart4 = initializeChart('chart_4', 'LDR', 'rgba(255, 255, 0, 0.5)', 'rgba(255, 255, 0, 1)');


  updateValues(firebaseRef1, document.querySelector('.temperature-data'));
  updateValues(firebaseRef2, document.querySelector('.humidity-data'));
  updateValues(firebaseRef3, document.querySelector('.metangas-data'));
  updateValues(firebaseRef4, document.querySelector('.ldr-data'));

  updateValues(firebaseRef5, document.querySelector('.gyro-x'));
  updateValues(firebaseRef6, document.querySelector('.gyro-y'));
  updateValues(firebaseRef7, document.querySelector('.gyro-z'));

  updateValues(firebaseRef5, document.querySelector('.radians-x'));
  updateValues(firebaseRef6, document.querySelector('.radians-y'));
  updateValues(firebaseRef7, document.querySelector('.radians-z'));




  updateChartFromFirebase(firebaseRef2, chart2);
  updateChartFromFirebase(firebaseRef3, chart3);
  updateChartFromFirebase(firebaseRef4, chart4);
  listen_gyro();

