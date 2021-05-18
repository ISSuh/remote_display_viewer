package main

// #cgo LDFLAGS: -L../../build -lrdv_screencapture
// #cgo CFLAGS: -I../../screen_capture
// #include <screen_capture_bridge.h>
import "C"
import (
	"fmt"
	"unsafe"
)

type ScreenCapture struct {
	handle unsafe.Pointer
}

type ScreemInfo struct {
	id     int
	width  int
	height int
	x      int
	y      int
}

func CreateScreenCaptureHandle() ScreenCapture {
	var screen_capture ScreenCapture
	screen_capture.handle = C.create_rdv_hadle()
	return screen_capture
}

func DestroyScreenCaptureHandle(screen_capture *ScreenCapture) {
	C.destroy_rdv_hadle(screen_capture.handle)
}

func ScreenCount(screen_capture *ScreenCapture) int {
	return int(C.screen_count(screen_capture.handle))
}

func ScreenInfomations(screen_capture *ScreenCapture) []ScreemInfo {
	count := ScreenCount(screen_capture)
	infomations := make([]ScreemInfo, count, count)
	screen_info := make([]C.RemoteScreen, count, count)

	C.screen_infomations(screen_capture.handle, &screen_info[0])

	for i, info := range screen_info {
		infomations[i].id = int(info.id)
		infomations[i].width = int(info.width)
		infomations[i].height = int(info.height)
		infomations[i].x = int(info.x)
		infomations[i].y = int(info.y)
	}

	return infomations
}

func main() {
	testHandle := CreateScreenCaptureHandle()

	fmt.Println("Screen Capture Handle : ", testHandle.handle)

	count := ScreenCount(&testHandle)
	fmt.Println("Screen Count : ", count)

	screen_info := ScreenInfomations(&testHandle)

	for _, info := range screen_info {
		fmt.Println("[", info.id, "] : ", info.width, "x", info.height, "/", info.x, ",", info.y)
	}

	DestroyScreenCaptureHandle(&testHandle)
}
