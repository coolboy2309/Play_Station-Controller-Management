package routes

import (
	"net/http"

	"zoneeye/backend/internal/api"
)

func RegisterAgentRoutes(mux *http.ServeMux, handler *api.HeartbeatHandler) {

	mux.HandleFunc("/agent/heartbeat", handler.Heartbeat)

}