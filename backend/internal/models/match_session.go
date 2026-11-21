
package models

import "time"

type MatchSession struct {
	ID          int64     `json:"id"`
	SessionID   string    `json:"session_id"`
	Competition string    `json:"competition"`
	Station     string    `json:"station"`
	StartTimer  string    `json:"start_timer"`
	EndTimer    string    `json:"end_timer"`
	ExtraTime   bool      `json:"extra_time"`
	CreatedAt   time.Time `json:"created_at"`
}
