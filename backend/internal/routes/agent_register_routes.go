package routes

import (
	"net/http"

	"zoneeye/backend/internal/api"
)

func RegisterAgentRegisterRoutes(
	mux *http.ServeMux,
	handler *api.AgentHandler,
) {

	mux.HandleFunc("/agent/register", handler.Register)

}