# J-- Compiler

A compiler for the language J-- (a minimal subset of the Java language) written in C++, compiling to web-assembly text (.WAT files).

This project was broken into 4 separate milestones that iteratively built the compiler.

## Milestone 1 - Scanner
Reads in plaintext file, tokenizing all valid characters and erroring on invalid input. Transforms input "word" into 1 of 35 possible tokens.

## Milestone 2 - Parser
Makes sure all tokens follow the [grammar of the language](https://docs.google.com/document/d/1v5CveJPJUucAdX-ed-qCm1JeQhxQy9cpWzftVqdNCbs/edit?usp=sharing) while converting the data into an AST (Abstract Syntax Tree). 

## Milestone 3 - Analyzer
Executes 4 separate passes analyzing the AST ensuring semantical integrity is held. 

## Milestone 4 - Generator
Traverses AST, creating semantically equivalent WAT code in 4 passes. 
1. Creates global declarations. 
2. Creates function declarations.
3. Creates function bodies.
4. Creates main function.


## Compile
```
J--Compiler/milestone4$ mkdir build
J--Compiler/milestone4$ cd build
J--Compiler/milestone4/build$ cmake ..
J--Compiler/milestone4/build$ make
```

## Run
```
J--Compiler/milestone4$ ./out/cj-- FILENAMEHERE
```
