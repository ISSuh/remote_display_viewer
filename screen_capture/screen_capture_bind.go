package screencapture

// #cgo LDFLAGS:
// #cgo CXXFLAGS: -I. -g -O3 -Wall -Wextra -fPIC -std=c++11
// #cgo pkg-config: x11 xext xinerama gtk+-2.0
// #include <screen_capture_bridge.h>
import "C"
import (
	"unsafe"
)

type ScreenInfo struct {
	Id     int
	Width  int
	Height int
	X      int
	Y      int
}

type ScreenImage struct {
	buffer []byte
	width  int
	height int
	plane  int
}

type ScreenCapture struct {
	handle     unsafe.Pointer
	screenInfo ScreenInfo
	cpature    ScreenImage
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

func ScreenInfomations(screenCapture *ScreenCapture) []ScreenInfo {
	count := ScreenCount(screenCapture)
	infomations := make([]ScreenInfo, count, count)
	screenInfo := make([]C.ScreenInfo, count, count)

	C.screen_infomations(screenCapture.handle, &screenInfo[0])

	for i, info := range screenInfo {
		infomations[i].Id = int(info.id)
		infomations[i].Width = int(info.width)
		infomations[i].Height = int(info.height)
		infomations[i].X = int(info.x)
		infomations[i].Y = int(info.y)
	}

	return infomations
}

func CreateScreenImage(screenCapture *ScreenCapture) {
}

func DestroyScreenImage() {
}

func Capture() {
}
