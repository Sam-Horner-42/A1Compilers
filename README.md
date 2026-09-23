# Stelline Compiler Front-End (`A1Compilers`)

![Language](https://img.shields.io/badge/Language-C11%20%2F%20ANSI%20C-blue.svg)
![Build](https://img.shields.io/badge/Build-GCC%20%7C%20Clang%20%7C%20MSVC-green.svg)
![License](https://img.shields.io/badge/License-MIT-orange.svg)

> **A high-performance, modular compiler front-end for the Stelline programming language written in ANSI C.**

---

## 🌟 Overview

`A1Compilers` is a multi-stage compiler front-end implementation designed to analyze and process source code written in the **Stelline** programming language. Built entirely in ANSI C with zero third-party dependencies, it provides custom memory management, deterministic lexical analysis, and predictive syntax parsing.

---

## ✨ Core Features

* **Dynamic Character Buffer Reader (`buffer.c`)**: High-efficiency input stream management supporting Fixed, Additive, and Multiplicative buffer expansion policies with rollback and retract operations.
* **FSA-Driven Lexical Analyzer (`scanner.c`)**: Finite State Automaton (FSA) transition-table lexer converting raw input into typed language tokens with robust error trapping.
* **Recursive-Descent Syntax Analyzer (`parser.c`)**: Top-down syntax parser verifying context-free grammar (EBNF) and reporting precise line and character location diagnostics.
* **Zero External Dependencies**: Built using standard C libraries (`stdio.h`, `stdlib.h`, `string.h`) for maximum portability across POSIX systems and Windows.

---

## 🏗 System Architecture

The compiler processes source code through a three-stage modular pipeline:

```
[ Stelline Source (.stl) ]
            │
            ▼
┌───────────────────────┐
│     Buffer Reader     │  ◄── Dynamic Memory Allocation & Re-allocation
└───────────┬───────────┘
            │ Character Stream
            ▼
┌───────────────────────┐
│    Scanner / Lexer    │  ◄── Transition Table & State Automaton
└───────────┬───────────┘
            │ Token Stream
            ▼
┌───────────────────────┐
│     Parser / AST      │  ◄── Recursive-Descent EBNF Validation
└───────────────────────┘
```

---

## 📂 Repository Structure

```
A1Compilers/
├── include/                 # Header declarations & structure definitions
│   ├── buffer.h             # Buffer structure and allocation prototypes
│   ├── scanner.h            # Token definitions, DFA states & tables
│   └── parser.h             # EBNF recursive-descent parser declarations
├── src/                     # C source implementation files
│   ├── buffer.c             # Dynamic buffer implementation
│   ├── scanner.c            # Finite State Automaton lexical analyzer
│   ├── parser.c             # Recursive descent syntax parser
│   └── main.c               # Compiler CLI entry point and pipeline orchestration
├── tests/                   # Sample Stelline (.stl) test inputs
│   ├── test_valid.stl       # Valid test source files
│   └── test_invalid.stl     # Syntax and lexical error test cases
├── Makefile                 # Automated build script for GCC / Clang
└── README.md                # Project documentation
```

---

## ⚡ Buffer Allocation Modes

The **Buffer Module** supports three distinct operational modes to balance execution speed and memory footprint:

| Mode | Mode Flag | Description | Allocation Strategy |
| :--- | :---: | :--- | :--- |
| **Fixed** | `'f'` | Static buffer size | Rejects input when capacity limit is reached. |
| **Additive** | `'a'` | Linear capacity expansion | Grows capacity by adding a fixed byte increment upon overflow. |
| **Multiplicative** | `'m'` | Exponential expansion | Multiplies current capacity by a growth factor upon overflow. |

---

## ⚙️ Building & Execution

### Prerequisites

* C Compiler: `gcc` (v9.0+), `clang` (v10.0+), or `MSVC` (2019+)
* Build System: `make` (optional)

### Building the Project

Using **GNU Make**:
```bash
# Build the compiler executable
make

# Clean object files and build output
make clean
```

Manual compilation using **GCC**:
```bash
gcc -Wall -Wextra -std=c11 -Iinclude src/buffer.c src/scanner.c src/parser.c src/main.c -o stelline_compiler
```

### Usage Examples

```bash
# Run full compilation pipeline on a source file
./stelline_compiler tests/test_valid.stl

# Run scanner in diagnostic mode with additive buffer expansion
./stelline_compiler -s -m a tests/test_valid.stl
```

---

## 🧪 Testing

Test files are located in the `tests/` directory:

```bash
# Test lexical tokenization
./stelline_compiler -s tests/test_valid.stl

# Test syntax parser error handling
./stelline_compiler tests/test_invalid.stl
```

---

## 📜 License

Distributed under the MIT License. See `LICENSE` for details.
