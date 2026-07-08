package database

import (
	"database/sql"

	_ "modernc.org/sqlite"
)

var DB *sql.DB

func Connect(path string) error {

	db, err := sql.Open("sqlite", path)
	if err != nil {
		return err
	}

	if err = db.Ping(); err != nil {
		return err
	}

	DB = db

	if err := Migrate(); err != nil {
		return err
	}

	return nil
}
