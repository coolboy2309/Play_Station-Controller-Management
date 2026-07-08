package routes

import (
	"net/http"

	"zoneeye/backend/internal/api"
)

func RegisterStationRoutes(mux *http.ServeMux, handler *api.StationHandler) {

	mux.HandleFunc("/admin/stations", func(w http.ResponseWriter, r *http.Request) {

		switch r.Method {

		case http.MethodPost:
			handler.RegisterStation(w, r)

		case http.MethodGet:
			handler.GetStations(w, r)

		default:
			http.Error(w, "Method Not Allowed", http.StatusMethodNotAllowed)
		}

	})

}
