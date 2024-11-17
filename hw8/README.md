# Chapter 40

2.

The first available block is preferred for allocation

3.

Empty files or links are usually ended up to exist. The `makedir` and `write` operations would fail.

4.

`unlink` can succeed while all other operations will usually fail.

The final state usually ends up to be with an empty directory.

# 26

1.

```shell
   dx          Thread 0
    0
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt
```

2.

3, there's no race condition so threads won't affect calculation

3.

No

4.

```shell
 2000          Thread 0
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1006 halt
```

5.

Becasue bx is set to 3. Final value is 6.

6.

It depends on if increased ax is saved to value before it's reset

7.

For 1 or 2 as interval the value is 1, when interval equals to or is greater than 3, it's always 2.

8.

When interval equals to or is greater than 597.

When it's times of 3, value is always 200.

9.

Thread 0 sets memory to 1 then halts, while thread 1 keeps running until memory is set to 1.

10.

The threads will switch positions, now thread 0 will keep spinning until thread 1 executes. The interval determines how long thread 0 spins around.

The program is not efficient since CPU gets wasted by thread 0.
