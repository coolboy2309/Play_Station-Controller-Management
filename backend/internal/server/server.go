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

	stationRepo := repository.NewStationRepository(database.DB)
	stationService := service.NewStationService(stationRepo)
	stationHandler := api.NewStationHandler(stationService)
	heartbeatRepo := repository.NewHeartbeatRepository(database.DB)
	heartbeatService := service.NewHeartbeatService(heartbeatRepo)
	heartbeatHandler := api.NewHeartbeatHandler(heartbeatService)
	agentHandler := api.NewAgentHandler()

	routes.Register(
		mux,
		stationHandler,
		heartbeatHandler,
		agentHandler,
	)

	address := fmt.Sprintf(
		"%s:%d",
		config.AppConfig.Host,
		config.AppConfig.Port,
	)

	return http.ListenAndServe(address, mux)
}
