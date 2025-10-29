const ws = new WebSocket(`ws://${location.host}/ws`); // Kết nối WebSocket

const sensors = [
    { id: 1, name: 'Temperature', unit: '°C', period: 10},
    { id: 2, name: 'Humidity', unit: '%', period: 10},
    { id: 3, name: 'Lux', unit: 'Lux', period: 10},
];

function initializeSensors() {
    sensors.forEach(sensor => {
        const savedName = localStorage.getItem(`sensorName${sensor.id}`);
        const savedPeriod = localStorage.getItem(`sensorPeriod${sensor.id}`);
        
        if (savedName) {
            document.getElementById(`sensorName${sensor.id}`).value = savedName;
        }
        if (savedPeriod) {
            document.getElementById(`sensorPeriod${sensor.id}`).value = savedPeriod;
        }
    });
}

function updateSensorSettings(sensorId) {
    const name = document.getElementById(`sensorName${sensorId}`).value;
    const period = parseInt(document.getElementById(`sensorPeriod${sensorId}`).value);
    
    // Validate period
    if (period < 1) {
        alert('Period must be at least 1 second');
        return;
    }

    // Save to localStorage
    localStorage.setItem(`sensorName${sensorId}`, name);
    localStorage.setItem(`sensorPeriod${sensorId}`, period);

    // Send to server
    ws.send(JSON.stringify({
        action: "settings",
        id: sensorId,
        name: name,
        period: period
    }));

    // Visual feedback
    const button = document.querySelector(`button[onclick="updateSensorSettings(${sensorId})"]`);
    button.textContent = 'Saved!';
    setTimeout(() => {
        button.textContent = 'Save Settings';
    }, 1000);
}

// WebSocket message handler
ws.onmessage = function(event) {
    try {
        const data = JSON.parse(event.data);
        if (data.type === 'sensor' && data.id) {
            document.getElementById(`sensorValue${data.id}`).textContent = data.value;
        }
    } catch (error) {
        console.error('WebSocket message error:', error);
    }
};

// Initialize everything when page loads
document.addEventListener('DOMContentLoaded', () => {
    initializeSensors();
});



