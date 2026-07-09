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
	uploadHandler := api.NewUploadHandler()

	routes.Register(
		mux,
		stationHandler,
		heartbeatHandler,
		agentHandler,
		uploadHandler,
	)

	address := fmt.Sprintf(
		"%s:%d",
		config.AppConfig.Host,
		config.AppConfig.Port,
	)

	return http.ListenAndServe(address, mux)
}
$filePath = "C:\Users\ABENEZER\Downloads\files (2)\test.jpg"
Invoke-WebRequest `
-Uri http://localhost:8080/agent/upload `
-Method POST `
-Form @{
    screenshot = Get-Item $filePath
}