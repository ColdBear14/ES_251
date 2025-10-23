const ws = new WebSocket(`ws://${location.host}/ws`); // Kết nối WebSocket

document.getElementById('wifi-config-form').addEventListener('submit', async function(e) {
    e.preventDefault();
    const ssid = document.getElementById('ssid').value;
    const password = document.getElementById('password').value;

    if (!ssid.trim() || !password.trim()) {
        alert("Vui lòng điền đầy đủ thông tin WiFi!");
        return;
    }

    ws.send(JSON.stringify({
        action: "wifi",
        ssid: ssid,
        password: password
    }));
});

document.getElementById('mqtt-config-form').addEventListener('submit', async function(e) {
    e.preventDefault();
    const username = document.getElementById('username').value;
    const key = document.getElementById('key').value;

    if (!username.trim() || !key.trim()) {
        alert("Vui lòng điền đầy đủ thông tin MQTT!");
        return;
    }

    ws.send(JSON.stringify({
        action: "mqtt",
        username: username,
        key: key
    }));
});

function checkInputs(formId, buttonId) {
    const form = document.getElementById(formId);
    const connectBtn = document.getElementById(buttonId);
    const inputs = form.querySelectorAll('input[required]');

    let allFilled = true;
    inputs.forEach(input => {
        if (!input.value.trim()) allFilled = false;
    });
    connectBtn.disabled = !allFilled;
}

document.querySelectorAll('input[required]').forEach(input => {
    input.addEventListener('input', function() {
        checkInputs(input.closest('form').id, input.closest('form').querySelector('button').id);
    });
});

window.addEventListener('DOMContentLoaded', function() {
    checkInputs('wifi-config-form', 'wifi-connect-btn');
    checkInputs('mqtt-config-form', 'mqtt-connect-btn');
});