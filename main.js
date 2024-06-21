const firebaseConfig = {
    apiKey: "AIzaSyBbZHqxDbojbNKaDfLs7uR9NR84MzQEMQw",
    authDomain: "flux-m-cca2f.firebaseapp.com",
    databaseURL: "https://flux-m-cca2f-default-rtdb.firebaseio.com",
    projectId: "flux-m-cca2f",
    storageBucket: "flux-m-cca2f.appspot.com",
    messagingSenderId: "43662843090",
    appId: "1:43662843090:web:6032014d4a08165e21610b",
    measurementId: "G-5V6EKLSVCQ"
  };



  firebase.initializeApp(firebaseConfig);


  var firebaseRef1 = firebase.database().ref("DHT11/Temperature");
  var firebaseRef2 = firebase.database().ref("DHT11/Humidity");
  var firebaseRef3 = firebase.database().ref("MQ4");
  var firebaseRef4 = firebase.database().ref("LDR");
  var firebaseRef5 = firebase.database().ref("GYRO/x");

  function updateValues(firebaseRef, dataTag) {
    var data;
    const dataMapping = {
      'humidity-data': (data) => `${data}%`,
      'temperature-data': (data) => `${data} °C`,
      'metangas-data': (data) => `${data}%`,
      'ldr-data': (data) => `${data}%`,
      'fire-data': (data) => data === true ? "Ok." : "FIRE!"

    };
    firebaseRef.on('value', function (snapshot) {
      var data = snapshot.val();
      data = snapshot.val();
      const updateFunction = dataMapping[dataTag.className];

      console.log(data);

      dataTag.innerHTML = updateFunction(data);

    });

  }

  function initializeChart(chartId, label, backgroundColor, borderColor) {
    var ctx = document.getElementById(chartId).getContext('2d');
    return new Chart(ctx, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: label,
          backgroundColor: backgroundColor,
          borderColor: borderColor,
          data: []
        }]
      },
      options: {
        scales: {
          x: {
            type: 'realtime',
            realtime: {
              delay: 2000,
              onRefresh: function (chart) {

              }
            }
          },
          y: {
            beginAtZero: true
          }
        }
      }
    });
  }
  function addDataToChart(chart, label, data) {
    chart.data.labels.push(label);
    chart.data.datasets.forEach((dataset) => {
      dataset.data.push(data);
    });
    if (chart.data.labels.length > 6) {
      chart.data.labels.shift();
      chart.data.datasets.forEach((dataset) => {
        dataset.data.shift();
      });
    }
    chart.update();
  }

  function updateChartFromFirebase(firebaseRef, chart) {
    firebaseRef.on('value', function (snapshot) {
      const data = snapshot.val();
      const label = new Date().toLocaleTimeString();
      addDataToChart(chart, label, data);
    });
  }
  var gyroX;
  firebaseRef5.on('value', function (snapshot) {
    gyroX = snapshot.val();
    cube.rotation.x = gyroX

    renderer.render(scene, camera);

  });

  let scene, camera, renderer, cube;

  function parentWidth(elem) {
    return elem.parentElement.clientWidth;
  }
  function parentHeight(elem) {
    return elem.parentElement.clientHeight;
  }
  function init3D() {
    scene = new THREE.Scene();
    scene.background = new THREE.Color(0x3F3F3F);

    camera = new THREE.PerspectiveCamera(75, parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube")), 0.1, 1000);

    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));

    document.getElementById('3Dcube').appendChild(renderer.domElement);


    const geometry = new THREE.BoxGeometry(5, 1, 4);


    var cubeMaterials = [
      new THREE.MeshBasicMaterial({ color: 0x03045e }),
      new THREE.MeshBasicMaterial({ color: 0x023e8a }),
      new THREE.MeshBasicMaterial({ color: 0x0077b6 }),
      new THREE.MeshBasicMaterial({ color: 0x03045e }),
      new THREE.MeshBasicMaterial({ color: 0x023e8a }),
      new THREE.MeshBasicMaterial({ color: 0x0077b6 }),
    ];

    const material = new THREE.MeshFaceMaterial(cubeMaterials);

    cube = new THREE.Mesh(geometry, material);
    scene.add(cube);
    camera.position.z = 5;

    //listener HERE
    cube.rotation.x = 0

    renderer.render(scene, camera);
  }

  init3D();
  var chart2 = initializeChart('chart_2', 'Humidity', 'rgba(0, 255, 255, 0.5)', 'rgba(0, 255, 255, 1)');
  var chart3 = initializeChart('chart_3', 'Metan Gas', 'rgba(255, 255, 255, 0.5)', 'rgba(255, 255, 255, 1)');
  var chart4 = initializeChart('chart_4', 'LDR', 'rgba(255, 255, 0, 0.5)', 'rgba(255, 255, 0, 1)');


  updateValues(firebaseRef1, document.querySelector('.temperature-data'));
  updateValues(firebaseRef2, document.querySelector('.humidity-data'));
  updateValues(firebaseRef3, document.querySelector('.metangas-data'));
  updateValues(firebaseRef4, document.querySelector('.ldr-data'));



  updateChartFromFirebase(firebaseRef2, chart2);
  updateChartFromFirebase(firebaseRef3, chart3);
  updateChartFromFirebase(firebaseRef4, chart4);

