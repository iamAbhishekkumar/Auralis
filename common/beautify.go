package common

import (
	"fmt"
	"runtime"
)

func ServerGreetMessage() {
	logo := []string{
		"  ___                      _  _       ",
		" / _ \\                    | |(_)     ",
		"/ /_\\ \\ _   _  _ __  __ _ | | _  ___  ",
		"|  _  || | | || '__|/ _` || || |/ __| ",
		"| | | || |_| || |  | (_| || || |\\__ \\",
		"\\_| |_/ \\__,_||_|   \\__,_||_||_||___/",
	}

	for _, line := range logo {
		fmt.Println(line)
	}
	fmt.Println("\n       A U R A L I S   D B")
}

func ServerInfo(mode string, pid int, port int) {

	arch := runtime.GOARCH
	if arch == "amd64" {
		arch = "64-bit"
	} else if arch == "386" {
		arch = "32-bit"
	}

	fmt.Printf("╭──────────────────────────────────────────────╮\n")
	fmt.Printf("│  Mode     : %-32s │\n", mode)
	fmt.Printf("│  PID      : %-32d │\n", pid)
	fmt.Printf("│  Port     : %-32d │\n", port)
	fmt.Printf("│  Arch     : %-32s │\n", arch)
	fmt.Printf("╰──────────────────────────────────────────────╯\n")
	fmt.Println("\033[32m[OK]\033[0m Ready to accept connections")
}

func InfoLog(text ...any) {
	fmt.Print("\033[32m[INFO]\033[0m ")
	fmt.Println(text...)
}

func ErrorLog(text ...any) {
	fmt.Print("\033[31m[ERROR]\033[0m ")
	fmt.Println(text...)
}

func DebugLog(text ...any) {
	fmt.Print("\033[33m[DEBUG]\033[0m ")
	fmt.Println(text...)
}
