package server

import (
	"bytes"
	"encoding/json"
	"image"
	"image/jpeg"
	"log"
	"net/http"
	"strconv"
	"sync"
	"time"

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

type RemoteScreen struct {
	Id            int
	FrameChannels map[chan bytes.Buffer]bool
	Pos           Position
	Lock          sync.Mutex
	Interval      time.Duration
	Fps           uint8
}

type RemoteDisplay struct {
	ScreenCapture screencapture.ScreenCapture
	RemoteScreen  []RemoteScreen
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
		if len(screen.RemoteScreen) <= id {
			screen.RemoteScreen = append(screen.RemoteScreen, screen.createScreen(id))

			// go screen.worker()
		}

		res.Header().Set("Cache-Control", "no-cache")
		res.Header().Set("Connection", "keep-alive")
		res.Header().Add("Content-Type", "multipart/x-mixed-replace;boundary=--MJPEGBOUNDARY")
		res.Header().Add("Content-Type", "image/jpeg")

		// res.Header().Add("Content-Length", strconv.Itoa(len(buf.Bytes())))
		// res.Write(buf.Bytes())
	} else {
		res.Write([]byte("Invalid Method"))
	}
}

func (screen *RemoteDisplay) worker(id int) {
	image := screencapture.NewImage(image.Rect(
		0, 0, screen.RemoteScreen[id].Pos.Width, screen.RemoteScreen[id].Pos.Height))

	buf := make(chan bytes.Buffer)
	screen.RemoteScreen[id].Lock.Lock()
	screen.RemoteScreen[id].FrameChannels[buf] = true
	screen.RemoteScreen[id].Lock.Unlock()

	screencapture.Capture(&screen.ScreenCapture, uint8(id), image)

	jpeg.Encode(buf, image, &jpeg.Options{Quality: 50})
}

func (screen *RemoteDisplay) createScreen(id int) RemoteScreen {
	var remoteScreen RemoteScreen
	infomations := screencapture.ScreenInfomations(&screen.ScreenCapture)

	remoteScreen.Id = id
	remoteScreen.Pos.Width = infomations[id].Width
	remoteScreen.Pos.Height = infomations[id].Height
	remoteScreen.Pos.X = infomations[id].X
	remoteScreen.Pos.Y = infomations[id].Y

	return remoteScreen
}
