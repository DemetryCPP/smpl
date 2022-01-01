# What is SMPL
SMPL is mini programming language. Repository contains his interpeter.

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
SMPL have two functions: `input` and `print`
Warning: use `input(0)` ().

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