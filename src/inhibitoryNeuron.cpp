#include "inhibitoryNeuron.hpp"
#include "parameters.hpp"

using namespace std;

double InhibitoryNeuron::ratioJinoverJexG(J_INHIBATORY_OVER_J_EXCITATORY_G); //Initializes the inhibitory neuron's static attribute ratioJinoverJexG

InhibitoryNeuron::InhibitoryNeuron()
:Neuron()
{}

double InhibitoryNeuron::getSpikeAmplitude() const
{
	return -(SPIKE_AMPLITUDE_J_EXCITATORY_NEURON*ratioJinoverJexG);
}

void InhibitoryNeuron::setRatioJinoverJexG(double ratioJinoverJexG_)
{
	ratioJinoverJexG = ratioJinoverJexG_;
}
