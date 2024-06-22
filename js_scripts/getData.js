export function updateValues(firebaseRef, dataTag) {
    var data;
    const dataMapping = {
      'humidity-data': (data) => `${data}%`,
      'temperature-data': (data) => `${data} °C`,
      'metangas-data': (data) => `${data}%`,
      'ldr-data': (data) => `${data}%`,
      'fire-data': (data) => data === true ? "Ok." : "FIRE!",
      'density-data': (data) => `${data} kg/m³`,
      'gyro-x': (data) => `${data}°`,
      'gyro-y': (data) => `${data}°`,
      'gyro-z': (data) => `${data}°`,
      'radians-x': (data) => `${(data * (180 / Math.PI)).toFixed(0)}π`,
      'radians-y': (data) => `${(data * (180 / Math.PI)).toFixed(0)}π`,
      'radians-z': (data) => `${(data * (180 / Math.PI)).toFixed(0)}π`


    };
    firebaseRef.on('value', function (snapshot) {
      var data = snapshot.val();
      const updateFunction = dataMapping[dataTag.className];
  
      console.log(data);
  
      dataTag.innerHTML = updateFunction(data);
      

      dataTag.classList.add('blink');
      setTimeout(() => {
        dataTag.classList.remove('blink');
      }, 200); 
    });
  
  }

  
