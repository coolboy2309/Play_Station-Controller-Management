package repository

import (
	"database/sql"
	"errors"
)

type HeartbeatRepository struct {
	db *sql.DB
}

func NewHeartbeatRepository(db *sql.DB) *HeartbeatRepository {
	return &HeartbeatRepository{
		db: db,
	}
}

func (r *HeartbeatRepository) UpdateHeartbeat(agentID string) error {

	query := `
	UPDATE stations
	SET
		is_online = ?,
		last_seen = CURRENT_TIMESTAMP
	WHERE agent_id = ?;
	`

	result, err := r.db.Exec(query, true, agentID)
	if err != nil {
		return err
	}

	rowsAffected, err := result.RowsAffected()
	if err != nil {
		return err
	}

	if rowsAffected == 0 {
		return errors.New("station not found")
	}

	return nil
}