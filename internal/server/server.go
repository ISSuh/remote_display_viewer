package server

import (
	"encoding/json"
	"log"
	"net/http"
	"strconv"

	"remote_display_viewer/internal/screencapture"

	"github.com/gorilla/mux"
)

type Position struct {
	Width  int
	Height int
	X      int
	Y      int
}

type ScreenInfoJson struct {
	Id  int
	Pos Position
}

type RemoteDisplay struct {
	ScreenCapture screencapture.ScreenCapture
}

func (screen *RemoteDisplay) ScreensList(res http.ResponseWriter, req *http.Request) {
	if req.Method == "GET" {
		infomations := screencapture.ScreenInfomations(&screen.ScreenCapture)

		var jsonData = make([]ScreenInfoJson, len(infomations))

		for i, info := range infomations {
			jsonData[i].Id = info.Id
			jsonData[i].Pos.Width = info.Width
			jsonData[i].Pos.Height = info.Height
			jsonData[i].Pos.X = info.X
			jsonData[i].Pos.Y = info.Y
		}

		jsonBytes, err := json.Marshal(jsonData)
		if err != nil {
			log.Fatal(err)
		}

		res.Write(jsonBytes)
	} else {
		res.Write([]byte("Invalid Method"))
	}
}

func (screen *RemoteDisplay) ScreenStream(res http.ResponseWriter, req *http.Request) {
	vars := mux.Vars(req)
	id, _ := strconv.Atoi(vars["id"])

	if req.Method == "GET" {
		res.Header().Set("Cache-Control", "no-cache")
		res.Header().Set("Connection", "keep-alive")
		res.Header().Add("Content-Type", "multipart/x-mixed-replace;boundary=--MJPEGBOUNDARY")
		res.Header().Add("Content-Type", "image/jpeg")
		// res.Header().Add("Content-Length", "0")

		screencapture.CreateScreenImage(&screen.ScreenCapture, uint8(id))

		screencapture.Capture(&screen.ScreenCapture, uint8(id))

		// res.Write([]byte(Id))

		screencapture.DestroyScreenImage(&screen.ScreenCapture, uint8(id))
	} else {
		res.Write([]byte("Invalid Method"))
	}
}
