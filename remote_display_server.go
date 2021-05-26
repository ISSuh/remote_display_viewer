package main

import (
	"net/http"
	"time"

	"remote_display_viewer/internal/screencapture"
	"remote_display_viewer/internal/server"

	"github.com/gorilla/mux"
)

func main() {
	server.LoggerInitialize()

	var remoteDisplay server.RemoteDisplay
	remoteDisplay.ScreenCapture = screencapture.CreateScreenCaptureHandle()

	remoteDisplay.Config.JpegQuality = 70
	remoteDisplay.Config.Interval = 16 * time.Millisecond

	router := mux.NewRouter()
	router.HandleFunc("/api/screens", remoteDisplay.ScreensList)
	router.HandleFunc("/stream/{id}", remoteDisplay.ScreenStream)
	router.PathPrefix("/").Handler(http.FileServer(http.Dir("./static")))

	if err := http.ListenAndServe(":5000", router); err != nil {
		server.Log.Error.Println(err)
	}

	screencapture.DestroyScreenCaptureHandle(&remoteDisplay.ScreenCapture)
}
