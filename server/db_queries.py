## @file db_queries.py
# @brief SQLite persistence and game-phase queries.
import sqlite3
from contextlib import closing

from settings import GamePhase, DB_PATH, KEYED_PHASES


## @brief Execute a parameterized SQLite statement and commit on successful completion.
# @param query SQL statement to execute.
# @param args Positional SQL parameter values.
# @param one Return only the first row when true.
# @return A list of rows, or the first row/None when one is true.
# @note SQLite errors propagate to the caller.
def _query_db(query, args=(), one=False):
    with closing(sqlite3.connect(DB_PATH)) as conn, conn:
        rows = conn.execute(query, args).fetchall()
    if one:
        return rows[0] if rows else None
    return rows


## @brief Create player and measurement tables and seed measurements when the table is empty.
# @return None.
def init_db():
    _query_db(f'''
        CREATE TABLE IF NOT EXISTS PLAYER (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL,
            activation_code TEXT NOT NULL UNIQUE,
            game_phase TEXT NOT NULL DEFAULT '{GamePhase.WAIT_INITIALIZATION}'
        )
    ''')
    
    _query_db('''
        CREATE TABLE IF NOT EXISTS MEASUREMENT (
            key TEXT UNIQUE NOT NULL,
            value FLOAT NOT NULL
        )
    ''')
    
    if not _query_db("SELECT * FROM MEASUREMENT"):
        _query_db('''
            INSERT INTO MEASUREMENT (key, value) 
            VALUES ('temperature', 24.5), ('humidity', 42.0), ('pressure', 1012.3)
        ''')

## @brief Create the game-phase table and upsert the ordered phases and unlock keys.
# @return None.
def init_phases():
    _query_db('''
        CREATE TABLE IF NOT EXISTS GAME_PHASE (
            name TEXT PRIMARY KEY,
            position INTEGER NOT NULL UNIQUE,
            unlock_key TEXT
        )
    ''')
    for position, phase in enumerate(GamePhase):
        key = f'{phase.value.lower()}_key' if phase in KEYED_PHASES else None
        _query_db(
            '''
            INSERT INTO GAME_PHASE (name, position, unlock_key) VALUES (?, ?, ?)
            ON CONFLICT(name) DO UPDATE SET position = excluded.position,
                                            unlock_key = excluded.unlock_key
            ''',
            (phase.value, position, key),
        )


## @brief Find the unlock key for a phase.
# @param phase Game phase name or GamePhase value.
# @return The key string, or None for a missing or unkeyed phase.
def get_phase_key(phase):
    row = _query_db("SELECT unlock_key FROM GAME_PHASE WHERE name = ?", (phase,), one=True)
    return row[0] if row else None


## @brief Find the phase immediately following the given phase.
# @param phase Game phase name or GamePhase value.
# @return The next phase name, or None if no successor exists.
def get_next_phase(phase):
    row = _query_db(
        '''
        SELECT name FROM GAME_PHASE
        WHERE position = (SELECT position + 1 FROM GAME_PHASE WHERE name = ?)
        ''',
        (phase,), one=True,
    )
    return row[0] if row else None


## @brief Read all players.
# @return A list of (username, activation_code, game_phase) rows.
def get_players():
    return _query_db("SELECT username, activation_code, game_phase FROM PLAYER")


## @brief Insert a player in the default initialization phase.
# @param username Player display name.
# @param activation_code Unique activation code.
# @return None.
# @note A duplicate activation code raises sqlite3.IntegrityError.
def add_player(username, activation_code):
    _query_db(
        "INSERT INTO PLAYER (username, activation_code) VALUES (?, ?)",
        (username, activation_code),
    )


## @brief Look up a player by activation code.
# @param activation_code Unique activation code.
# @return An (id, username, game_phase) row, or None.
def get_player_by_code(activation_code):
    return _query_db(
        "SELECT id, username, game_phase FROM PLAYER WHERE activation_code = ?",
        (activation_code,), one=True,
    )


## @brief Look up a player by numeric identifier.
# @param player_id Numeric player identifier.
# @return An (id, username, game_phase) row, or None.
def get_player_by_id(player_id):
    return _query_db(
        "SELECT id, username, game_phase FROM PLAYER WHERE id = ?",
        (player_id,), one=True,
    )


## @brief Update the stored game phase for a player.
# @param player_id Numeric player identifier.
# @param phase Game phase name or GamePhase value.
# @return None.
def set_game_phase(player_id, phase):
    _query_db("UPDATE PLAYER SET game_phase = ? WHERE id = ?", (phase, player_id))


# --- Injection ---
## @brief Read the available measurement names.
# @return A list of measurement key strings.
def get_measurement_keys():
    rows = _query_db("SELECT key FROM MEASUREMENT")
    return [row[0] for row in rows]

## @brief Execute the backend puzzle measurement lookup using the supplied SQL fragment.
# @param user_input Raw measurement lookup input used by the puzzle.
# @return The first matching value, None for no match, or a SQLite error string.
# @note Intentionally interpolates untrusted input for the SQL-injection puzzle; opens SQLite in read-only mode.
def get_vulnerable_measurement(user_input):
    query = f'SELECT value FROM MEASUREMENT WHERE key = "{user_input}"'
    
    try:
        with closing(sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)) as conn, conn:
            row = conn.execute(query).fetchone()
            
        return row[0] if row else None
    except sqlite3.Error as e:
        return str(e)