package main

import (
	"log"
	"net/http"

	"remote_display_viewer/internal/screencapture"
	"remote_display_viewer/internal/server"

	"github.com/gorilla/mux"
)

func main() {
	var remoteDisplay server.RemoteDisplay
	remoteDisplay.ScreenCapture = screencapture.CreateScreenCaptureHandle()

	router := mux.NewRouter()
	router.HandleFunc("/api/screens", remoteDisplay.ScreensList)
	router.HandleFunc("/stream/{id}", remoteDisplay.ScreenStream)
	router.PathPrefix("/").Handler(http.FileServer(http.Dir("./static")))

	if err := http.ListenAndServe(":5000", router); err != nil {
		log.Fatal(err)
	}

	screencapture.DestroyScreenCaptureHandle(&remoteDisplay.ScreenCapture)
}
