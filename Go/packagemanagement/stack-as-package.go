package stack

// the Stack struct to save the items 
type Stack struct {
	i	int
	data	[10]int
}

// push the items into the stack
func (s *Stack) Push(k int){
	s.data[s.i] = k
	s.i++
}

// pop a item from the stack
func (s *Stack) Pop()(ret int){
	s.i--
	ret = s.data[s.i]
	return
}
