import {initializeChart, addDataToChart} from '../../infrastructure/chart/ChartUpdater.js'
import {firebaseRef1, firebaseRef2, firebaseRef3, firebaseRef4, firebaseRef5, firebaseRef6, firebaseRef7, firebaseRef8,firebaseRef9} from '../../infrastructure/firebase/firebaseConfig.js'
import {updateValues} from '../../domain/services/DataFetcher.js'
import { listen_gyro } from '../../infrastructure/gyroscope/GyroListener.js';


function updateChartFromFirebase(firebaseRef, chart) {
  firebaseRef.on('value', function (snapshot) {
    const data = snapshot.val();
    const label = new Date().toLocaleTimeString();
    addDataToChart(chart, label, data);
  });
}

  var chart2 = initializeChart('line', 'chart_2', 'Humidity', 'rgba(0, 255, 255, 0.5)', 'rgba(0, 255, 255, 1)');
  var chart3 = initializeChart('line', 'chart_3', 'Metan Gas', 'rgba(255, 255, 255, 0.5)', 'rgba(255, 255, 255, 1)');
  var chart4 = initializeChart('line', 'chart_4', 'LDR', 'rgba(255, 255, 0, 0.5)', 'rgba(255, 255, 0, 1)');


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

  updateValues(firebaseRef8, document.querySelector('.fire-data'));
  updateValues(firebaseRef9, document.querySelector('.density-data'));

  updateChartFromFirebase(firebaseRef2, chart2);
  updateChartFromFirebase(firebaseRef3, chart3);
  updateChartFromFirebase(firebaseRef4, chart4);

  
  listen_gyro();

