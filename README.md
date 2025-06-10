<!-- noashalom5@gmail.com -->
# Square matrixs Project

This project implements the `SquareMat` class, which perform various operators on square matrices.  

---

## `SquareMat` Class

This class implements square matrix with two fields: size and a two-dimensional array representing a matrix.

### Explanation of the operator implementation method:
In general, all operators that are also שssignment operators, I implemented the שssignment first and then the regular operator executes the placement according to the desired action.

- `Constructors` – There are three types of constructors: a regular one that receives all the information from the user, one that receives as input the size of the matrix and creates the identity matrix of that size (for power operator and comfortable for tests), and a copy constructor.
- `operator[][]` – Allows direct access to a cell in a matrix in the format matrix[][].
- `Assignment operator (=)` – Delete the old matrix and 'put' a deep copy of the new one instead.
- `operator (+=, +, -=, -, *=, *, %=, %)` – All basic operations between two matrices (no division of a matrix by a matrix) require the same size matrices. (% here is the multiplication of the corresponding cells in their places in the two matrices.)
- `Comparison operators: (<=, <, >=, >, ==, !=)` – The comparison operators determine based on the sum of the elements in the matrix, as defined in the assignment instructions. The size of the matrix does not matter.
- `operator^ (power)` – Multiplies the matrix by itself in powers of 2 until it reaches the power received as output (positive power only, the test is in the function body and not in the signature). If the power is zero - the identity matrix is ​​returned.

### Helper Functions:
- `SumOfMatrix`

---

## Project Files

- `SquareMat.hpp` – Declaration file of the functions under the SquareMat class and member functions
- `SquareMat.cpp` – Implementation file for all functions from the declarations file
- `main.cpp` – Demonstrates performing all operations on square matrices.
- `test.cpp` – Contains tests for all algorithms, including edge cases.

---

## Compile & Run
- `make Main` – Compiles and run the project
- `make test` – Runs the tests (from test.cpp)
- `make valgrind` – Checking memory leaks
- `make clean` – Remove the files from the running

- `make main` – Compile only
- `make test_exe` – Compiles test only
