
package server

import (
	"fmt"
	"net/http"

	"zoneeye/backend/internal/api"
	"zoneeye/backend/internal/config"
	"zoneeye/backend/internal/database"
	"zoneeye/backend/internal/repository"
	"zoneeye/backend/internal/routes"
	"zoneeye/backend/internal/service"
)

func Start() error {

	mux := http.NewServeMux()

	// Station
	stationRepo := repository.NewStationRepository(database.DB)
	stationService := service.NewStationService(stationRepo)
	stationHandler := api.NewStationHandler(stationService)

	// Heartbeat
	heartbeatRepo := repository.NewHeartbeatRepository(database.DB)
	heartbeatService := service.NewHeartbeatService(heartbeatRepo)
	heartbeatHandler := api.NewHeartbeatHandler(heartbeatService)

	// Agent registration
	agentHandler := api.NewAgentHandler()

	// Screenshot upload
	uploadHandler := api.NewUploadHandler()

	// Match sessions
	matchSessionRepo := repository.NewMatchSessionRepository(database.DB)
	matchSessionService := service.NewMatchSessionService(matchSessionRepo)
	matchSessionHandler := api.NewMatchSessionHandler(matchSessionService)

	routes.Register(
		mux,
		stationHandler,
		heartbeatHandler,
		agentHandler,
		uploadHandler,
		matchSessionHandler,
	)

	address := fmt.Sprintf(
		"%s:%d",
		config.AppConfig.Host,
		config.AppConfig.Port,
	)

	return http.ListenAndServe(address, mux)
}
