// Hàm lấy dữ liệu từ server


async function fetchSensorData(collection, name = "") {
    let url = `http://192.168.4.2:3000/getData10?collection=${collection}`;
    if (name) url += `&name=${name}`;
    const response = await fetch(url);
    if (!response.ok) throw new Error("Network response was not ok");
    return await response.json();
}

// Hàm vẽ biểu đồ với Chart.js
function renderChart(canvasId, label, sensorData, color) {
    const ctx = document.getElementById(canvasId).getContext('2d');
    new Chart(ctx, {
        type: 'line',
        data: {
            labels: sensorData.map(d => new Date(d.timestamp).toLocaleTimeString()),
            datasets: [{
                label: label,
                data: sensorData.map(d => d.value),
                borderColor: color,
                backgroundColor: "rgba(255, 255, 255, 1)",
                fill: true,
                tension: 0.2
            }]
        },
        options: {
            responsive: true,
            plugins: { legend: { display: true } }
        }
    });
}

// Lấy và vẽ dữ liệu sensor
async function loadDashboard() {
    try {
        // Temperature
        const tempData = await fetchSensorData("environment_sensor", "temperature");
        renderChart("sensorChart1", "Temperature (°C)", tempData, "rgba(255,99,132,1)");

        // Humidity
        const humData = await fetchSensorData("environment_sensor", "humidity");
        renderChart("sensorChart2", "Humidity (%)", humData, "rgba(54,162,235,1)");

        // // Lux
        // const luxData = await fetchSensorData("environment_sensor", "lux");
        // renderChart("sensorChart3", "Lux", luxData, "rgba(255,206,86,1)");

        // // Soil Humidity
        // const soilData = await fetchSensorData("environment_sensor", "soil_humidity");
        // renderChart("sensorChart4", "Soil Humidity (%)", soilData, "rgba(75,192,192,1)");

        // // Distance
        // const distData = await fetchSensorData("environment_sensor", "distance");
        // renderChart("sensorChart5", "Distance (cm)", distData, "rgba(153,102,255,1)");
    } catch (err) {
        console.error("Error loading dashboard:", err);
    }
}

document.addEventListener('DOMContentLoaded', () => {
    // 1. Tải dữ liệu ngay lập tức khi trang vừa mở
    loadDashboard();

    // 2. Thiết lập một bộ đếm thời gian để gọi lại hàm loadSensor mỗi 10 giây (10000 mili giây)
    setInterval(loadDashboard, 60000);
});