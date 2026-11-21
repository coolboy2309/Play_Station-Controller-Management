
package api

import (
	"encoding/json"
	"errors"
	"net/http"
	"io"

	"zoneeye/backend/internal/models"
	"zoneeye/backend/internal/service"
)

type MatchSessionHandler struct {
	service *service.MatchSessionService
}

func NewMatchSessionHandler(service *service.MatchSessionService) *MatchSessionHandler {
	return &MatchSessionHandler{
		service: service,
	}
}

func (h *MatchSessionHandler) SaveSession(w http.ResponseWriter, r *http.Request) {

	if r.Method != http.MethodPost {
		http.Error(w, "Method Not Allowed", http.StatusMethodNotAllowed)
		return
	}

	r.Body = http.MaxBytesReader(w, r.Body, 1<<20)
	defer r.Body.Close()

	var session models.MatchSession

	decoder := json.NewDecoder(r.Body)
	decoder.DisallowUnknownFields()

	if err := decoder.Decode(&session); err != nil {
		http.Error(w, "Invalid JSON: "+err.Error(), http.StatusBadRequest)
		return
	}

	if err := decoder.Decode(&struct{}{}); err == nil {
		http.Error(w, "Invalid JSON: multiple JSON values", http.StatusBadRequest)
		return
	} else if !errors.Is(err, io.EOF) {
		http.Error(w, "Invalid JSON: trailing data", http.StatusBadRequest)
		return
	}

	if err := h.service.SaveSession(&session); err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusCreated)

	json.NewEncoder(w).Encode(map[string]interface{}{
		"message": "Match session saved successfully",
		"session": session,
	})
}
