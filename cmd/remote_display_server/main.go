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

func ScreenInfomations(screen_capture *ScreenCapture, screen_info ) {

}

func main() {
	testHandle := CreateScreenCaptureHandle()

	fmt.Println("Screen Capture Handle : ", testHandle.handle)

	fmt.Println("Screen Count : ", ScreenCount(&testHandle))

	var remote_screen_info []C.RemoteScreen

	DestroyScreenCaptureHandle(&testHandle)
}
