#ifndef NETWORK_H
#define NETWORK_H

#include "parameters.hpp"
#include "neuron.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <array>

/** A network of neurons is what is being simulated.
 * An ensemble of neurons, that evolves over time and reacts to stimuli that arrive from the rest of the brain.
 */
class Network
{
	public:
	

	/** A constructor.
	 * Initializes a neuronal network by creating and connecting a number of neurons specified in the parameter file by means of the functions createNeurons() and establishConnections().
	   There is a defined ratio of inhibitory and exitatory neurons and each one of them receives a fixed number of connections from both inhibitory and excitatory presynaptic neuron that are chosen randomly. */
	Network();

	
	/// A destructor which deletes all neurons of the network.
	~Network();
	
	/// A method updating all of the network's neuron by one step which is used in the main loop.
	void update();
	
	
	//print data
	/**Prints each neuron's spike times and neuron id in a file of given name using the private function print simulation data.
	 * @param nameOfFile a string*/
	void printSimulationData(const std::string& nameOfFile) const;
	/**Prints each neuron's spike times that lie in an interval defined by the simulation and neuron id in a file of given name using the private function print simulation data.
	 * @see Simulation::printDataForBrunelFigureToFile()
	 * @param nameOfFile a string*/
	void printSimulationDataWithinTimeInterval(const std::string& nameOfFile) const;
	
	//void printSimulationDataWithinTimeInterval(const std::string& nameOfFile, unsigned int beginInterval, unsigned int endInterval) const; // Would be nice to have but is nasty to program with my class architecture
		
	/**Calculates the mean spike rate of the network's neurons in an interval to indicate.
	 * @see Simulation::getMeanSpikeRateInInterval
	 * @see Simulation::printDataForBrunelFigureToFileWithMeanSpikingRate
	 * @param beginInterval to investigate in steps an unsigned int
	 * @param endInterval to investigate in steps an unsigned int*/ 
	double getMeanSpikeRateInInterval(unsigned int beginInterval, unsigned int endInterval) const;
	
	/**Goes through the network's neurons and calculates the mean number of targets per neuron.
	 * @see neuron_unitTest.cpp */ 
	double getMeanNumberOfTargetsPerNeuron() const;
	/**Goes through the network's neurons and calculates the mean number of excitatory targets per neuron.
	 * @see neuron_unitTest.cpp */ 
	double getMeanNumberOfExcitatoryTargetsPerNeuron() const;
	
	private:
	std::array<Neuron*, TOTAL_NUMBER_OF_NEURONS_N> neurons; ///< A container carrying the neurons forming the network, an array of pointers to neurons.
	
	//creation of network
	/**Auxiliary function that creates a number of neurons defined the parameter file.
	 * @see Neuron()*/
	void createNeurons();
	/**Auxiliary function that creates the connections between neurons.
	  *@see Neuron()*/
	void establishConnections();
	
	//fetch data
	
	/**An auxiliary function for printing the network's spike times to a file that allows to avoid the duplication of code.
	 * @see printSimulationData(const std::string& nameOfFile)
	 * @see printSimulationDataWithinTimeInterval
	 * @param nameOfFile a string
	 * @param getIteratorBegin a member function that yields an iterator on a vector<unsigned int> pointing the first element that is printed
	 * @param getIteratorEnd a member function that yields an iterator on a vector<unsigned int> pointing the last element that is printed */
	void printSimulationData(const std::string& nameOfFile, std::vector<unsigned int>::const_iterator (Network::*getIteratorBegin)(unsigned int) const , std::vector<unsigned int>::const_iterator (Network::*getIteratorEnd)(unsigned int) const ) const;
	
	/**An auxiliary function for printing the network's spike times to a file that returns an iterator pointing on neuron of given id's first spike time.
	 * @see printSimulationData(const std::string& nameOfFile)
	 * @param neuronId an unsigned int */
	std::vector<unsigned int>::const_iterator getIteratorToBegin(unsigned int neuronId) const;
	
	/**An auxiliary function for printing the network's spike times to a file that returns an iterator pointing on neuron of given id's last spike time.
	 * @see printSimulationData(const std::string& nameOfFile)
	 * @param neuronId an unsigned int */
	std::vector<unsigned int>::const_iterator getIteratorToEnd(unsigned int neuronId) const;
	
	/**An auxiliary function for printing the network's spike times to a file that returns an iterator pointing on neuron of given id's first spike time that lies within an interval defined in the parameter file.
	 * @see printSimulationDataWithinTimeInterval(const std::string& nameOfFile)
	 * @param neuronId an unsigned int */
	std::vector<unsigned int>::const_iterator getIteratorToBeginInterval(unsigned int neuronId) const;
	
	/**An auxiliary function for printing the network's spike times to a file that returns an iterator pointing on neuron of given id's last spike time that lies within an interval defined in the parameter file.
	 * @see printSimulationDataWithinTimeInterval(const std::string& nameOfFile)
	 * @param neuronId an unsigned int */
	std::vector<unsigned int>::const_iterator getIteratorToEndInterval(unsigned int neuronId) const;
	
	//testing connectivity
	/**Auxiliary function .
	  *@see getMeanNumberOfTargetsPerNeuron()
	  *@see getMeanNumberOfExcitatoryTargetsPerNeuron()
	  *@param getNumberTargets a member function of neuron that yields an unsigned int */
	double getMeanNumberOfTargetsPerNeuron(size_t (Neuron::*getNumberTargets)() const ) const;
};


#endif
