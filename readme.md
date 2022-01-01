# What is SMPL
SMPL is mini programming language. Repository contains his interpeter.

# Compiling and using
```sh
chmod +x compile.sh
./compile.sh test.cpp
```
Warning: required 11 version of g++.

```sh
./smpl <filename>
```

# Syntax

In the end of all lines you must put semicolon.
You can define your function:
```
define name(arg) = expression;
```
Write value to variable
```
name = expression;
```
Call function:
```
name(expression);
```
Warning: Functions have only one argument.

Also he have four operators: `+` (addition), `-` (substraction), `*` (multyplying), `/` (division).

# Standart Library
SMPL has two functions: `input` and `print`
Warning: use `input(0)`.

# Using

Create instance of `SMPL::Interpreter`:
```cpp
auto interpreter = new SMPL::Interpreter();

// or
map<string, SMPL::Interpreter::Func> functions;
map<string, double> variables;

// initialize them...

auto interpreter = new SMPL::Interpreter(functions, variables);
```
And call `eval` method with code:
```cpp
interpreter->eval(yourcode);
```

# Catch errors
Interpreter throws `SMPL::Error *`.
He have type:

`SMPL::Error::Type::UnexpectedToken` - unexpected token.

`SMPL::Error::Type::IsNotAFunction` - environment does not have function, with this name (e->token - her name).

`SMPL::Error::Type::IsNotDefined` - environment does not have variable (e->token - her name).

You can use standart method `SMPL::Error::format()` for get error message.