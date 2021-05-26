package server

import (
	"io/ioutil"
	"log"
	"os"
)

type Logger struct {
	Trace *log.Logger
	Debug *log.Logger
	Warn  *log.Logger
	Info  *log.Logger
	Error *log.Logger
}

var Log *Logger

func LoggerInitialize() {
	Log = &Logger{}

	Log.Trace = log.New(ioutil.Discard, "[TRACE] ", log.Ldate|log.Ltime|log.Lshortfile)
	Log.Debug = log.New(os.Stdout, "[TRACE] ", log.Ldate|log.Ltime|log.Lshortfile)
	Log.Info = log.New(os.Stdout, "[INFO] ", log.Ldate|log.Ltime|log.Lshortfile)
	Log.Warn = log.New(os.Stdout, "[WARNING] ", log.Ldate|log.Ltime|log.Lshortfile)
	Log.Error = log.New(os.Stderr, "[ERROR] ", log.Ldate|log.Ltime|log.Lshortfile)
}
