This program implements a mathematical model proposed by Mr. Brunel that describes the behaviour of a simplistic neuronal network formed by 12'500 neurons of which 2'500 are of inhibitory and 10'000 of excitatory nature. Depending on the parameters chosen, different patterns regarding the network's behaviour are generated. The program in the current form allows to chose between four settings of parameters, that correspond to four graphs in Brunel's paper (Figure 8). For graphs B, C, and D the mean spiking frequency of a neuron is calculated and displayed on the terminal along with the corresponding values that are given by Brunel for these settings.

Running the program:

	1)The following arcitecture should be in place:

	/build - where commands are excecuted

	/src
		gtest/...
		CMakeLists.txt
		Doxyfile.in
		pyscript.py
	
		neuron.hpp
		neuron.cpp
		excitatoryNeuron.hpp
		excitatoryNeuron.cpp
		inhibitoryNeuron.hpp
		inhibitoryNeuron.cpp
		network.hpp
		network.cpp
		simulation.hpp
		simulation.cpp

		main.cpp
		parameters.hpp
		neuron_unitTest.cpp
	
	/docs - where the doxygen documentation is created

	
	2)Open a terminal and go to /build

	3)Type: "cmake ../src", then: "make"

	4)To generate the doxygen documentation: "make doc"

	5)Then to run the program: "./neuron" or the unit test: "./neuron_unitTest"

