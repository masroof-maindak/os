### Usage

```bash
gcc multi.c -o multi && time ./multi
gcc single.c -o single && time ./single
```

### Observations

Unsurprisingly, the single-core performance, for finding primes in the first 10,000,000 numbers was poor compared to the multi-core performance; averaging 6.4s vs 2.9s respectively. Setting THREAD\_COUNT to a value above '4' proved to be counter-intuitive; this is probably because my machine only has 4 cores.

### References

- https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-pthread-exit-exit-thread#ptexit
- https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-pthread-create-create-thread
- https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-pthread-join-wait-thread-end#ptjoin
- https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
