const ws = new WebSocket(`ws://${location.host}/ws`);
const charts = {}; // Lưu trữ biểu đồ

const sensors = [
    { id: 1, name: 'Temperature', color: "rgba(255,99,132,1)" },
    { id: 2, name: 'Humidity', color: "rgba(54,162,235,1)" },
    { id: 3, name: 'Lux', color: "rgba(255,206,86,1)" },
];

// --- 1. KHỞI TẠO BIỂU ĐỒ TRỐNG ---
function initEmptyCharts() {
    sensors.forEach(sensor => {
        const ctx = document.getElementById(`sensorChart${sensor.id}`);
        if (ctx) {
            charts[sensor.id] = new Chart(ctx.getContext('2d'), {
                type: 'line',
                data: {
                    labels: [],
                    datasets: [{
                        label: sensor.name,
                        data: [],
                        borderColor: sensor.color,
                        backgroundColor: sensor.color.replace("1)", "0.2)"),
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
    });
}

ws.onopen = () => {
    console.log("WebSocket Connected. Requesting History...");
    
    sensors.forEach(s => {
        ws.send(JSON.stringify({
            action: "get_history",
            sensorId: s.id
        }));
    });
};

ws.onmessage = (event) => {
    try {
        const data = JSON.parse(event.data);

        if (Array.isArray(data)) {
            
            if (data.length > 1) {
                const historyData = data.reverse(); 
                if (historyData.length > 0 && historyData[0].sensorId) {
                    updateChartHistory(historyData[0].sensorId, historyData);
                }
            } else if (data.length === 1) {
                updateChartRealtime(data[0]);
            }
        } else if (typeof data === 'object') {
            updateChartRealtime(data);
        }

    } catch (e) {
        console.error("WS Error:", e);
    }
};

function updateChartHistory(sensorId, dataArray) {
    const chart = charts[sensorId];
    if (chart) {
        chart.data.labels = dataArray.map(d => new Date(d.timestamp).toLocaleTimeString());
        chart.data.datasets[0].data = dataArray.map(d => d.value);
        chart.update();
        console.log(`Updated History for Sensor ${sensorId}`);
    }
}

function updateChartRealtime(item) {
    if (item.sensorId && item.value) {
        const chart = charts[item.sensorId];
        if (chart) {
            const timeNow = new Date().toLocaleTimeString();
            chart.data.labels.push(timeNow);
            chart.data.datasets[0].data.push(item.value);

            // Giữ lại 20 điểm thôi cho nhẹ
            if (chart.data.labels.length > 20) {
                chart.data.labels.shift();
                chart.data.datasets[0].data.shift();
            }
            chart.update();
        }
    }
}

document.addEventListener('DOMContentLoaded', () => {
    initEmptyCharts(); // Vẽ khung biểu đồ trước
});