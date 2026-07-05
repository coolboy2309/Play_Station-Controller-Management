package routes

import (
	"fmt"
	"net/http"
	"zoneeye/backend/internal/config"
)

func Register(mux *http.ServeMux){
	//home
	mux.HandleFunc("/", func (w http.ResponseWriter, r *http.Request)  {
		fmt.Fprintf(
			w, "Welcome To %s v%s\nStatus: Running",
			config.AppConfig.ServerName,
			config.AppConfig.Version,
		)		
	})
	// health check
	mux.HandleFunc("/health", func (w http.ResponseWriter, r *http.Request)  {
		fmt.Fprintln(w, "OK")
	})
	//Version
	mux.HandleFunc("/version", func (w http.ResponseWriter,r *http.Request)  {
		fmt.Fprintf(w, "%s", config.AppConfig.Version)
	})
}