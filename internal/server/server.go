package server

import (
	"bytes"
	"encoding/json"
	"fmt"
	"image"
	"image/jpeg"
	"net/http"
	"strconv"
	"sync/atomic"
	"time"

	"mime/multipart"
	"net/textproto"

	"remote_display_viewer/internal/screencapture"

	"github.com/gorilla/mux"
)

type ScreenInfoJson struct {
	Id  int
	Pos screencapture.Position
}

type Configure struct {
	Interval    time.Duration
	Fps         uint8
	JpegQuality int
}

type RemoteDisplay struct {
	ScreenCapture screencapture.ScreenCapture
	Screen        []screencapture.RemoteScreen
	Config        Configure
}

func (display *RemoteDisplay) ScreensList(res http.ResponseWriter, req *http.Request) {
	if req.Method == "GET" {
		display.Screen = display.ScreenCapture.ScreenListUpdate()

		var jsonData = make([]ScreenInfoJson, len(display.Screen))

		for i, info := range display.Screen {
			jsonData[i].Id = info.Id
			jsonData[i].Pos.Width = info.Pos.Width
			jsonData[i].Pos.Height = info.Pos.Height
			jsonData[i].Pos.X = info.Pos.X
			jsonData[i].Pos.Y = info.Pos.Y
		}

		jsonBytes, err := json.Marshal(jsonData)
		if err != nil {
			Log.Error.Println(err.Error())
		}

		res.Write(jsonBytes)
	} else {
		res.Write([]byte("Invalid Method"))
	}
}

func (display *RemoteDisplay) ScreenStream(res http.ResponseWriter, req *http.Request) {
	Log.Info.Println("Connect : ", req.RemoteAddr)

	vars := mux.Vars(req)
	id, _ := strconv.Atoi(vars["id"])

	if req.Method == "GET" {
		screen := &display.Screen[id]

		atomic.AddInt32(&screen.Running, 1)

		if atomic.LoadInt32(&screen.Running) <= 1 {
			go display.updateStream(id)
		}

		buffer := make(chan []byte)
		screen.Lock.Lock()
		screen.FrameChannels[buffer] = true
		screen.Lock.Unlock()

		mimeWriter := multipart.NewWriter(res)
		contentType := fmt.Sprintf("multipart/x-mixed-replace;boundary=%s", mimeWriter.Boundary())

		res.Header().Add("Cache-Control", "no-store, no-cache, must-revalidate")
		res.Header().Add("Pragma", "no-cache")
		res.Header().Add("Content-Type", contentType)

		for {
			time.Sleep(display.Config.Interval)

			partHeader := make(textproto.MIMEHeader)
			partHeader.Add("Content-Type", "image/jpeg")

			partWriter, err := mimeWriter.CreatePart(partHeader)
			if err != nil {
				Log.Error.Println(err.Error())
				break
			}

			imageBuffer := <-buffer
			if _, err := partWriter.Write(imageBuffer); err != nil {
				Log.Error.Println(err.Error())
				break
			}
		}

		screen.Lock.Lock()
		delete(screen.FrameChannels, buffer)
		screen.Lock.Unlock()

		atomic.AddInt32(&screen.Running, -1)
	} else {
		res.Write([]byte("Invalid Method"))
	}

	Log.Info.Println("Disconnect : ", req.RemoteAddr)
}

func (display *RemoteDisplay) updateStream(id int) {
	screen := &display.Screen[id]
	image := screencapture.NewImage(image.Rect(0, 0, screen.Pos.Width, screen.Pos.Height))

	for atomic.LoadInt32(&screen.Running) > 0 {
		var buffer bytes.Buffer

		if err := display.ScreenCapture.Capture(&display.ScreenCapture, uint8(id), image); err != nil {
			Log.Error.Println(err.Error())
			break
		}

		jpeg.Encode(&buffer, image, &jpeg.Options{Quality: display.Config.JpegQuality})

		screen.Lock.Lock()
		for c := range screen.FrameChannels {
			select {
			case c <- buffer.Bytes():
			default:
			}
		}
		screen.Lock.Unlock()
	}
}
