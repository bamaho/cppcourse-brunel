#include "simulation.hpp"
#include "network.hpp"
#include "neuron.hpp"
#include "parameters.hpp"


#include <cassert>
#include <iostream>

using namespace std;


int main()
{
	Simulation simulation;
	return(simulation.runBrunel());
}
