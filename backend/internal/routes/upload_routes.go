package routes

import (
	"net/http"

	"zoneeye/backend/internal/api"
)

func RegisterUploadRoutes(
	mux *http.ServeMux,
	handler *api.UploadHandler,
) {
	mux.HandleFunc("/agent/upload", handler.UploadScreenshot)
}