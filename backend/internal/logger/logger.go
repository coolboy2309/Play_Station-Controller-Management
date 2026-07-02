package logger

import (
	"log"
)

func Info(message string) {
	log.Println("[INFO] " + message)
}

func Warning(message string) {
	log.Println("[WARNING] " + message)
}

func Error(message string) {
	log.Println("[ERROR] " + message)
}