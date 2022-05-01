I have done merge sort in 3 ways 
1. General merge sort
2. Merge sort using processes
3. Merge sort using Threads

In merge sort, we have three stages sorting left, right, and merging them here. If the number of elements is less than or equal to 5, then sorting is done using selection sort.
In merge sort using processes, the left array is sorted by a process, and the right array is sorted by another process 

On running for an array of 10000 randomly filled elements:
    Time taken for merge sort is 0.002801
    Time taken for concurrent merge sort is 0.246735
    Time taken for merge sort using threads is 0.082223

    Here we can observe that standard merge sort is 29 times faster than merge sort using threads and 88 times faster than concurrent merge sort
because in the concurrent and threads initialization should be done so it consumes some time in concurrent it is relatively higher than threads so concurrent will be much slower than threads


    In order of times :
                        Merge sort > threads >> concurrent

    

