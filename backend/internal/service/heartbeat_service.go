package service

import "zoneeye/backend/internal/repository"

type HeartbeatService struct {
	repo *repository.HeartbeatRepository
}

func NewHeartbeatService(repo *repository.HeartbeatRepository) *HeartbeatService {
	return &HeartbeatService{
		repo: repo,
	}
}

func (s *HeartbeatService) Heartbeat(agentID string) error {
	return s.repo.UpdateHeartbeat(agentID)
}