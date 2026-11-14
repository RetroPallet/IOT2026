## @file app.py
# @brief Flask pages and game API endpoints.
import hmac

from flask import Flask, render_template, request, redirect, url_for, jsonify

from db_queries import (
    init_db, init_phases, get_players, add_player,
    get_player_by_code, get_player_by_id, set_game_phase,
    get_phase_key, get_next_phase, get_measurement_keys, get_vulnerable_measurement,
)
from settings import GamePhase, DEBUG
from adapter import send_lorawan_downlink

## @brief Flask application instance.
app = Flask(__name__)

init_db()
init_phases()


## @brief Render the home page for GET / and /home.
# @return Rendered index.html.
@app.route('/')
@app.route('/home')
def index():
    return render_template('index.html')


## @brief Display registered players or create a player from submitted form fields.
# @return Rendered admin.html for GET, or a redirect after POST.
@app.route('/admin', methods=['GET', 'POST'])
def admin():
    if request.method == 'POST':
        add_player(request.form['name'], request.form['activation_code'])
        return redirect(url_for('admin'))
    return render_template('admin.html', data=get_players())


## @brief Render the player interface.
# @return Rendered player.html.
@app.route('/player')
def player():
    return render_template('player.html')


## @brief Render the measurement puzzle for a player in BACKEND_PUZZLE.
# @return Rendered injection.html, or a home-page redirect when access is unavailable.
# @note Reads the player identifier from the player_id cookie.
@app.route('/injection')
def injection():
    player_id = request.cookies.get('player_id', type=int) 
    row = get_player_by_id(player_id) if player_id is not None else None

    if row is None or row[2] != GamePhase.BACKEND_PUZZLE:
        return redirect(url_for('index'))
    
    keys = get_measurement_keys()
    return render_template('injection.html', measurements=keys)


# --- API ---
## @brief Activate a player using the JSON activation_code field.
# @return Player JSON with status 200, or error JSON with status 404.
# @note Advances WAIT_INITIALIZATION to PHASE_PUZZLE and sends a downlink when DEBUG is false.
@app.route('/api/player/activate', methods=['POST'])
def api_activate():
    code = (request.get_json(silent=True) or {}).get('activation_code', '').strip()
    row = get_player_by_code(code)
    if row is None:
        return jsonify(error='Invalid activation code'), 404

    player_id, username, phase = row
    if phase == GamePhase.WAIT_INITIALIZATION:
        set_game_phase(player_id, GamePhase.PHASE_PUZZLE)
        phase = GamePhase.PHASE_PUZZLE
        if not DEBUG:
            send_lorawan_downlink(GamePhase.PHASE_PUZZLE)
            print("sending downlink")
    return jsonify(id=player_id, username=username, phase=phase)


## @brief Return the stored player identity and phase.
# @param player_id Numeric player identifier.
# @return Player JSON with status 200, or error JSON with status 404.
@app.route('/api/player/<int:player_id>')
def api_player(player_id):
    row = get_player_by_id(player_id)
    if row is None:
        return jsonify(error='Player not found'), 404
    return jsonify(id=row[0], username=row[1], phase=row[2])


## @brief Reset a player to PHASE_PUZZLE and optionally queue a device downlink.
# @param player_id Numeric player identifier.
# @return Player phase JSON with status 200, or error JSON with status 404.
@app.route('/api/player/<int:player_id>/reset', methods=['POST'])
def api_reset(player_id):
    if get_player_by_id(player_id) is None:
        return jsonify(error='Player not found'), 404
    set_game_phase(player_id, GamePhase.PHASE_PUZZLE)
    if not DEBUG:
        send_lorawan_downlink(GamePhase.PHASE_PUZZLE)
        print("sending downlink")
        
    return jsonify(id=player_id, phase=GamePhase.PHASE_PUZZLE)


## @brief Validate the submitted phase and unlock key before advancing a player.
# @param player_id Numeric player identifier.
# @return Updated phase JSON (200), or error JSON for a missing player (404), locked phase (409) or wrong key (403).
# @note Compares keys with hmac.compare_digest and sends a downlink when DEBUG is false.
@app.route('/api/player/<int:player_id>/advance', methods=['POST'])
def api_advance(player_id):
    row = get_player_by_id(player_id)
    if row is None:
        return jsonify(error='Player not found'), 404

    current = row[2]
    data = request.get_json(silent=True) or {}
    phase = data.get('phase')
    key = str(data.get('key', '')).strip()

    expected = get_phase_key(current)
    if expected is None or phase != current:
        return jsonify(error='This phase is locked'), 409
    if not hmac.compare_digest(key.encode(), expected.encode()):
        return jsonify(error='Wrong key'), 403

    next_phase = get_next_phase(current)
    set_game_phase(player_id, next_phase)
    if (not DEBUG):
        send_lorawan_downlink(next_phase)
        print("sending downlink")
    return jsonify(id=player_id, phase=next_phase)

## @brief Run a submitted sensor_value lookup for the backend puzzle.
# @return Value JSON (200), or error JSON for missing player (401), wrong phase (403), query error (400) or no result (404).
@app.route('/api/injection', methods=['POST'])
def api_injection():
    player_id = request.cookies.get('player_id', type=int) 
    row = get_player_by_id(player_id) if player_id is not None else None

    if row is None:
        return jsonify(error="Unauthorized: Missing or invalid player ID"), 401

    if row[2] != GamePhase.BACKEND_PUZZLE:
        return jsonify(error="Forbidden: Player is not in the correct game phase"), 403

    data = request.get_json(silent=True) or {}
    user_input = data.get('sensor_value', '')

    result = get_vulnerable_measurement(user_input)
    
    if isinstance(result, str) and ("error" in result.lower() or "no such" in result.lower()):
        return jsonify(error=result), 400
    
    if result is not None:
        return jsonify(value=result)
    
    return jsonify(error="No data for the specified key"), 404
if __name__ == '__main__':
    app.run(debug=False)