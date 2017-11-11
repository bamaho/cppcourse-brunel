#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <cmath>
#include <cassert>
#include <string>

//Parameters


	//Time
constexpr unsigned int INITIAL_TIME(0);	//starting time of the simulation, in steps
constexpr unsigned int FINAL_TIME(12000);	//time when the simulation ends, in steps
constexpr double MIN_TIME_INTERVAL_H(0.1);	//minimal time interval in milliseconds, duration of one time step, H*(number of steps) gives the actual time in milliseconds
constexpr unsigned int SIGNAL_DELAY_D(15); //delay that the signal undergoes between emission and reception, for the sake of convenience uniform, in steps

//Fetch Data
constexpr unsigned int TIME_BEGIN_PRINT_TO_TXT_FILE_BY_DEFAULT(10000); //simulation time interval in which the data gets printed to a text file, shouldn't between initial and final time of the simulation
constexpr unsigned int TIME_END_PRINT_TO_TXT_FILE_BY_DEFAULT(12000);

const std::string NAME_OF_FILE("simulationData.txt"); //If not otherwise specified the data gets printed in a file of this name

	//Current
constexpr double EXTERNAL_CURRENT_BY_DEFAULT(0); //current applied to the neuron from the outside in piktoampere, by default zero, is not accounted for when simulating an entire network

constexpr double J_INHIBATORY_OVER_J_EXCITATORY_G(4.5); //J_INHIBATORY/J_EXCITATORY

constexpr double SPIKE_AMPLITUDE_J_EXCITATORY_NEURON(0.1);	//spike amplitude of an excitatory neuron
constexpr double SPIKE_AMPLITUDE_J_INHIBITORY_NEURON(SPIKE_AMPLITUDE_J_EXCITATORY_NEURON*J_INHIBATORY_OVER_J_EXCITATORY_G);	//spike amplitude of an inhibitory neuron

constexpr double SPIKE_AMPLITUDE_J(0.1); 	//post synaptic spike response amplitude, defines the impact of one received spike on the membrane potential, changed for later version required for testing only, has no impact on simulation

	//Network
constexpr unsigned int TOTAL_NUMBER_OF_NEURONS_N(12500);	//number of neurons in a simulation
constexpr double PERCENT_EXCITATORY_NEURONS(80);	//percent of excitatory neurons in a simulation
constexpr unsigned int NUMBER_OF_EXCITATORY_NEURONS_Ne(TOTAL_NUMBER_OF_NEURONS_N*PERCENT_EXCITATORY_NEURONS/100); //number of excitatory neurons in a simulation
constexpr unsigned int NUMBER_OF_INHIBITORY_NEURONS_Ni(TOTAL_NUMBER_OF_NEURONS_N-NUMBER_OF_EXCITATORY_NEURONS_Ne); //number of inhibitory neurons in a simulation

constexpr unsigned int NUMBER_OF_CONNECTIONS_FROM_NEURONS_C(1);	//each neuron receives C randomly chosen connections from other neurons in the network, since this value is also important for the computation of the membrane resistance R it can't have its actual value for the testing of the older parts of the program to work, MUST NOT BE ZERO FOR THIS RESULTS IN UNDEFINED BEHAVIOUR, DIVISION BY ZERO!

constexpr double RATIO_C_OVER_N_E(0.1);//The ratio Ce/Ne is equal to the ratio Ci/Ni

constexpr unsigned int NUMBER_OF_CONNECTIONS_FROM_EXCITATORY_NEURONS_Ce(NUMBER_OF_EXCITATORY_NEURONS_Ne*RATIO_C_OVER_N_E); // Ce = Cext, the number of connections from excitatory from the rest of the brain that fire arbitrarily at a given rate
constexpr unsigned int NUMBER_OF_CONNECTIONS_FROM_INHIBITORY_NEURONS_Ci(NUMBER_OF_INHIBITORY_NEURONS_Ni*RATIO_C_OVER_N_E);


//Membrane Potential
constexpr double INITIAL_MEMBRANE_POTENTIAL(0);	//initial membrane portential in mV
constexpr double MEMBRANE_POTENTIAL_THRESHOLD(20);	//value in mV of mambrane potential that, once reached, causes the neuron to spike
constexpr double RESET_MEMBRANE_POTENTIAL(0); //value in mV that the membrane potential takes just after the spike
constexpr unsigned int REFRACTION_PERIOD(20);	//period after spike during which the neuron is insenstitive to stimulation, in steps

constexpr unsigned int TIME_CONSTANT_TAU(20);	//Tau = membrane resistance * Number of Connections from neurons, in milliseconds (!), MUST NOT BE ZERO FOR THIS RESULTS IN UNDEFINED BEHAVIOUR, DIVISION BY ZERO WHEN THE MEMBRANE POTENTIAL IS UPDATED!

constexpr double MEMBRANE_RESISTANCE_R(TIME_CONSTANT_TAU/NUMBER_OF_CONNECTIONS_FROM_NEURONS_C);	//in GΩ, the neuron can be thought of as a simple electrical circuit
constexpr double INTERMEDIATE_RESULT_UPDATE_POTENTIAL(exp(-MIN_TIME_INTERVAL_H/TIME_CONSTANT_TAU));	//an expression which has to be calculated multiple times when updating the membrane potential and that is constant

//Activity of the rest of the brain
constexpr double RATIO_V_EXTERNAL_OVER_V_THRESHOLD(0.9); //mean frequency of stimulation from the rest of  over the external frequency that was needed to reach the threshold in absence of feedback

#endif
