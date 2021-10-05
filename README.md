## />mpl

[![Liberapay](https://liberapay.com/assets/widgets/donate.svg)](https://liberapay.com/andreas7/donate)

### setup
/>mpl requires CMake and a C++ compiler to build, rlwrap is highly recommended for running the REPL.

```
$ cd ampl
$ mkdir build
$ cd build
$ cmake ..
$ make
$ rlwrap ./ampl
/>mpl v2

Press Return on empty line to evaluate.

May the source be with you!

  42

[42]
```

### the stack
Values are pushed on the stack in specified order.

```
  1 2 3
  
[1 2 3]
```

```

`cp` may be used to copy the top value.

```
  1 2 3 cp

[1 2 3 3]
```

`d` may be used to drop values.

```
  1 2 3 4 5 ddd

[1 2]

  d

[1]
```
