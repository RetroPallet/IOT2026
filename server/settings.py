## @file settings.py
# @brief Game phases and server configuration.
import enum
import os

## @brief Ordered server-side phases stored as string enum values.
class GamePhase(enum.StrEnum):
    ## @brief Wait initialization.
    WAIT_INITIALIZATION = 'WAIT_INITIALIZATION'
    ## @brief Phase puzzle.
    PHASE_PUZZLE = 'PHASE_PUZZLE'
    ## @brief Serial puzzle.
    SERIAL_PUZZLE = 'SERIAL_PUZZLE'
    ## @brief Backend puzzle.
    BACKEND_PUZZLE = 'BACKEND_PUZZLE'
    ## @brief Completed.
    COMPLETED = 'COMPLETED'
## @brief Phases requiring a submitted unlock key.
KEYED_PHASES = (GamePhase.PHASE_PUZZLE, GamePhase.SERIAL_PUZZLE, GamePhase.BACKEND_PUZZLE)

## @brief Path to the SQLite database.
DB_PATH = 'database.db'
## @brief Disable device downlinks when enabled through the debug environment variable.
DEBUG = os.environ.get('debug', 'True').lower() in ('true', '1', 't')