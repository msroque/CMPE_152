# CMPE 152 - Complier Design
This is the final Project for the Complier Design class made in 2020. The project is a language called ***Michelle*** similar to C/C++.

### This language includes:
* Arithmetic Operations
* Assignment Statements
* Conditional and Looping Control Statements
* Print Statements / Variables
* Function Calls (pass by value)
* Function Declarations
* Function Returns

## Getting Started
### Prerequisites
* [Eclipse](https://www.eclipse.org/downloads/)
* [ANTLR4](https://www.antlr.org/download.html)
* [Jasmin Assembler](http://jasmin.sourceforge.net/)

### Instructions
1. Create a new C++ project in Eclipse
2. Set up configurations to generate jasmin files (.j)
3. Set C++ dialect to C++ 11, the library should include antlr4-runtime in the C++ linker
4. Create the grammar file using antlr 4.8

(insert picture here)

5. Compile the program in Eclipse
   1. In run configurations, provide a name
   2. Add your sample code filename in the arguments under program arguments
6. Build and run the project in order to generate the jasmin code

## Running Tests
1. Go to the terminal, and change the directory to the jasmin folder
2. Build the class file

```console
java -jar jasmin.jar[YourJasminFileName + .j]
```

3. Execute the class file

```console
java -cp .:PascalRTL.jar [YourJasminFileName]
```

Here is an example using this project's sample.j file

(insert picture here)

## Credits
Project was created for the Compiler Design course at San José State University. 

Course is headed by Ronald Mak. 

Project created by Michelle Roque, Jason Chong, Joshua Gadia, and Jonathan Naraja. 
