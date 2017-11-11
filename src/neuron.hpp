#ifndef NEURON_H
#define NEURON_H

#include "parameters.hpp"

#include <array>
#include <string>
#include <vector>

/** The unit cell of a network.
 * A neuron evolves over time, its membrane potential reacts to external stimuli and it can itself send electrical signals to connected neurons.*/
class Neuron
{
	public:
	
	//constructor and deconstructor
	/// A constructor.
	Neuron();
	/// A destructor.
	virtual ~Neuron();	//has to be virtual, since otherwise the object might not get properly destroyed
	
	//getters
	/** A getter for the neuron's membrane potential.
	 * @return the neuron's membrane potential, a double*/
	double getMembranePotential() const;
	
	/** A getter for the number of times the neuron spiked in the course of time. Serves in testing.
	 * Uses the size of the vector storing the spike times.
	 * @return the number of times the neuon spiked, a size_t */
	size_t getNumberOfSpikes() const; 
	
	 /** A getter for the neuron's spike times.
	  * @see Network::getMeanSpikeRateInInterval
	  * @see Network::getIteratorToBeginInterval
	  * @see Network::getIteratorToEndInterval
	  * @see Network::getIteratorToBegin
	  * @see Network::getIteratorToEnd
	  * @return the neuron's spiking times in simulation steps, a const reference to a vector of unsigned integers	*/
	const std::vector<unsigned int>& getSpikeTime() const;
	
		//Testing
	/** A getter of the number of targets the neuron has.
	  * @see Network::getMeanNumberOfTargetsPerNeuron
	  * @return the number of targets the neuron has, an unsigned int 	*/
	size_t getNumberOfTargets() const;
	/** A getter of the number of excitatory neurons among the targets the neuron has.
	  * @see Network::getMeanNumberOfExcitatoryTargetsPerNeuron
	  * @return the number of excitatory neurons among the targets the neuron has, an unsigned int 	*/
	size_t getNumberOfExcitatoryTargets() const;
	
	//setters
	/**A setter for the external input current.
	 * A method allowing to simulate the stimulation of the neuron by an external current.
	 * Is invoked in the main loop of the simulation in order to indicate the strength of the external stimulus. Not required any more in the final version of the program.
	 * @param externalCurrent the external input current a double.
	 */
	void setInputCurrent(double externalCurrent);
	
	//update and related functions
	///Is invoked at each cycle of the simulation and makes the neutron evolve in the course of time.
	/**Advances the neuron one step as a function of its current state by eventual spiking 
	   if the membrane potential has reached a threshold, resting inactive during the refractory period after a spike or 
	   updating the membrane potential and finally handling the ring buffer and the random contribution from the rest of the brain as well as incrementing the neuron's internal clock. 
	   Makes use of the function void update(void (Neuron::*membranePotentialUpdate)()) in order to avoid duplication of code.
	 * 	@see Network::update()*/
	void update();
	
	///The method is similar to void update() but it doesn't account for the random contribution from the rest of the brain. Is invoked at each cycle of the simulation and makes the neutron evolve in the course of time.
	/**Advances the neuron one step as a function of its current state by eventual spiking 
	   if the membrane potential has reached a threshold, resting inactive during the refractory period after a spike or 
	   updating the membrane potential and finally handling the ring buffer as well as incrementing the neuron's internal clock.	*/
	void updateWithoutBackgroundNoise();//A method only involved in testing, enables to run the previous versions of the program
	
	/**This method of a connected neuron is called when the neuron spikes. The spike gets stored in its ring buffer in order to be read at the appropriate time.
	 * @see spike()
	 * @param localTimeOfSpikingNeuron an unsigned integer
	 * @param spikeAmplitude defining the strength of the signal
	 * @return if the neuron is in a refractory state, a bool	*/ 
	void receiveSpike(unsigned int localTimeOfSpikingNeuron, double spikeAmplitude);
	
	
	
	//Network
	/**Establishes a connection to a postsynaptic neuron, by adding it to its target.
	 * @see Network::establishConnections()
	 * @param target a pointer to a neuron that shall receive signals*/
	void addTarget(Neuron* target);
	
	
		//Random Generator
	/** Creates a value which accounts for the contribution of the rest of the brain. This contribution is modeled by Cext excitatory neurons that fire randomly according to a poisson distribution at a frequency vext.
	 * @see updateMembranePotential()
	 * @return a randomly generated  value representing the background noise coming from the rest of the brain, a double	*/
	double getBackgroundNoise() const;
	
