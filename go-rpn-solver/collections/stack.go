package collections

type Stack[T any] struct {
	elements []T
}

func NewStack[T any](initialCapacity int) Stack[T] {
	return Stack[T]{elements: make([]T, 0, initialCapacity)}
}

func (s *Stack[T]) Push(item T) {
	s.elements = append(s.elements, item)
}

func (s *Stack[T]) PopDangerous() T {
	n := len(s.elements) - 1
	ret := s.elements[n]
	s.elements = s.elements[:n]
	return ret
}

func (s *Stack[T]) Pop() (T, bool) {
	var zero T
	if len(s.elements) == 0 {
		return zero, false
	}
	n := len(s.elements) - 1
	ret := s.elements[n]
	s.elements = s.elements[:n]
	return ret, true
}

func (s *Stack[T]) IsEmpty() bool {
	return len(s.elements) == 0
}

func (s *Stack[T]) PeekDangerous() T {
	return s.elements[len(s.elements)-1]
}

func (s *Stack[T]) Peek() (T, bool) {
	var zero T
	if len(s.elements) == 0 {
		return zero, false
	}
	return s.elements[len(s.elements)-1], true
}
