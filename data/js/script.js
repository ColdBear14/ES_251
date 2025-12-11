const ws = new WebSocket(`ws://${location.host}/ws`); // Kết nối WebSocket

const sensors = [
    { id: 1, name: 'Temperature', unit: '°C', period: 10},
    { id: 2, name: 'Humidity', unit: '%', period: 10},
    { id: 3, name: 'Lux', unit: 'Lux', period: 10},
];

ws.onmessage = (event) => {
    try {
        const data = JSON.parse(event.data);
        console.log("Received WS Data:", data);

        if (Array.isArray(data)) {
            data.forEach(item => processIncomingData(item));
        } else if (typeof data === 'object') {
            processIncomingData(data);
        }

    } catch (e) {
        console.error("JSON Parse Error:", e);
    }
};

function processIncomingData(item) {
    // TRƯỜNG HỢP 1: Dữ liệu Cảm biến (Có 'sensorId' và 'value')
    if (item.sensorId !== undefined && item.value !== undefined) {
        const element = document.getElementById(`sensorValue${item.sensorId}`);
        if (element) {
            element.textContent = item.value;
            
            // Hiệu ứng nhấp nháy để biết có dữ liệu mới
            element.style.transition = "color 0.2s";
            element.style.color = "blue";
            setTimeout(() => element.style.color = "black", 500);
        }
    }
    
    // TRƯỜNG HỢP 2: Dữ liệu Cài đặt (Có 'id', 'name', 'period')
    else if (item.id !== undefined && item.period !== undefined) {
        // Cập nhật ô input trên giao diện
        const nameInput = document.getElementById(`sensorName${item.id}`);
        const periodInput = document.getElementById(`sensorPeriod${item.id}`);
        
        if (nameInput) nameInput.value = item.name;
        if (periodInput) periodInput.value = item.period;

        const localSensor = sensors.find(s => s.id === item.id);
        if (localSensor) {
            localSensor.name = item.name;
            localSensor.period = item.period;
        }
    }
}


function updateSensorSettings(sensorId) {
    const name = document.getElementById(`sensorName${sensorId}`).value;
    const period = parseInt(document.getElementById(`sensorPeriod${sensorId}`).value);
    
    // Validate period
    if (period < 1) {
        alert('Period must be at least 1 second');
        return;
    }

    ws.send(JSON.stringify({
        action: "settings", // Vẫn giữ action khi gửi đi để ESP32 biết client muốn làm gì
        id: sensorId,
        name: name,
        period: period
    }));

    // Visual feedback (Hiệu ứng nút bấm)
    const button = document.querySelector(`button[onclick="updateSensorSettings(${sensorId})"]`);
    if(button) {
        const originalText = button.textContent;
        button.textContent = 'Sent!';
        setTimeout(() => {
            button.textContent = originalText;
        }, 1000);
    }
}


document.addEventListener('DOMContentLoaded', () => {
    console.log("Ready. Waiting for WebSocket data...");
});