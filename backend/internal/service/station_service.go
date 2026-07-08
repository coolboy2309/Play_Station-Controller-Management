package service

import (
	"errors"
	"time"

	"zoneeye/backend/internal/models"
	"zoneeye/backend/internal/repository"
)

type StationService struct {
	repo repository.StationRepositoryInterface
}

func NewStationService(repo repository.StationRepositoryInterface) *StationService {
	return &StationService{
		repo: repo,
	}
}

func (s *StationService) RegisterStation(station *models.Station) error {

	if station.AgentID == "" {
		return errors.New("agent id is required")
	}

	if station.Name == "" {
		return errors.New("station name is required")
	}

	now := time.Now()

	station.Status = "Idle"
	station.IsOnline = false
	station.CurrentBill = 0
	station.LastSeen = now
	station.CreatedAt = now

	return s.repo.Create(station)
}
func (s *StationService) GetAllStations() ([]models.Station, error) {
	return s.repo.GetAll()
}
