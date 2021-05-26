package screencapture

// #cgo CXXFLAGS: -I. -g -O3 -Wall -Wextra -fPIC -std=c++11
// #cgo pkg-config: gtk+-3.0
// #include <screen_capture_bridge.h>
import "C"
import (
	"errors"
	"sync"
	"sync/atomic"
	"unsafe"
)

type Position struct {
	Width  int
	Height int
	X      int
	Y      int
}

type RemoteScreen struct {
	Id            int
	FrameChannels map[chan []byte]bool
	Pos           Position
	Lock          sync.Mutex
	Running       int32
}

type ScreenCapture struct {
	Handle unsafe.Pointer
}

func (capture *ScreenCapture) ScreenCount() int {
	return int(C.screen_count(capture.Handle))
}

func (capture *ScreenCapture) ScreenListUpdate() []RemoteScreen {
	count := capture.ScreenCount()

	infomations := make([]RemoteScreen, count, count)
	screen := make([]C.Screen, count, count)

	C.screen_infomations(capture.Handle, &screen[0])

	for i, info := range screen {
		infomations[i].Id = int(info.id)
		infomations[i].Pos.Width = int(info.width)
		infomations[i].Pos.Height = int(info.height)
		infomations[i].Pos.X = int(info.x)
		infomations[i].Pos.Y = int(info.y)

		atomic.StoreInt32(&infomations[i].Running, 0)
		infomations[i].FrameChannels = make(map[chan []byte]bool)
	}

	return infomations
}

func (capture *ScreenCapture) Capture(screenCapture *ScreenCapture, screenId uint8, image *Image) error {
	C.capture(capture.Handle, C.int(screenId), unsafe.Pointer(&image.Pix[0]))
	if image.Pix == nil {
		return errors.New("Invalid pixel memory")
	}
	return nil
}

func CreateScreenCaptureHandle() ScreenCapture {
	var screenCapture ScreenCapture
	screenCapture.Handle = C.create_rdv_hadle()
	return screenCapture
}

func DestroyScreenCaptureHandle(screenCapture *ScreenCapture) {
	C.destroy_rdv_hadle(screenCapture.Handle)
}
