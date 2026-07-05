package api

import (
	"fmt"
	"net/http"

	"zoneeye/backend/internal/config"
	"zoneeye/backend/internal/routes"
)

func Start() error {

	mux := http.NewServeMux()

	routes.Register(mux)

	address := fmt.Sprintf(
		"%s:%d",
		config.AppConfig.Host,
		config.AppConfig.Port,
	)

	return http.ListenAndServe(address, mux)
}