	/** Setter of the static attribute ratioVextOverVthr.
	 * @see Simulation::run()	*/
	static void setRatioVextOverVthr(double ratioVextOverVthr_);
	
	private:
	
	double membranePotential; ///< The neuron's most important variable, a double.
	double inputCurrent; ///< The external current that is applied to the neuron, a double, is zero if not altered in the main using its the setter.
	std::vector<unsigned int> spikes; ///< A container that stores the spiking times in the course of time, a vector !!!!!!is not the appropriate choice...a list would provide the required functions as well.
	std::vector<Neuron*> targets; ///< A container containing the postsynaptic neurons, the neurons on which an eventual spike of the current neuron has an impact.
	//I tested vectors and deques against eachother and vector outperformed deques in this case !?
	
	/** Another clock which allows to synchronize the times between the neurons, otherwise a problem arises when it comes to distinguishing between alrady updated and not yet updated neurons in neuron interactions. */
	unsigned int internalTime; ///< A clock keeping track of the neuron's local time, an unsigned integer. 
	
	/** An array containing one more element than the uniform signal delay which allows to record all the incoming spike amplitudes and them being read at the right time. */
	std::array<double, SIGNAL_DELAY_D + 1> incomingSpikes; ///< A ring buffer ensuring spikes arrive with the right signal delay, an array of doubles.
	
	static double ratioVextOverVthr;///< A value determining the frequency of spikes from the rest of the brain.
	
	
	//update and related functions
	///An auxiliary function that allows to avoid duplication of code in update() and updateWithoutBackgroundNoise().
	/**@see update()
	 * @see updateWithoutBackgroundNoise()
	 * @param membranePotentialUpdate a member function without return value */
	void update(void (Neuron::*membranePotentialUpdate)());
	
	/**Calculates and sets the new membrane potential as a function of the current membrane potential, the spikes that arrived with a signal delay and the random background noise arriving from the rest of the brain.
	 * @see update()	*/
	void updateMembranePotential();	
	/**Calculates and sets the new membrane potential as a function of the current membrane potential, the external input current and the spikes that arrived with a signal delay. Similar to updateMembranePotential() but not considering the random background noise arriving from the rest of the brain, but an external current instead.
	 * @see updateWithoutBackgroundNoise()	*/
	void updateMembranePotentialWithoutBackgroundNoise();
	
	/**Compares the neuron's internal time to the time of the last spike and the refraction time in order to test if the neuron is in a refractory period.
	 * @see update(void (Neuron::*membranePotentialUpdate)())
	 * @return if the neuron is in a refractory state, a bool	*/
	bool isRefractory() const;
	
	/**Stores the spiking time and sets the membrane potential to zero and sends an electrical impulse to the connected neurons.
	 * @see update(void (Neuron::*membranePotentialUpdate)())	*/ 
	void spike();

	/**Reads the ring buffer's current entry to receive the spikes arriving with a delay that where emmited by presynaptic neurons.
	 * @see updateMembranePotential()
	 * @see updateMembranePotentialWithoutBackgroundNoise()
	 * @return the sum of spike amplitudes of spikes from connected neurons that are currently arriving, a double	*/
	double readRingBuffer() const;
	
	/**Sets the current ring buffer element to zero and makes it thus ready to record new arriving spikes in another cycle to come.
	 * @see update(void (Neuron::*membranePotentialUpdate)())	*/
	void reinitializeCurrentRingBufferElement();
	
	/**Auxilliary method that yields the associated ring buffer index to a a given time by means of the modulo operator and checks if it is licit.
	 * @see receiveSpike()
	 * @see readRingBuffer()
	 * @see reinitializeCurrentRingBufferElement()
	 * @param time the time of which one wants to know the corresponding ring buffer index an unsigned integer	
	 * @return the ring buffer index corresponding to the given time a size_t*/
	size_t timeToRingBufferIndex(unsigned int time) const; 
	
	/**A virtual method which returns the constant spike amplitude, that differs between inhibitory and excitatory neurons.
	   (The method is defined for unspecified neurons as well so that the tests of previous versions of the program are still functional, otherwise it could be virtual pure.).
	 * @see updateMembranePotential()
	 * @see updateMembranePotentialWithoutBackgroundNoise()
	 * @return the spike amplitudes of the presynaptic neuron, a double	*/
	virtual double getSpikeAmplitude() const;
	
	
	/*//Print Data - printing the data would have been easier to achieve by means of such a function :-(
	void printSpikingTimes(const std::string& nameOfFile) const; and not handling everythin in network*/

};


#endif
