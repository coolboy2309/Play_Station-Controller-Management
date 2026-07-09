package routes

import (
	"fmt"
	"net/http"
	"zoneeye/backend/internal/api"
	"zoneeye/backend/internal/config"
)

func Register(
	mux *http.ServeMux,
	stationHandler *api.StationHandler,
	heartbeatHandler *api.HeartbeatHandler,
    agentHandler *api.AgentHandler,
) {

	mux.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(
			w,
			"Welcome to %s v%s\nStatus: Running",
			config.AppConfig.ServerName,
			config.AppConfig.Version,
		)
	})

	mux.HandleFunc("/health", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintln(w, "OK")
	})

	mux.HandleFunc("/version", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "%s", config.AppConfig.Version)
	})

	RegisterStationRoutes(mux, stationHandler)
    RegisterAgentRoutes(mux, heartbeatHandler)
    RegisterAgentRegisterRoutes(mux, agentHandler)
}
