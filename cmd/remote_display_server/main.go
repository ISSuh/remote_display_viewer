package main

import (
	"encoding/json"
	"log"
	"net/http"
	screencapture "remote_display_viewer/screen_capture"

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

func screenHandler(res http.ResponseWriter, req *http.Request) {
	if req.Method == "GET" {
		screenCaptureHadle := screencapture.CreateScreenCaptureHandle()
		infomations := screencapture.ScreenInfomations(&screenCaptureHadle)

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
		screencapture.DestroyScreenCaptureHandle(&screenCaptureHadle)
	} else {
		res.Write([]byte("Invalid Method"))
	}
}

func screenView(res http.ResponseWriter, req *http.Request) {
	if req.Method == "GET" {
		vars := mux.Vars(req)
		Id := vars["id"]

		res.Write([]byte(Id))
	} else {
		res.Write([]byte("Invalid Method"))
	}
}

func main() {
	router := mux.NewRouter()
	router.HandleFunc("/api/screens", screenHandler)
	router.HandleFunc("/api/screens/{id}", screenView)
	router.PathPrefix("/").Handler(http.FileServer(http.Dir("./static")))

	if err := http.ListenAndServe(":5000", router); err != nil {
		log.Fatal(err)
	}
}
