
package database

func Migrate() error {

	query := `
	CREATE TABLE IF NOT EXISTS stations (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		agent_id TEXT NOT NULL UNIQUE,
		name TEXT NOT NULL,
		status TEXT NOT NULL,
		is_online BOOLEAN NOT NULL DEFAULT 0,
		current_bill REAL DEFAULT 0,
		last_seen DATETIME,
		created_at DATETIME
	);

	CREATE TABLE IF NOT EXISTS match_sessions (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		session_id TEXT NOT NULL UNIQUE,
		competition TEXT NOT NULL,
		station TEXT NOT NULL,
		start_timer TEXT NOT NULL,
		end_timer TEXT NOT NULL,
		extra_time BOOLEAN NOT NULL DEFAULT 0,
		created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
	);
	`

	_, err := DB.Exec(query)
	if err != nil {
		return err
	}

	// Check whether session_id already exists.
	rows, err := DB.Query("PRAGMA table_info(match_sessions)")
	if err != nil {
		return err
	}

	hasSessionID := false

	for rows.Next() {
		var cid int
		var name, dataType string
		var notNull, primaryKey int
		var defaultValue interface{}

		if err := rows.Scan(
			&cid,
			&name,
			&dataType,
			&notNull,
			&defaultValue,
			&primaryKey,
		); err != nil {
			rows.Close()
			return err
		}

		if name == "session_id" {
			hasSessionID = true
		}
	}

	if err := rows.Err(); err != nil {
		rows.Close()
		return err
	}

	rows.Close()

	// Upgrade an existing database without deleting its records.
	if !hasSessionID {
		_, err = DB.Exec(
			"ALTER TABLE match_sessions ADD COLUMN session_id TEXT",
		)
		if err != nil {
			return err
		}
	}

	// Ensure every non-null session ID is unique.
	_, err = DB.Exec(`
		CREATE UNIQUE INDEX IF NOT EXISTS
		idx_match_sessions_session_id
		ON match_sessions(session_id)
	`)
	if err != nil {
		return err
	}

	return nil
}
