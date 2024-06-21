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


  export var firebaseRef1 = firebase.database().ref("DHT11/Temperature");
  export var firebaseRef2 = firebase.database().ref("DHT11/Humidity");
  export var firebaseRef3 = firebase.database().ref("MQ4");
  export var firebaseRef4 = firebase.database().ref("LDR");
  export var firebaseRef5 = firebase.database().ref("GYRO/x");