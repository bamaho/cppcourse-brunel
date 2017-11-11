#include "inhibitoryNeuron.hpp"
#include "network.hpp"
#include "neuron.hpp"
#include "parameters.hpp"
#include "simulation.hpp"

#include <string>
#include <iostream>


using namespace std;

unsigned int Simulation::timeBeginPrintToTxtFile(TIME_BEGIN_PRINT_TO_TXT_FILE_BY_DEFAULT);
unsigned int Simulation::timeEndPrintToTxtFile(TIME_END_PRINT_TO_TXT_FILE_BY_DEFAULT);

Simulation::Simulation()
:network()
{}

int Simulation::runBrunel()
{
	cout << "Please indicate the graph that you would like to reproduce. [A,B,C,D]" << endl << "(Enter anything else to leave the program without running the simulation.)" << endl;
	char readKeyboard;
	cin >> readKeyboard;
	
	switch(readKeyboard)
	{
		case 'A':   printDataForBrunelFigureToFile(3,2,5000,6000);
					return system("python ../src/pyscript.py");
					
		case 'B':	cout << "The neurons have a mean firing frequency of " << printDataForBrunelFigureToFileWithMeanSpikingRate(6,4) <<
					" Hz." << endl << "The corresponding value for this setting from Brunel is in Theory: 55.8 Hz and in Simulation: 60.7 Hz." << endl;
					return system("python ../src/pyscript.py");	
		case 'C':	cout << "The neurons have a mean firing frequency of " << printDataForBrunelFigureToFileWithMeanSpikingRate(5,2) <<
					" Hz." << endl << "The corresponding value for this setting from Brunel is in Theory: 38.0 Hz and in Simulation: 37.7 Hz." << endl;
					return system("python ../src/pyscript.py"); 
		case 'D':	cout << "The neurons have a mean firing frequency of " << printDataForBrunelFigureToFileWithMeanSpikingRate(4.5,0.9) <<
					" Hz." << endl << "The corresponding value for this setting from Brunel is in Theory: 6.5 Hz and in Simulation: 5.5 Hz." << endl;
					return system("python ../src/pyscript.py");
		default: cout << "You did not chose a graph to be generated." << endl; return 1;
	}
}
	
unsigned int Simulation::getTimeBeginPrintToTxtFile()
{
	return timeBeginPrintToTxtFile;
}
	
unsigned int Simulation::getTimeEndPrintToTxtFile()
{
	return timeEndPrintToTxtFile;
}

double Simulation::getMeanSpikeRateInInterval(double ratioJinoverJexG, double ratioVextOverVthr, unsigned int timeBeginMeasurement, unsigned int timeEndMeasurement)
	{
		InhibitoryNeuron::setRatioJinoverJexG(ratioJinoverJexG);
		Neuron::setRatioVextOverVthr(ratioVextOverVthr);
		
		unsigned int simulationTime(INITIAL_TIME);	//initializing the simulation time
		
		while (simulationTime < timeEndMeasurement)	// "<" because the time scale is defined as each interval step going from [t to t+h), t+h isn't in the interval otherwise I would account twice for certain points in time
		{
			network.update();
			simulationTime ++;
		}
		return network.getMeanSpikeRateInInterval(timeBeginMeasurement,timeEndMeasurement);
	}
	
		
void Simulation::printDataForBrunelFigureToFile(double ratioJinoverJexG, double ratioVextOverVthr, unsigned int timeBeginMeasurement , unsigned int timeEndMeasurement, const string& nameOfFile)
{
	InhibitoryNeuron::setRatioJinoverJexG(ratioJinoverJexG);
	Neuron::setRatioVextOverVthr(ratioVextOverVthr);
	timeBeginPrintToTxtFile = timeBeginMeasurement;
	timeEndPrintToTxtFile = timeEndMeasurement;
	run(timeEndMeasurement);
	network.printSimulationDataWithinTimeInterval(nameOfFile);
}
	
double Simulation::printDataForBrunelFigureToFileWithMeanSpikingRate(double ratioJinoverJexG, double ratioVextOverVthr)
{
	printDataForBrunelFigureToFile(ratioJinoverJexG,ratioVextOverVthr);
	return network.getMeanSpikeRateInInterval(2000,12000);
}
	
void Simulation::run(unsigned int durationOfSimulation)
{
	unsigned int simulationTime(INITIAL_TIME);	//initializing the simulation time
	
	cout << "The desired simulation gets excecuted. This can take a moment. Please be patient!" << endl;
	
	while (simulationTime < durationOfSimulation)	// "<" because the time scale is defined as each interval step going from [t to t+h), t+h isn't in the interval otherwise I would account twice for certain points in time
	{
		network.update();
		simulationTime ++;
	}
}
	
