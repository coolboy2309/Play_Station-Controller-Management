package models

import "time"

type Heartbeat struct {
	AgentID   string    `json:"agent_id"`
	Timestamp time.Time `json:"timestamp"`
}
