# SBRK malloc demo

The world's simplest malloc implementation.

Let's take a look at it's growing memory usage. To do this, open a shell and
run (fun fact - you can split screen shells with tmux):

```
watch -n 1 "free -h"
```

In another shell let's run `make run_test` which will run `./test` with our
`sbrk malloc`.

See how the memory usage grows by 1M every second. We're freeing memory (1M
every 3s), but that's not reflected since our heap usage is constantly
increasing.

Now, kill the tester, and let's run `./test`. Now, we see a slightly different
pattern. Every 2s, we see the memory usage stay the same, since the free'd
blocks can be reused. The memory usage doesn't go down, because we're not
giving the allocator a chance to actually shrink the heap (calling sbrk with a
negative value).
