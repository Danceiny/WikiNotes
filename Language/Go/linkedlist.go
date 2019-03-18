/* doubly linked list using container/list */

/*	*/
package main

import (
	"fmt"
	"container/list"
)

func main(){
	l := list.New()
	l.PushBack(1)
	l.PushBack(2)
	l.PushBack(4)

	for e := l.Front(); e!=nil; e = e.Next(){
		fmt.Printf("%v\n",e.Value)
	}
}




/* doubly linked list implemented by self */

/*	*/

package main

import (
	"errors"
	"fmt"
)

type Value int

type Node struct {
	Value
	prev, next *Node
}

type List struct {
	head, tail *Node
}

func (l *List) Front() *Node {
	return l.head
}

func (n *Node) Next() *Node {
	return n.next
}

func (l *List) Push(v Value) *List {
	n := &Node{Value: v}	//when pushing, create a new Node with the provided value
	
	if l.head == nil {
		l.head = n
	}
	else {
		l.tail.next = n
		n.prev = l.tail
	}
	l.tail = n
	return l
}

var errEmpty = errors.New("List is empty")

func (l *List) Pop()(v Value, err error){
	if l.tail == nil {
		err = errEmpty
	}
	else {
		v = l.tail.Value
		l.tail = t.tail.prev
		if l.tail == nil {
			l.head = nil
		}
	}
	return v, err
}

func main() {
	l :=  new(List)
	l.Push(1)
	l.Push(2)
	l.Push(4)

	for n := l.Front(); n!=nil; n=n.Next(){
		fmt.Printf("%v\n",n.Value)
	}
	fmt.Println()

	for v, err := l.Pop(); err==nil; v,err = l.Pop(){
		fmt.Printf("%v\n",v)
	}
}



