
package repository

import (
	"database/sql"
	"errors"
	"time"

	"zoneeye/backend/internal/models"
)

type MatchSessionRepository struct {
	db *sql.DB
}

func NewMatchSessionRepository(db *sql.DB) *MatchSessionRepository {
	return &MatchSessionRepository{
		db: db,
	}
}

func (r *MatchSessionRepository) Create(session *models.MatchSession) error {

	session.CreatedAt = time.Now().UTC()

	query := `
	INSERT INTO match_sessions (
		session_id,
		competition,
		station,
		start_timer,
		end_timer,
		extra_time,
		created_at
	)
	VALUES (?, ?, ?, ?, ?, ?, ?)
	ON CONFLICT(session_id) DO NOTHING
	`

	_, err := r.db.Exec(
		query,
		session.SessionID,
		session.Competition,
		session.Station,
		session.StartTimer,
		session.EndTimer,
		session.ExtraTime,
		session.CreatedAt,
	)

	if err != nil {
		return err
	}

	// Retrieve the stored session.
	var stored models.MatchSession

	err = r.db.QueryRow(`
		SELECT
			id,
			session_id,
			competition,
			station,
			start_timer,
			end_timer,
			extra_time,
			created_at
		FROM match_sessions
		WHERE session_id = ?
	`, session.SessionID).Scan(
		&stored.ID,
		&stored.SessionID,
		&stored.Competition,
		&stored.Station,
		&stored.StartTimer,
		&stored.EndTimer,
		&stored.ExtraTime,
		&stored.CreatedAt,
	)

	if err != nil {
		return err
	}

	// Reject reuse of an ID with different session data.
	if stored.Competition != session.Competition ||
		stored.Station != session.Station ||
		stored.StartTimer != session.StartTimer ||
		stored.EndTimer != session.EndTimer ||
		stored.ExtraTime != session.ExtraTime {

		return errors.New("session ID already exists with different data")
	}

	// Return the original database record.
	session.ID = stored.ID
	session.CreatedAt = stored.CreatedAt

	return nil
}
