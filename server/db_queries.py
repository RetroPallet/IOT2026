import sqlite3
from contextlib import closing

from settings import GamePhase, DB_PATH, KEYED_PHASES


def _query_db(query, args=(), one=False):
    with closing(sqlite3.connect(DB_PATH)) as conn, conn:
        rows = conn.execute(query, args).fetchall()
    if one:
        return rows[0] if rows else None
    return rows


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


def get_phase_key(phase):
    row = _query_db("SELECT unlock_key FROM GAME_PHASE WHERE name = ?", (phase,), one=True)
    return row[0] if row else None


def get_next_phase(phase):
    row = _query_db(
        '''
        SELECT name FROM GAME_PHASE
        WHERE position = (SELECT position + 1 FROM GAME_PHASE WHERE name = ?)
        ''',
        (phase,), one=True,
    )
    return row[0] if row else None


def get_players():
    return _query_db("SELECT username, activation_code, game_phase FROM PLAYER")


def add_player(username, activation_code):
    _query_db(
        "INSERT INTO PLAYER (username, activation_code) VALUES (?, ?)",
        (username, activation_code),
    )


def get_player_by_code(activation_code):
    return _query_db(
        "SELECT id, username, game_phase FROM PLAYER WHERE activation_code = ?",
        (activation_code,), one=True,
    )


def get_player_by_id(player_id):
    return _query_db(
        "SELECT id, username, game_phase FROM PLAYER WHERE id = ?",
        (player_id,), one=True,
    )


def set_game_phase(player_id, phase):
    _query_db("UPDATE PLAYER SET game_phase = ? WHERE id = ?", (phase, player_id))


# --- Injection ---
def get_measurement_keys():
    rows = _query_db("SELECT key FROM MEASUREMENT")
    return [row[0] for row in rows]

def get_vulnerable_measurement(user_input):
    query = f'SELECT value FROM MEASUREMENT WHERE key = "{user_input}"'
    
    try:
        with closing(sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)) as conn, conn:
            row = conn.execute(query).fetchone()
            
        return row[0] if row else None
    except sqlite3.Error as e:
        return str(e)