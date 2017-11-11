#include "excitatoryNeuron.hpp"
#include "inhibitoryNeuron.hpp"
#include "network.hpp"
#include "neuron.hpp"
#include "parameters.hpp"
#include "simulation.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;

Network::Network()
{
		createNeurons();//creation of neurons
		establishConnections();//establishing connections
}

Network::~Network() //neurons can't exist without a network
{
    for (auto& neuron : neurons) {
        delete neuron;
        neuron = nullptr;
    }
}

void Network::update()
{
	for(auto& neuron: neurons)
	{
		assert(neuron!=nullptr);
		neuron->update();
	}
}


void Network::printSimulationData(const std::string& nameOfFile) const //allows to write only spikes in a certain interval into a file
{
	printSimulationData(nameOfFile,&Network::getIteratorToBegin,&Network::getIteratorToEnd);
}

void Network::printSimulationDataWithinTimeInterval(const std::string& nameOfFile) const
{
	printSimulationData(nameOfFile,&Network::getIteratorToBeginInterval,&Network::getIteratorToEndInterval);
}

double Network::getMeanSpikeRateInInterval(unsigned int beginInterval, unsigned int endInterval) const
{
	assert(endInterval >= beginInterval and endInterval<=FINAL_TIME);
	double meanFrequency(0);
	for(auto const& neuron: neurons)
			{
				
				if(not neuron->getSpikeTime().empty() )
				{
					for(vector<unsigned int>::const_iterator it = lower_bound(neuron->getSpikeTime().begin(), neuron->getSpikeTime().end(), beginInterval); it != upper_bound(neuron->getSpikeTime().begin(), neuron->getSpikeTime().end(), endInterval); ++it)
					{
						meanFrequency ++;
					}
			}
		}
		return meanFrequency/=(neurons.size()*(endInterval-beginInterval)*MIN_TIME_INTERVAL_H*0.001);
}

//testing connectivity
double Network::getMeanNumberOfTargetsPerNeuron() const
{
	return getMeanNumberOfTargetsPerNeuron(&Neuron::getNumberOfTargets);
}

double Network::getMeanNumberOfExcitatoryTargetsPerNeuron() const
{
	return getMeanNumberOfTargetsPerNeuron(&Neuron::getNumberOfExcitatoryTargets);
}


vector<unsigned int>::const_iterator Network::getIteratorToBeginInterval(unsigned int neuronId) const
	{
		assert(neuronId<neurons.size());
		return lower_bound(neurons[neuronId]->getSpikeTime().begin(), neurons[neuronId]->getSpikeTime().end(), Simulation::getTimeBeginPrintToTxtFile());
	}

vector<unsigned int>::const_iterator Network::getIteratorToEndInterval(unsigned int neuronId) const
	{
		assert(neuronId<neurons.size());
		return upper_bound(neurons[neuronId]->getSpikeTime().begin(), neurons[neuronId]->getSpikeTime().end(), Simulation::getTimeEndPrintToTxtFile());
	}
	
vector<unsigned int>::const_iterator Network::getIteratorToEnd(unsigned int neuronId) const
	{
		assert(neuronId<neurons.size());
		return neurons[neuronId]->getSpikeTime().end();
	}
vector<unsigned int>::const_iterator Network::getIteratorToBegin(unsigned int neuronId) const
	{
		assert(neuronId<neurons.size());
		return neurons[neuronId]->getSpikeTime().begin();
	}




//creation of network
	
void Network::createNeurons()
{
	assert(PERCENT_EXCITATORY_NEURONS >=0);//Beware of negative number of neurons
	assert(neurons.size()>=NUMBER_OF_EXCITATORY_NEURONS_Ne);
	assert(neurons.size()==TOTAL_NUMBER_OF_NEURONS_N);
	
	for(size_t i(0); i < NUMBER_OF_EXCITATORY_NEURONS_Ne; i++)
		{
			neurons[i] = new ExcitatoryNeuron;
		}
		
		for(size_t i(NUMBER_OF_EXCITATORY_NEURONS_Ne);i < TOTAL_NUMBER_OF_NEURONS_N; i++)
		{
			neurons[i] = new InhibitoryNeuron;
		}
}

void Network::establishConnections()
{
	default_random_engine randomGenerator;
	
	uniform_int_distribution<int> distributionExcitatoryNeurons(0,NUMBER_OF_EXCITATORY_NEURONS_Ne-1);
	uniform_int_distribution<int> distributionInhibitoryNeurons(NUMBER_OF_EXCITATORY_NEURONS_Ne,TOTAL_NUMBER_OF_NEURONS_N-1);
	
	assert(neurons.size()>=NUMBER_OF_EXCITATORY_NEURONS_Ne);
	assert(neurons.size()==TOTAL_NUMBER_OF_NEURONS_N);
	
		for(const auto& neuron : neurons)
		{
			for(size_t i(0); i < NUMBER_OF_CONNECTIONS_FROM_EXCITATORY_NEURONS_Ce; i++)
			{
				neurons[distributionExcitatoryNeurons(randomGenerator)]->addTarget(neuron);	//Can stimulate itself???
			}
			
			for(size_t i(0); i < NUMBER_OF_CONNECTIONS_FROM_INHIBITORY_NEURONS_Ci; i++)
			{
				neurons[distributionInhibitoryNeurons(randomGenerator)]->addTarget(neuron);
			}
		}
}

void Network::printSimulationData(const std::string& nameOfFile, vector<unsigned int>::const_iterator (Network::*getIteratorBegin)(unsigned int) const , vector<unsigned int>::const_iterator (Network::*getIteratorEnd)(unsigned int) const) const
{
	ofstream out(nameOfFile);
		
		if(out.fail())
		{
			cerr << "Error: impossible to write in file " << nameOfFile << endl;//(Too) simplistic solution!
		}
		
		else
		{
			assert(neurons.size()>=TOTAL_NUMBER_OF_NEURONS_N);//redundancy, I already tested that
			for(size_t i(0); i < TOTAL_NUMBER_OF_NEURONS_N; i++)
			{
				
				if(not neurons[i]->getSpikeTime().empty() )
				{
					for(vector<unsigned int>::const_iterator it = (this->*getIteratorBegin)(i) ; it !=(this->*getIteratorEnd)(i); ++it)//could be solved much easier by calling a function in neuron that prints its data into a file and/or treating data after printing it to a text file
					{
						out << *it*MIN_TIME_INTERVAL_H << '\t'<< i << '\n' ; //prints the times, not in number of steps, but converted milliseconds
					}
				}
			}
		}
		out.close();
}

//testing connectivity
double Network::getMeanNumberOfTargetsPerNeuron(size_t (Neuron::*getNumberTargets)() const ) const
{
	double meanNumberOfTargets(0);
	for(const auto& neuron: neurons)
	{ meanNumberOfTargets+=(neuron->*getNumberTargets)();}
	assert(neurons.size()!=0);
	return meanNumberOfTargets/=neurons.size();
}

