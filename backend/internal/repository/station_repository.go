package repository

import (
	"database/sql"

	"zoneeye/backend/internal/models"
)

type StationRepository struct {
	db *sql.DB
}

func NewStationRepository(db *sql.DB) *StationRepository {
	return &StationRepository{
		db: db,
	}
}

func (r *StationRepository) Create(station *models.Station) error {

	query := `
	INSERT INTO stations
	(
		agent_id,
		name,
		status,
		is_online,
		current_bill,
		last_seen,
		created_at
	)
	VALUES (?, ?, ?, ?, ?, ?, ?)
	`

	_, err := r.db.Exec(
		query,
		station.AgentID,
		station.Name,
		station.Status,
		station.IsOnline,
		station.CurrentBill,
		station.LastSeen,
		station.CreatedAt,
	)

	if err != nil {
		return err
	}

	return nil
}

func (r *StationRepository) GetAll() ([]models.Station, error) {

	query := `
	SELECT
		id,
		agent_id,
		name,
		status,
		current_bill,
		is_online,
		last_seen,
		created_at
	FROM stations
	ORDER BY id;
	`

	rows, err := r.db.Query(query)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	var stations []models.Station

	for rows.Next() {

		var station models.Station

		err := rows.Scan(
			&station.ID,
			&station.AgentID,
			&station.Name,
			&station.Status,
			&station.CurrentBill,
			&station.IsOnline,
			&station.LastSeen,
			&station.CreatedAt,
		)

		if err != nil {
			return nil, err
		}

		stations = append(stations, station)
	}

	return stations, nil
}
