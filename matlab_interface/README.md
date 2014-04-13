# Getting Started

First, ensure that your workspace is properly setup.

Then, if you wish to test this package in isolation, first build the package as chained workspace:

	roscd matlab_interface
	mkcd build
	cmake ..
	make

Next, run the basic matlab test:

	rosrun matlab_interface run_test.sh

or

	roscd matlab_interface
	./test/run_test.sh

This will run `matlab` with the `-nodesktop` and `-nosplash` options.
(Make sure that the binary `matlab` is available on your `$PATH` and accepts arguments!).
