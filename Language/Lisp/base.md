# 1
```lisp
> (format t "~A plus ~A equals ~A.~%" 2 3 (+ 2 3))
2 plus 3 equals 5.
NIL
```			
`~A` indicates a position to be filled.
`~%` indicates a new line.


# 2 
```lisp
(
	defun askem (string)
		(format t "~A" string)
		(read)
)
```
> (askem "How old are you?")
How old are you? 2000
2000
```