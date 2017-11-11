#include "gtest/gtest.h"
#include "network.hpp"
#include "neuron.hpp"
#include "parameters.hpp"
#include "simulation.hpp"

#include <cmath>
#include <iostream>
#include <vector> 

 void updateNeuronNTimes(Neuron& neuron, const unsigned int n) //auxilliary function that allows to update a neuron n times
{
	
	for(size_t i(0);i < n;++i)
	{
		neuron.updateWithoutBackgroundNoise();
	}
	
}
 
 /*
 * Tests that require certain parameters to have precisely the indicated values:
 * - initial membrane potential = 0 mV
 * - membrane potential threshold = 20 mV
 * - reset membrane potential = 0 mV
 * - refraction period = 2 ms
 * - time constant tau = 20 ms
 * - number of connections from neurons C = 1
 * - and thus the membrane resistance R = 20 GΩ (=tau/C)
 * */
 
 TEST(oneNeuron, spikeTimes)	//with an external input current of 1.01 mV neuron spikes after 92.4, 186.8, 281.2 and 375.6 ms, this is being tested
 {
	Neuron neuron;
	neuron.setInputCurrent(1.01);
	
	updateNeuronNTimes(neuron, 924);
	EXPECT_TRUE(neuron.getSpikeTime().empty());
	neuron.update();
	updateNeuronNTimes(neuron, 1);
	EXPECT_FALSE(neuron.getSpikeTime().empty());
	updateNeuronNTimes(neuron, 3000);
	
	std::vector<unsigned int> calculatedSpikeTimes({924,1868,2812,3756});
	EXPECT_EQ(calculatedSpikeTimes,neuron.getSpikeTime());
	
}
 
 
 TEST(oneNeuron, membranePotentialTendsToVthr)	//with an external input current of 1 mV the membrane potential tends asymtotically to V_Threshold without reaching it
 {
	Neuron neuron;
	neuron.setInputCurrent(1);
	neuron.updateWithoutBackgroundNoise();
	
	EXPECT_DOUBLE_EQ( (TIME_CONSTANT_TAU/NUMBER_OF_CONNECTIONS_FROM_NEURONS_C)*(1-exp(-MIN_TIME_INTERVAL_H/TIME_CONSTANT_TAU)),neuron.getMembranePotential()); //tests if the first update of the membrane potential one correct
	
	updateNeuronNTimes(neuron, 10000);
	EXPECT_EQ(0,neuron.getNumberOfSpikes());
	EXPECT_NEAR (MEMBRANE_POTENTIAL_THRESHOLD, neuron.getMembranePotential(), 0.1);
	
}
 
 
TEST(oneNeuron, updateMembranePotentialWithPosExternalCurrent) //tests if the membrane potential is correctly updated after one step if the neuron doesn't receive any spike but with stimulation through an external current after one time step
{
	Neuron neuron;
	double externalCurrent(1.01);
	neuron.setInputCurrent(externalCurrent);
	neuron.updateWithoutBackgroundNoise();
	
	EXPECT_DOUBLE_EQ(exp(-MIN_TIME_INTERVAL_H/TIME_CONSTANT_TAU)*INITIAL_MEMBRANE_POTENTIAL + (TIME_CONSTANT_TAU/NUMBER_OF_CONNECTIONS_FROM_NEURONS_C)*externalCurrent*(1-exp(-MIN_TIME_INTERVAL_H/TIME_CONSTANT_TAU)),neuron.getMembranePotential());
	
}

