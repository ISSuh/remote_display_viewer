package screencapture

// #cgo CXXFLAGS: -I. -g -O3 -Wall -Wextra -fPIC -std=c++11
// #cgo pkg-config: gtk+-3.0
// #include <screen_capture_bridge.h>
import "C"
import "unsafe"

type Screen struct {
	Id     int
	Width  int
	Height int
	X      int
	Y      int
}

type ScreenCapture struct {
	Handle unsafe.Pointer
	Screen C.Screen
}

func CreateScreenCaptureHandle() ScreenCapture {
	var screenCapture ScreenCapture
	screenCapture.Handle = C.create_rdv_hadle()
	return screenCapture
}

func DestroyScreenCaptureHandle(screenCapture *ScreenCapture) {
	C.destroy_rdv_hadle(screenCapture.Handle)
}

func ScreenCount(screenCapture *ScreenCapture) int {
	return int(C.screen_count(screenCapture.Handle))
}

func ScreenInfomations(screenCapture *ScreenCapture) []Screen {
	count := ScreenCount(screenCapture)

	infomations := make([]Screen, count, count)
	screen := make([]C.Screen, count, count)

	C.screen_infomations(screenCapture.Handle, &screen[0])

	for i, info := range screen {
		infomations[i].Id = int(info.id)
		infomations[i].Width = int(info.width)
		infomations[i].Height = int(info.height)
		infomations[i].X = int(info.x)
		infomations[i].Y = int(info.y)
	}

	return infomations
}

func Capture(screenCapture *ScreenCapture, screenId uint8, image *Image) {
	C.capture(screenCapture.Handle, C.int(screenId), unsafe.Pointer(&image.Pix[0]))
}
