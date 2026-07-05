package models

import "time"

type Station struct{
	Id 			int
	AgentID 	string
	Name 		string
	Status 		string
	CurrentBill	float64
	LastSeen 	time.Time
	CreatedAt 	time.Time
}