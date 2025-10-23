const ws = new WebSocket(`ws://${location.host}/ws`); // Kết nối WebSocket

async function fetchSensorData(collection, name = "") {
    let url = `http://192.168.4.2:3000/getData?collection=${collection}`;
    if (name) url += `&name=${name}`;
    const response = await fetch(url);
    if (!response.ok) throw new Error("Network response was not ok");
    return await response.json();
}

async function loadSensor() {
    try {
        // Temperature
        const tempData = await fetchSensorData("environment_sensor", "temperature");
        document.getElementById('sensorValue1').textContent = tempData[0].value; // Hiển thị nhiệt độ

        // Humidity
        const humData = await fetchSensorData("environment_sensor", "humidity");
        document.getElementById('sensorValue2').textContent = humData[0].value; // Hiển thị độ ẩm

        // // Lux
        // const luxData = await fetchSensorData("environment_sensor", "lux");

        // // Soil Humidity
        // const soilData = await fetchSensorData("environment_sensor", "soil_humidity");

        // // Distance
        // const distData = await fetchSensorData("environment_sensor", "distance");
    } catch (err) {
        console.error("Error loading sensor:", err);
    }
}

document.addEventListener('DOMContentLoaded', () => {
    loadSensor();
    setInterval(loadSensor, 20000);
});


// ws.onmessage = (event) => {
//     const data = JSON.parse(event.data); // Parse dữ liệu JSON từ ESP32
//     document.getElementById('sensorValue1').textContent = data.temperature; // Hiển thị nhiệt độ
//     document.getElementById('sensorValue2').textContent = data.humidity;    // Hiển thị độ ẩm
//     document.getElementById('sensorValue3').textContent = data.lux;    // Hiển thị độ ánh sáng
//     document.getElementById('sensorValue4').textContent = data.soil;    // Hiển thị độ ẩm đất
//     document.getElementById('sensorValue5').textContent = data.distance;    // Hiển thị khoảng cách
// };

const states = [false, false, false];
const toggleButtons = [
    { id: 'toggleButton1', device: 1 },
    { id: 'toggleButton2', device: 2 },
    { id: 'toggleButton3', device: 3 },
    { id: 'toggleButton4', device: 4 }

];

function updateState(index) {
    const stateSpan = document.getElementById(`state${index+1}`);
    if (states[index]) {
        stateSpan.textContent = "ON";
        stateSpan.classList.add("state-on");
        stateSpan.classList.remove("state-off");
    } else {
        stateSpan.textContent = "OFF";
        stateSpan.classList.add("state-off");
        stateSpan.classList.remove("state-on");
    }
}

toggleButtons.forEach((button, idx) => {
    const element = document.getElementById(button.id);
    element.addEventListener('click', () => {
        states[idx] = !states[idx];
        updateState(idx);
        document.getElementById(`state${idx+1}`).textContent = states[idx] ? "ON" : "OFF";
        ws.send(JSON.stringify({
            action: "device", 
            device: button.device,
            state: states[idx] ? "on" : "off" }));
    });
    // Khởi tạo trạng thái ban đầu
    document.getElementById(`state${idx+1}`).textContent = "OFF";
});

// Hiển thị giá trị cảm biến (giả lập)
const sensorValues = [
    { id: 'sensorValue1', name: 'Temperature' },
    { id: 'sensorValue2', name: 'Humidity' },
    { id: 'sensorValue3', name: 'Lux' },
    { id: 'sensorValue4', name: 'Soil Humidity' },
    { id: 'sensorValue5', name: 'Distance' }
];

sensorValues.forEach(sensor => {
    const element = document.getElementById(sensor.id);
    element.textContent = `Value of ${sensor.name}`; // Hiển thị giá trị cảm biến
});

