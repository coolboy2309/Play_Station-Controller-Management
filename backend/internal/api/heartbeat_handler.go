package api

import (
	"encoding/json"
	"net/http"

	"zoneeye/backend/internal/models"
	"zoneeye/backend/internal/service"
)

type HeartbeatHandler struct {
	service *service.HeartbeatService
}

func NewHeartbeatHandler(service *service.HeartbeatService) *HeartbeatHandler {
	return &HeartbeatHandler{
		service: service,
	}
}

func (h *HeartbeatHandler) Heartbeat(w http.ResponseWriter, r *http.Request) {

	if r.Method != http.MethodPost {
		http.Error(w, "Method Not Allowed", http.StatusMethodNotAllowed)
		return
	}

	var heartbeat models.Heartbeat

	err := json.NewDecoder(r.Body).Decode(&heartbeat)
	if err != nil {
		http.Error(w, "Invalid JSON", http.StatusBadRequest)
		return
	}

	err = h.service.Heartbeat(heartbeat.AgentID)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")

	json.NewEncoder(w).Encode(map[string]string{
		"message": "Heartbeat Received",
	})
}