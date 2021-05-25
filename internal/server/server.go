package server

import (
	"bytes"
	"encoding/json"
	"fmt"
	"image"
	"image/jpeg"
	"log"
	"net/http"
	"strconv"
	"time"

	"remote_display_viewer/internal/screencapture"

	"github.com/gorilla/mux"
)

type ScreenInfoJson struct {
	Id  int
	Pos screencapture.Position
}

type RemoteDisplay struct {
	ScreenCapture screencapture.ScreenCapture
	Screen        []screencapture.RemoteScreen
	Interval      time.Duration
	Fps           uint8
}

const headerParts = "\r\n" +
	"Content-Type: multipart/x-mixed-replace;boundary=MJPEGBOUNDARY\r\n" +
	"Content-Type: image/jpeg\r\n" +
	"Content-Length: %d\r\n" +
	"X-Timestamp: 0.000000\r\n" +
	"\r\n"

func (display *RemoteDisplay) ScreensList(res http.ResponseWriter, req *http.Request) {
	if req.Method == "GET" {
		display.Screen = screencapture.ScreenInfomations(&display.ScreenCapture)

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
			log.Fatal(err)
		}

		res.Write(jsonBytes)
	} else {
		res.Write([]byte("Invalid Method"))
	}
}

func (display *RemoteDisplay) ScreenStream(res http.ResponseWriter, req *http.Request) {
	vars := mux.Vars(req)
	id, _ := strconv.Atoi(vars["id"])

	if req.Method == "GET" {
		if !display.Screen[id].Running {
			go display.updateStream(id)
		}

		buffer := make(chan []byte)
		display.Screen[id].Lock.Lock()
		display.Screen[id].FrameChannels[buffer] = true
		display.Screen[id].Lock.Unlock()

		res.Header().Add("Cache-Control", "no-store, no-cache, must-revalidate")
		res.Header().Add("Pragma", "no-cache")
		res.Header().Add("Content-Type", "multipart/x-mixed-replace;boundary=MJPEGBOUNDARY")

		for {
			time.Sleep(display.Interval)

			imageBuffer := <-buffer
			fmt.Println("Receive : ", len(imageBuffer))

			if _, err := res.Write(imageBuffer); nil != err {
				fmt.Println(err)
				break
			}
		}

		display.Screen[id].Lock.Lock()
		delete(display.Screen[id].FrameChannels, buffer)
		display.Screen[id].Lock.Unlock()
	} else {
		res.Write([]byte("Invalid Method"))
	}
}

func (display *RemoteDisplay) updateStream(id int) {
	display.Screen[id].Running = true

	image := screencapture.NewImage(image.Rect(
		0, 0, display.Screen[id].Pos.Width, display.Screen[id].Pos.Height))

	for {
		var buffer bytes.Buffer

		screencapture.Capture(&display.ScreenCapture, uint8(id), image)
		jpeg.Encode(&buffer, image, &jpeg.Options{Quality: 50})

		bufferSize := len(buffer.Bytes())

		header := fmt.Sprintf(headerParts, bufferSize)
		headerSize := len(header)

		frame := make([]byte, (bufferSize + headerSize))
		copy(frame, header)
		copy(frame[bufferSize:], buffer.Bytes())

		display.Screen[id].Lock.Lock()
		for c := range display.Screen[id].FrameChannels {
			select {
			case c <- frame:
			default:
			}
		}
		display.Screen[id].Lock.Unlock()
	}
}
