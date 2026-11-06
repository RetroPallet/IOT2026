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
    GamePhase.WAIT_INITIALIZATION: 0,
    GamePhase.ARMED: 1,
    GamePhase.SERIAL_PUZZLE: 2,
    GamePhase.BACKEND_PUZZLE: 3,
    GamePhase.COMPLETED: 4
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

    url = 'https://eu1.cloud.thethings.network/api/v3/as/applications/' + application_name + '/devices/' + end_device_name + '/down/'+ SCHEDULE
    header = {'Authorization': 'Bearer ' + config.APIKey, 
        'Content-Type': 'application/json',
        'User-Agent': 'my-integration/my-integration-version'
        }