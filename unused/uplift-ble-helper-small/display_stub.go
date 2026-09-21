//go:build !darwin

package main

import (
	"fmt"
)

func supportsDisplayCheck() error {
	return fmt.Errorf("unknown arch - display detection is not supported")
}

func checkDisplay() (bool, error) {
	return false, fmt.Errorf("unknown arch - checkDisplay is not supported")
}
