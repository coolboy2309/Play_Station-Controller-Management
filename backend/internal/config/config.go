package config

import (
	"encoding/json"
	"os"
)

type Config struct {
	ServerName string `json:"server_name"`
	Version    string `json:"version"`
	Host       string `json:"host"`
	Port       int    `json:"port"`
	Database   string `json:"database"`
	LogLevel   string `json:"log_level"`
}

var AppConfig Config

func LoadConfig(path string) error {

	data, err := os.ReadFile(path)
	if err != nil {
		return err
	}

	err = json.Unmarshal(data, &AppConfig)
	if err != nil {
		return err
	}

	return nil
}
