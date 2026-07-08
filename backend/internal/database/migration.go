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
	`

	_, err := DB.Exec(query)
	if err != nil {
		return err
	}

	return nil
}