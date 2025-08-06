# LISP Interpreter

This is a personal project implementing a basic **LISP interpreter** in C++. It supports core functionality such as parsing S-expressions, evaluating arithmetic and logical operations, and handling nested expressions.

## Features

- Parses and evaluates LISP-style expressions
- Supports nested expressions and parentheses
- Arithmetic operations: `+`, `-`, `*`, `/`
- Logical operations: `and`, `or`, `not`
- Variable assignment and lookup (if included)
- Simple REPL-like interface

## Getting Started

### Build

This project uses a standard C++ compiler:

```bash
g++ -o lisp main.cpp parser.cpp evaluator.cpp
./lisp
```

### Usage

Once compiled, run the interpreter and input LISP expressions:

```lisp
> (+ 1 2)
3

> (* (+ 1 2) (- 5 3))
6
```

## Project Structure

```
/lisp_interpreter/
├── main.cpp            # Entry point and REPL
├── parser.cpp/.h       # Tokenization and parsing into AST
├── evaluator.cpp/.h    # Evaluates parsed expressions
├── environment.cpp/.h  # Variable and scope management (if implemented)
├── test/               # Test cases (if included)
└── README.md
```

## Future Improvements

- Add support for `define`, `lambda`, and function calls
- Include conditional expressions (`if`)
- Improve error handling and debugging tools

## Requirements

- C++17 or later
- Standard libraries only (no external dependencies)
- Vagrant (Designed to run in a Linux environment)

## Author

**Logan Johnson**\
This project is a learning exercise in interpreters and functional programming concepts using C++.

