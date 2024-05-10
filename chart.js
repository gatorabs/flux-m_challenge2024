  var ctx = document.getElementById('myChart').getContext('2d');
  var chart = new Chart(ctx, {
    type: 'line',
    data:{
      labels: ['1', '2', '3', '4', '5'],
      datasets: [{
        label: 'My',
        backgroundColor: 'rgb(255,255,255)',
        borderColor: 'rgb(255,0,255)',
        data: [0,10,5,2,20,30,45]
      }]
    },

    options: {}
  })