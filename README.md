## />mpl

[![Liberapay](https://liberapay.com/assets/widgets/donate.svg)](https://liberapay.com/andreas7/donate)

### setup
/>mpl requires CMake and a C++ compiler to build, `rlwrap` is highly recommended for running the REPL.

```
$ cd ampl
$ mkdir build
$ cd build
$ cmake ..
$ make
$ rlwrap ./ampl
/>mpl v6

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
All values have boolean representations; most are true; but `0`, empty strings etc. are false.

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

Functions are lexically scoped,

```
  func foo () () (
    func bar () (Int) 42
    bar
  )

[]
  foo

[]
  bar

Error in 'repl' at line 1, column 0: Unknown id:  bar
```

and capture their defining environment.

```
  func foo () (Func) (
    let bar 42
    func baz () (Int) bar
    &baz
  )

[]
  foo

[Func(baz)]
  call _

[42]
```

### debugging tools
Values may be dumped to `cout` using `dump`.

```
  dump 42

42
[]
```

### performance
/>mpl currently runs around around 5 times as slow as Python3.<br/>

`bench` returns elapsed time in ms for running its body `n` times.

```
  func fib (n Int) (Int)
    if < n 2 n + fib - n 1 fib - n 2
  bench 100 (fib 20 d)

[1233]
```
