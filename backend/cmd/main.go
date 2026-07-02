package main

import "zoneeye/backend/internal/logger"

func main() {
	logger.Info("ZoneEye Server Starting...")
	logger.Info("Logger Initialized")
	logger.Warning("This is a warning example")
	logger.Error("This is an error example")
}
