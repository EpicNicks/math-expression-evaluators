package sliceutils

import "fmt"

type GenericSlice[T any] []T

func RemoveAt[T any](ts *[]T, index int) error {
	if index < 0 || index >= len(*ts) {
		return fmt.Errorf("index out of range: %d", index)
	}
	*ts = append((*ts)[:index], (*ts)[index+1:]...)
	return nil
}

func ReplaceAt[T any](ts *[]T, index int, element T) error {
	if index < 0 || index >= len(*ts) {
		return fmt.Errorf("index out of range: %d", index)
	}
	(*ts)[index] = element
	return nil
}
