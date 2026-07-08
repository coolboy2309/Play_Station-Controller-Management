package repository

import "zoneeye/backend/internal/models"

type StationRepositoryInterface interface {
	Create(station *models.Station) error
	GetAll() ([]models.Station, error)
}