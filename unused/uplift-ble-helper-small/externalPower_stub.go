//go:build !darwin

package main

import (
	"fmt"
)

func supportsExternalPowerCheck() error {
	return fmt.Errorf("unknown arch - external power detection is not supported")

}

func checkIsConnectedToExternalPower() (bool, error) {
	return false, fmt.Errorf("unknown arch - external power detection is not supported")
}
