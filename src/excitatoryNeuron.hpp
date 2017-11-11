#ifndef EXCITATORY_NEURON_H
#define EXCITATORY_NEURON_H

#include "parameters.hpp"
#include "neuron.hpp"

/** A subclass of Neuron.
 * A class that allows excitatory neurons to adopt specific behaviour, namely having a distinct spike amplitude.*/
class ExcitatoryNeuron : public Neuron
{
	public:
	
	///A constructor, calling the constructor of the superclass Neuron.
	ExcitatoryNeuron();
	
	private:
	
	///A getter of the excitatory neuron's spike amplitude.
	/** Reads the excitatory neuron's spike amplitude from the parameter file.
	 *@see Neuron::spike()
	 *@see Neuron::getNumberOfExcitatoryTargets()
	 *@return the excitatory neuron's spike amplitude, a double */
	double getSpikeAmplitude() const override;
};

#endif
