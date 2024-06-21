export function updateValues(firebaseRef, dataTag) {
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

  
