#ifndef INHIBITORY_NEURON_H
#define INHIBITORY_NEURON_H

#include "parameters.hpp"
#include "neuron.hpp"

/** A subclass of Neuron.
 * A class that allows inhibitory neurons to adopt specific behaviour, namely having a distinct spike amplitude.*/
class InhibitoryNeuron : public Neuron
{
	public:
	
	///A constructor, calling the constructor of the superclass Neuron.
	InhibitoryNeuron();
	
	/**A setter of the static attribute ratioJinoverJexG.
	 * Sets the static attribute and simulation parameter ratioJinoverJexG which, 
	   specified by methods of the class Simulation, defines the inhibitory's spike amplitude.
	 * @see Simulation::setRatioJinoverJexG
	 * @see Simulation::printDataForBrunelFigureToFile
	 * @see Simulation::getMeanSpikeRateInInterval
	 * @param ratioJinoverJexG_ a double */
	static void setRatioJinoverJexG(double ratioJinoverJexG_);
	
	private:
	static double ratioJinoverJexG;///< a simulation parameter, the ratio of the spike amplitudes of inhibitory and excitatory neurons. Defines the inhibitory neuron's spike amplitude, a double. 
	
	///A getter of the inhibitory neuron's spike amplitude.
	/** Calculates the inhibitory neuron's spike amplitude from ratioJinoverJexG and the excitatory neuron's spike amplitude specified in the parameter file.
	 * @see Neuron::spike()
	 * @see Neuron::getNumberOfExcitatoryTargets()
	 * @return the excitatory neuron's spike amplitude, a double */
	double getSpikeAmplitude() const override;
};


#endif
