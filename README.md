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
/>mpl v7

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

### bindings
Identifiers may be bound once per scope using `let`.

```
  let foo 42

[]
  let foo 42

Error in 'repl' at line 1, column 0: Dup binding: foo
```

`_` may be used as a placeholder to pop the stack.

```
  42

[42]
  let foo _

[]
  foo

[42]
```

### groups
Parens may be used to group forms.

```
  let foo (dump 'binding 42)
  foo

'binding
[42]
```

### functions
New functions may be defined using `func`.

```
  func foo () (Int) 42

[]
  foo

[42]
```

Anonymous functions may be created by simply omitting the name.

```
  func () (Int) 42

[Func()]
  call _

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

Error in 'repl' at line 1, column 0: Unknown id: bar
```

and capture their defining environment.

```
  func foo () (Func) (
    let bar 42
    func () (Int) bar
  )

[]
  call _ foo

[42]
```

Functions may be referenced by prefixing with `&`.

```
  func foo () (Int) 42

[]
  &foo
  
[Func(foo)]
```

#### call flags
Call flags may be specified by prefixing with `|`.

##### |d(rop)
Drops returned values.

```
  func foo () (Int) 42
  foo|d
  
[]
```

##### |t(co)
Performs tail call optimization, may be used outside of tail call position which causes an immediate return.

```
  func foo (n Int) (Sym)
    if = n 0 'done foo|t - n 1
  foo 42
  
['done]
```

### debugging tools
Values may be dumped to `cout` using `dump`.

```
  dump 42

42
[]
```

### performance
/>mpl currently runs 3+ times as slow as Python3.<br/>

`bench` returns elapsed time in ms for running its body `n` times.

```
$ cd bench
$ python3 fibrec.py
233
```

```
  func fibrec (n Int) (Int)
    if < n 2 n + fibrec - n 1 fibrec - n 2
  bench 100 (fibrec|d 20)

[1233]
```

```
$ python3 fibtail.py
105
```

```
  func fibtail (n Int a Int b Int) (Int)
    if = n 0 a if = n 1 b fibtail|t - n 1 b + a b
  bench 10000 (fibtail|d 70 0 1)

[278]
```
