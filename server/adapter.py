import os
import base64
import requests
import logging
from settings import GamePhase

FPORT = 1
CONFIRMED_DOWNLINK = False
PRIORITY = "NORMAL"
SCHEDULE = "push"

PHASE_CODES = {
    GamePhase.WAIT_INITIALIZATION: 1,
    GamePhase.PHASE_PUZZLE: 2,
    GamePhase.SERIAL_PUZZLE: 3,
    GamePhase.BACKEND_PUZZLE: 4,
    GamePhase.COMPLETED: 5
}

def send_lorawan_downlink(next_phase):
    code = PHASE_CODES.get(next_phase)
    if code is None:
        logging.error(f"Invalid next phase: {next_phase}")
        return

    payload_b64 = base64.b64encode(bytes([code])).decode('utf-8')

    application_name = os.environ.get('applicationName', 'your-application-name')
    end_device_name = os.environ.get('endDeviceName', 'your-end-device-name')
    api_key = os.environ.get('APIKey', 'your-api-key')
    api_key = api_key.strip().replace('"', '').replace("'", "")

    url = f"https://eu1.cloud.thethings.network/api/v3/as/applications/{application_name}/devices/{end_device_name}/down/{SCHEDULE}"
    
    headers = {
        'Authorization': f'Bearer {api_key}', 
        'Content-Type': 'application/json',
        'User-Agent': 'my-python-integration/1.0'
    }
    
    payload_data = {
        "downlinks": [{
            "f_port": FPORT,
            "frm_payload": payload_b64,
            "priority": PRIORITY,
            "confirmed": CONFIRMED_DOWNLINK
        }]
    }

    try:
        response = requests.post(url, headers=headers, json=payload_data)
        response.raise_for_status() 
        print("Downlink successful! Status:", response.status_code)
    except requests.exceptions.RequestException as e:
        print(f"Error sending downlink: {e}")
        if e.response is not None:
            print("TTN error details:", e.response.text)