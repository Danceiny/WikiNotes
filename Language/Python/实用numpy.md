
1. numpy.tile
https://docs.scipy.org/doc/numpy/reference/generated/numpy.tile.html
>Construct an array by repeating A the number of times given by reps.

>If reps has length d, the result will have dimension of max(d, A.ndim).

>If A.ndim < d, A is promoted to be d-dimensional by prepending new axes. So a shape (3,) array is promoted to (1, 3) for 2-D replication, or shape (1, 1, 3) for 3-D replication. If this is not the desired behavior, promote A to d-dimensions manually before calling this function.

>If A.ndim > d, reps is promoted to A.ndim by pre-pending 1’s to it. Thus for an A of shape (2, 3, 4, 5), a reps of (2, 2) is treated as (1, 1, 2, 2).

>Note : Although tile may be used for broadcasting, it is strongly recommended to use numpy’s broadcasting operations and functions.

2. 利用numpy计算曼哈顿距离
案例：http://www.programcreek.com/python/example/75174/scipy.spatial.distance.cityblock
源码：https://github.com/scipy/scipy/blob/v0.14.0/scipy/spatial/distance.py#L477
