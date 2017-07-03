# Test the C++ Library

In the file `test.cpp` it is provided a simple test for the Motors module. The test consist moving both motors forward and backward in a step up fashion. It means that the speed will gradually increase in clockwise direction until it reaches the maximum speed, and then the speed is gradually decreased to stop and then the motors will move counter clockwise until it reaches the maximum speed. This is done in a `while` loop, when you stop the program the motors will continue to rotate, so you stop them by pressing the `reset button` in the Motors Module.

To run the test follow these steps:

* Compile the sources with the following command: `make all`

* The previous command will generate an executable file named `runner`, which you can run as follows: `sudo ./runner`.

* To remove the compiled `*.o` files and the dist file `runner`, execute the following line: `make clean`.
