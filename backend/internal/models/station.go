package models

import "time"

type Station struct {
	ID          int       `json:"id"`
	AgentID     string    `json:"agent_id"`
	Name        string    `json:"name"`
	Status      string    `json:"status"`
	CurrentBill float64   `json:"current_bill"`
	IsOnline    bool      `json:"is_online"`
	LastSeen    time.Time `json:"last_seen"`
	CreatedAt   time.Time `json:"created_at"`
}