TEST(twoNeurons, ringBuffer) //tests if a spike arrive with the right delay and amplitude, tests that the order in which the neurons are updated is incidental
{
	//part one - spiking neuron gets updated before receiving one, one spike
	Neuron n1;
	Neuron n2;
	
	n1.addTarget(&n2);	//establishing a connection between the neurons
	
	n1.setInputCurrent(1.01);
	updateNeuronNTimes(n1, 924);	//neuron_1 spikes and thus sends a signal to the second
	updateNeuronNTimes(n2, 924);
	EXPECT_TRUE(n1.getSpikeTime().empty());
	n1.updateWithoutBackgroundNoise();
	EXPECT_FALSE(n1.getSpikeTime().empty());//tests if it spiked
		
	for(size_t i(0);i < SIGNAL_DELAY_D;i++)
	{
		n2.updateWithoutBackgroundNoise();
		EXPECT_NEAR(n2.getMembranePotential(),0,0.00001); //Verifies that the spike doesn't arrive before it is supposed to
	}
	
	n2.updateWithoutBackgroundNoise();
	EXPECT_NEAR(n2.getMembranePotential(),SPIKE_AMPLITUDE_J,0.00001); //Verifies that the spike arrives when it should
	n2.updateWithoutBackgroundNoise();
	EXPECT_NEAR(n2.getMembranePotential(),SPIKE_AMPLITUDE_J * INTERMEDIATE_RESULT_UPDATE_POTENTIAL,0.00001); //Verifies that the spike doesn't arrive later it is supposed to
	
	//part two - receiving neuron gets updated before receiving one, n spikes
	Neuron n3;
	Neuron n4;
	constexpr unsigned int n(100);
	
	for(size_t i(0); i < n; i++) //spiking n times
	{
		n3.addTarget(&n4);
	}

	updateNeuronNTimes(n4,925);
	
	n3.setInputCurrent(1.01);	//make neuron spike
	updateNeuronNTimes(n3,925);
	
	EXPECT_FALSE(n3.getSpikeTime().empty());
	
	
	for(size_t i(0); i < SIGNAL_DELAY_D; i++)
	{
		n4.updateWithoutBackgroundNoise();
	}
	EXPECT_NEAR(n4.getMembranePotential(),SPIKE_AMPLITUDE_J*n,0.00001);
	
}

TEST(neuronalNetwork, initialisation) //tests if each neuron receives the correct number of spikes and is thus properly connected. Even though the number of targets can vary, the mean value of all targets is equal to Cinhibitory + Cexcitatory
{
	Network network;
	EXPECT_NEAR(network.getMeanNumberOfTargetsPerNeuron(),NUMBER_OF_CONNECTIONS_FROM_EXCITATORY_NEURONS_Ce+NUMBER_OF_CONNECTIONS_FROM_INHIBITORY_NEURONS_Ci,0.00001);
	EXPECT_NEAR(network.getMeanNumberOfExcitatoryTargetsPerNeuron(),NUMBER_OF_CONNECTIONS_FROM_EXCITATORY_NEURONS_Ce,0.00001);
	
}

TEST(oneNeuron, randomBackgroundNoise) //tests if the variance resp. the expected value of the expression a*poissson(x) is equal to a*a*var(poisson(x)) resp. a*mean(poisson(x)) as expected
{
	Neuron neuron;
	
	std::vector<double> backgroundNoise;
	
	for(size_t i(0);i<1000000;i++)// calculate expected value
	{
		backgroundNoise.push_back(neuron.getBackgroundNoise());
	}
	
	double meanValue(std::accumulate(backgroundNoise.begin(), backgroundNoise.end(), 0.0) / backgroundNoise.size());
	
	EXPECT_NEAR(RATIO_V_EXTERNAL_OVER_V_THRESHOLD*MEMBRANE_POTENTIAL_THRESHOLD*MIN_TIME_INTERVAL_H/TIME_CONSTANT_TAU, meanValue, 0.001);//tests if the expected value is the right one
	
	for(auto& element:backgroundNoise) //calculate variance
	{
		element=pow(element-meanValue,2);
	}
	EXPECT_NEAR(RATIO_V_EXTERNAL_OVER_V_THRESHOLD*MEMBRANE_POTENTIAL_THRESHOLD*MIN_TIME_INTERVAL_H*SPIKE_AMPLITUDE_J/TIME_CONSTANT_TAU,std::accumulate(backgroundNoise.begin(), backgroundNoise.end(), 0.0) / backgroundNoise.size(),0.001);
	
}

/*TEST(simulation, averageSpikeRate) //tests if the mean spike frequency is close to the one indicate in brunel's paper, seems to be too time consuming for a unit test. Therfore the comparison of these values is given when excecuting the program.
{
	Simulation simulation;
	EXPECT_NEAR(simulation.getMeanSpikeRateInInterval(6,4,2000,12000),55.8,10);
	
}*/

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
