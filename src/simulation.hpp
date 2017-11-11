#ifndef SIMULATION_H
#define SIMULATION_H

#include "parameters.hpp"
#include "network.hpp"

#include <string>

/** The class simulation allows to specify the simulations precise parameter and makes running the simulation easier for the user.*/
class Simulation
{
	public:
	
	/// A constructor.
	Simulation();
	
	/**A function that allows to chose one of the four graphs from Brunel that is then reproduced.
	 * Once the graph is chosen, the simulation gets run for the desired parameters. Then a window opens and displays the scatter diagram. If it is closed, the histogram appears. If none of the graphs is chosen, the program stops.
	   For scenarios B, C and D the mean firing rate of the neurons is computed and a reference value from Brunel is given.
	 * @see int main()*/
	int runBrunel();
	
	///A getter of the simulation time from when on the spikes get printed to the text file.
	/**@see Network::getIteratorToBeginInterval(unsigned int neuronId)*/
	static unsigned int getTimeBeginPrintToTxtFile();// Due to the (too) complicated class architecture.
	///A getter of the simulation time up to when the spikes get printed to the text file.
	/**@see Network::getIteratorToEndInterval(unsigned int neuronId)*/
	static unsigned int getTimeEndPrintToTxtFile();
	

	/** A method allowing to specify the necessary simulation parameters (but not the interval) in order to obtain the data required the mean firing rate of the simulation's neurons in a given interval. Seems to be too time consuming to work out in a unitTest.
	 * @see neuron_unitTest.cpp
	 * @param ratioJinoverJexG a double
	 * @param ratioVextOverVthr a double
	 * @param timeBeginMeasurement an unsigned int
	 * @param timeEndMeasurement unsigned int */
	double getMeanSpikeRateInInterval(double ratioJinoverJexG, double ratioVextOverVthr, unsigned int timeBeginMeasurement, unsigned int timeEndMeasurement);
	
	private:
	
	static unsigned int timeBeginPrintToTxtFile;///< A static parameter specifying from when on the spikes get printed to the text file, an unsigned int.
	static unsigned int timeEndPrintToTxtFile;///< A static parameter specifying up to when the spikes get printed to the text file, an unsigned int.
	
	Network network;///< The ensemble of neurons that is being studied.	
	
	/**A method allowing to specify the necessary simulation parameters in order to obtain the data required for the reproduction of Brunel's figures.
	 * @see runBrunel()
	 * @see  printDataForBrunelFigureToFileWithMeanSpikingRate
	 * @param ratioJinoverJexG a double
	 * @param ratioVextOverVthr a double
	 * @param timeBeginMeasurement an unsigned int
	 * @param timeEndMeasurement an unsigned int
	 * @param nameOfFile a string */
	void printDataForBrunelFigureToFile(double ratioJinoverJexG, double ratioVextOverVthr, unsigned int timeBeginMeasurement = TIME_BEGIN_PRINT_TO_TXT_FILE_BY_DEFAULT, unsigned int timeEndMeasurement = TIME_END_PRINT_TO_TXT_FILE_BY_DEFAULT, const std::string& nameOfFile = NAME_OF_FILE);
	
	/**A method allowing to specify the necessary simulation parameters (but not the interval) in order to obtain the data required for the reproduction of Brunel's figures and ,as a test, the mean firing frequency of the simulation's neurons.
	 * @see runBrunel()
	 * @param ratioJinoverJexG a double
	 * @param ratioVextOverVthr a double */
	double printDataForBrunelFigureToFileWithMeanSpikingRate(double ratioJinoverJexG, double ratioVextOverVthr);
	
	/**An auxiliary method in order to modularize the code
	 * @see printDataForBrunelFigureToFile()
	 * @param durationOfSimulation a unsigned integer */
	void run(unsigned int durationOfSimulation = FINAL_TIME);
};


#endif
