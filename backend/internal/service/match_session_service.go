
package service

import (
	"errors"
	"strings"

	"zoneeye/backend/internal/models"
	"zoneeye/backend/internal/repository"
)

type MatchSessionService struct {
	repo *repository.MatchSessionRepository
}

func NewMatchSessionService(repo *repository.MatchSessionRepository) *MatchSessionService {
	return &MatchSessionService{
		repo: repo,
	}
}

func (s *MatchSessionService) SaveSession(session *models.MatchSession) error {

	if strings.TrimSpace(session.SessionID) == "" {
		return errors.New("session id is required")
	}

	if strings.TrimSpace(session.Competition) == "" {
		return errors.New("competition is required")
	}

	if strings.TrimSpace(session.Station) == "" {
		return errors.New("station is required")
	}

	if strings.TrimSpace(session.StartTimer) == "" {
		return errors.New("start timer is required")
	}

	if strings.TrimSpace(session.EndTimer) == "" {
		return errors.New("end timer is required")
	}

	return s.repo.Create(session)
}
