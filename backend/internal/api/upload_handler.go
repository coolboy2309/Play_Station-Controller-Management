package api

import (
	"io"
	"net/http"
	"os"
)

type UploadHandler struct{}

func NewUploadHandler() *UploadHandler {
	return &UploadHandler{}
}

func (h *UploadHandler) UploadScreenshot(w http.ResponseWriter, r *http.Request) {

	if r.Method != http.MethodPost {
		http.Error(w, "Method Not Allowed", http.StatusMethodNotAllowed)
		return
	}

	file, _, err := r.FormFile("screenshot")
	if err != nil {
		http.Error(w, "Screenshot Required", http.StatusBadRequest)
		return
	}
	defer file.Close()

	dst, err := os.Create("backend/uploads/screenshots/latest.jpg")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	defer dst.Close()

	io.Copy(dst, file)

	w.Write([]byte("Screenshot Uploaded"))
}