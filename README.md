# (SCC0215) File Organization - Final Project

### Developers 
* Gustavo Gabriel Ribeiro
* Artur De Vlieger Lima

### Professors

* Cristina Dutra de Aguiar
* João Paulo Clarindo
* Eduardo Souza Rocha

## Description

### Introduction

This project focuses on the manipulation of binary files in secondary memory. The entire program is implemented in C, without the use of any external libraries other than the standard C libraries, and with minimal use of the GNU Makefile tool. All code presented here was written exclusively by the duo. The development period of this project took place during the entire first semester of 2023.

### Objectives

Ten functionalities analogous to those present in Database Management Systems (DBMSs) were developed to manipulate a real database provided by the Secretariat of Public Security of São Paulo, containing information about cellphone thefts.

### The developed functionalities are:
1. Create table
2. Select
3. Create index
4. Select where
5. Delete
6. Insert into
7. Update
8. Create tree
9. Select where tree
10. Insert into tree

### Distribution

There were a total of 3 deliveries over 4 months, following this schedule:
1. Delivery
    - Create table
    - Select
2. Delivery
    - Create index
    - Select where
    - Delete
    - Insert into
    - Update
3. Delivery
    - Create tree
    - Select where tree
    - Insert into tree

This project is cumulative and sequential, with each functionality depending on its predecessor. The final delivery corresponds to the fully operational version.

### Execution

Each delivery has a set of test cases, which can be found in its ./tests folder.

To execute the program, first use the command to generate the executable:<br>
`make all`

Then, use the command to run the program:<br>
`make run`

Finally, choose a *.in* file in the *./test/in/* folder to use as input or manually input the required fields.

<em>Note: There is a detailed explanation, in Portuguese, for each of the functionalities and work requirements in the corresponding folders for their deliveries, which were provided to students by the team of Professors for the project's understanding <em>


## Final Thoughts

In this project, we had the opportunity to explore the inner workings of Database Management Systems (DBMSs) and the manipulation of binary files. It was an enriching experience, as we could better understand how DBMSs handle data at a lower level, from storage to internal search and retrieval functionalities.

## Grade
*We achieved the maximum score of 10.0 points*   
