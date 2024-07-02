export function initializeChart(type, chartId, label, backgroundColor, borderColor) {
    var ctx = document.getElementById(chartId).getContext('2d');
    return new Chart(ctx, {
      type: type,
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


export function addDataToChart(chart, label, data) {
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