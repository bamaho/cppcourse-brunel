#include "excitatoryNeuron.hpp"
#include "parameters.hpp"


using namespace std;

ExcitatoryNeuron::ExcitatoryNeuron()
:Neuron()
{}

double ExcitatoryNeuron::getSpikeAmplitude() const
{
	return SPIKE_AMPLITUDE_J_EXCITATORY_NEURON;
}
