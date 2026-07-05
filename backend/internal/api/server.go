package api

import (
 "fmt"
 "net/http"

 "zoneeye/backend/internal/config"
)

func Start() error {

 http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {

  fmt.Fprintf(
   w,
   "Welcome to %s v%s\nStatus: Running",
   config.AppConfig.ServerName,
   config.AppConfig.Version,
  )

 })

 address := fmt.Sprintf(
  "%s:%d",
  config.AppConfig.Host,
  config.AppConfig.Port,
 )

 return http.ListenAndServe(address, nil)
}