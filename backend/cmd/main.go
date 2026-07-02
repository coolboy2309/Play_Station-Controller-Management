package main

import (
	"fmt"

	"zoneeye/backend/internal/config"
	"zoneeye/backend/internal/database"
	"zoneeye/backend/internal/logger"
)

func main() {

	logger.Info("Loading configuration...")

	err := config.LoadConfig("backend/internal/config/config.json")
	if err != nil {
		logger.Error(err.Error())
		return
	}

	logger.Info("Configuration Loaded")

	logger.Info("Connecting database...")

	err = database.Connect(config.AppConfig.Database)
	if err != nil {
		logger.Error(err.Error())
		return
	}

	logger.Info("Database Connected Successfully")

	fmt.Println("================================")
	fmt.Println("Server :", config.AppConfig.ServerName)
	fmt.Println("Version:", config.AppConfig.Version)
	fmt.Println("Host   :", config.AppConfig.Host)
	fmt.Println("Port   :", config.AppConfig.Port)
	fmt.Println("DB     :", config.AppConfig.Database)
	fmt.Println("================================")
}