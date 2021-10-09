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
/>mpl v5

Press Return on empty line to evaluate.

May the Source be with you!

  func fib (n Int) (Int) 
    if < n 2 n + fib - n 1 fib - n 2

  fib 10
  
[55]
```

### stack tools
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

### branches
`if` may be used to branch on a condition.<br/>
All values boolean representations; most are true; but 0, empty strings etc. are false.

```
  if 42 'yes 'no

['yes]
```

### functions
New functions may be defined using `func`.

```
  func foo () (Int) 42

[]
  foo

[42]
```

### debugging tools
Values may be dumped to `cout` using `dump`.

```
  dump 42

42
[]
```