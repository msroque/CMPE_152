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

### [ANTLR4 Grammar Library](https://msroque.github.io/CMPE_152/)

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

<img src="https://github.com/msroque/CMPE_152/blob/master/readme_images/external_tools_config.jpg" width=80% height=80% title="External Tools Configurations" />

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


#### Here is an example using this project's sample.j file

<img src="https://github.com/msroque/CMPE_152/blob/master/readme_images/Screen%20Shot%202022-02-28%20at%201.32.46%20PM.png" width =80% height=80%
     title="Terminal" />

## Credits
Project was created for the Compiler Design course at San José State University. 

Course is headed by Ronald Mak. 

Project created by Michelle Roque, Jason Chong, Joshua Gadia, and Jonathan Naraja. 
