/*
William Bui
Engr 050
Final Project
15 December 2021
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cmath>
#include "C:\dislin\dislin.h"

using namespace std;

// OUTPUT VOLTAGE MODEL CLASS
class OutputVoltageModel {
	public:
		// Gets Data from User
		void inputValues();
		void setDelta();
		void setMaxTime();
		void setFilename();
		void setSize();
		
		// Calculates Numerical and Analytical Voltage Values
		void calculate();
		double calculateVolt(double t, int i);
		double calculateAVolt(double t);
		void initializeValues();
		void calculateMinMax();
		
		// Print Out Values
		void printTable();
		void writeTable();
		
		// Encapsulating Members
		double* getVolts();
		double* getAVolts();
		double* getTimes();
		double getMin();
		double getMax();
		double getMaxVTime();
		double getMinVTime();
		int getSize();
		string getFilename();
		
	private:
		double delta, maxTime, *volts, *aVolts, *times, min, max, minVTime, maxVTime;
		int size;	
		string filename;
		ofstream write;
};

// VOLT GRAPH CLASS
class VoltGraph {
	public:
		void configurePlot();
		void configureNumericalPlot();
		void configureAnalyticalPlot();
		void plotCurves();
		void plotNumericalCurves();
		void plotAnalyticalCurves();
		void setFloat(double times[], double volts[], double aVolts[]);
		void setSize(int size);
		float* toFloatArr(double arr[]);
		
	private:
		int size;
		float *floatTimes, *floatAVolts, *floatVolts;	
};

// User Interface Function
void userInterface();

// MAIN FUNCTION
int main() {
	cout << "- Engr 050 Final Project: Amplifier Circuit -" << endl;
	cout << "This program solves the DE d^2Vo/dt^2 + 2dVo/dt + 5Vo = 5Vs numerically and analytically." << endl << endl;
	cout << "Initial Conditions:" << endl;
	cout << "dVo/dt = 0, Vo = 0, Vs = 10" << endl << endl;
	
	userInterface();
	
	return 0;
}

// Mutators
void OutputVoltageModel::setDelta() {
	bool loop;
	char answer;
	loop = true;
	
	do {
		cout << "Enter Delta Value: ";
		cin >> delta;
		cout << endl;
		
		if (delta <= 0) {
			cout << "Invalid Delta Value! (Value Can't Be 0 or Negative)\n\n";	
		} if (delta > 0 && delta < 0.00001) {
			cout << "Warning! A delta value this low will cause longer load times. Proceed? (y or n): ";
			cin >> answer;
			cout << endl;
			if (answer == 'y') {
				loop = false;	
			}
		} else {
			loop = false;	
		}
	} while (loop);
}

void OutputVoltageModel::setMaxTime() {
	bool loop;
	loop = true;
	
	do {
		cout << "Enter Maximum Time(in seconds): ";
		cin >> maxTime;
		
		if (maxTime <= 0) {
			cout << "Invalid Time Value! (Value Can't Be 0 or Negative)\n\n";	
		} else {
			loop = false;	
		}
	} while(loop);
}

void OutputVoltageModel::setFilename() {
	cout << "Enter Output File Name(EX. filename.txt): ";
	cin >> filename;	
}

void OutputVoltageModel::setSize() {
	size = (int) (1 + (maxTime / delta));
}

// Accessors
int OutputVoltageModel::getSize() {
	return size;
}

double* OutputVoltageModel::getVolts() {
	return volts;
}

double* OutputVoltageModel::getAVolts() {
	return aVolts;
}

double* OutputVoltageModel::getTimes() {
	return times;
}

string OutputVoltageModel::getFilename() {
	return filename;	
}

double OutputVoltageModel::getMaxVTime() {
	return maxVTime;	
}

double OutputVoltageModel::getMinVTime() {
	return minVTime;	
}

double OutputVoltageModel::getMin() {
	return min;
}

double OutputVoltageModel::getMax() {
	return max;
}

// Value Initializers
void OutputVoltageModel::inputValues() {
	setFilename();
	setMaxTime();
	setDelta();
}

void OutputVoltageModel::initializeValues() {
	setSize();
	
	volts = new double[size];
	aVolts = new double[size];
	times = new double[size];
	
	volts[0] = 0;
    volts[1] = 0;
    aVolts[0] = 0;
    aVolts[1] = 0;
    times[0] = 0;
    times[1] = 0 + delta;
}

// Voltage Calculations
double OutputVoltageModel::calculateVolt(double t, int i) {
	double Vo, a, b, c;
	
	a = (1 / (pow(delta, 2))) + (1/delta);
    c = (1 / (pow(delta, 2))) - (1/delta);
    b = (-2 / (pow(delta, 2))) + 5;
    
    Vo = (50 - (volts[i - 1] * b) - (volts[i - 2] * c)) / a;
	return Vo;
}

double OutputVoltageModel::calculateAVolt(double t) {
	double Va;
	
	Va = 10 - (exp(-1 * t) * ((10 * cos(2 * t)) + (5 * sin(2 * t))));
	return Va;	
}

void OutputVoltageModel::calculate() {
	// Sets Initial Array Values
	initializeValues();
    
    // This function
    double t, Vo, Va;
    t = 0;
    
    for (int i = 2; i < size; i++){
		// Calculates Numerical Voltage
        Vo = calculateVolt(t, i);
        
        // Increments time
        t += delta;
        times[i] = t;
        
        // Calculates Analytical Voltage
        Va = calculateAVolt(t);
        
        volts[i] = Vo;
        aVolts[i] = Va;
    }

}

void OutputVoltageModel::calculateMinMax() {
	min = volts[0];
	max = volts[0];
	minVTime = times[0];
	maxVTime = times[0];
	
	for (int i = 1; i < size; i++) {
		if (min > volts[i]) {
			min = volts[i];	
			minVTime = times[i];
		}
		
		if (max < volts[i]) {
			max = volts[i];
			maxVTime = times[i];
		}
	}
}

// OVM Printing Functions
void OutputVoltageModel::printTable() {
	// Table Title
	cout << setw(77) << "Table of Numerical and Analytical Voltages" << endl << endl;
	cout << setw(30) << "Time(s)" << setw(30) << "Numerical Voltage(Volts)" << setw(30) << "Analytical Voltage(Volts)" << endl << endl;
	
	// Initial Table Values
    cout << setw(30) << times[0] << setw(30) << volts[0] << setw(30) << aVolts[0] << endl;
    
    // Print Values
    for (int i = 2; i < size; i++) {
	    if (i % 100 == 0){  
	        cout << setw(30) << times[i] << setw(30) << volts[i] << setw(30) << aVolts[i] << endl; 
	    }
	}
}

void OutputVoltageModel::writeTable() {
	write.open(filename.c_str());
	// Table Title
	write << setw(77) << "Table of Numerical and Analytical Voltages" << endl << endl;
	write << setw(30) << "Time(s)" << setw(30) << "Numerical Voltage(Volts)" << setw(30) << "Analytical Voltage(Volts)" << endl << endl;
	
	// Initial Table Values
    write << setw(30) << times[0] << setw(30) << volts[0] << setw(30) << aVolts[0] << endl;
    
    // Print Values
    for (int i = 2; i < size; i++) {
	    if (i % 100 == 0){  
	        write << setw(30) << times[i] << setw(30) << volts[i] << setw(30) << aVolts[i] << endl; 
	    }
	}
	write.close();
}

// Graph Configuration Functions
void VoltGraph::configurePlot(){
	int ic;
	
	metafl("pdf");
	disini();
	pagera();
	complx();
	axspos(450,1800);
	axslen(2200,1200);
	name("Time t (s)","x");
	name("Voltage (Volts)","y");
	labdig(-1,"fx");
	ticks(9,"x");
	ticks(10,"y");
	titlin("Numerical and Analytical Voltages",1);
	ic=intrgb(0.95,0.95,0.95);
	axsbgd(ic);
	graf(0., 10., 0., 2., 0., 14., 0., 2.);
	setrgb(0.7,0.7,0.7);
	grid(1,1);
	color("fore");
	height(50);
	title();
}

void VoltGraph::configureNumericalPlot(){
	int ic;
	
	metafl("pdf");
	disini();
	pagera();
	complx();
	axspos(450,1800);
	axslen(2200,1200);
	name("Time t (s)","x");
	name("Voltage (Volts)","y");
	labdig(-1,"fx");
	ticks(9,"x");
	ticks(10,"y");
	titlin("Numerical Voltages",1);
	ic=intrgb(0.95,0.95,0.95);
	axsbgd(ic);
	graf(0., 10., 0., 2., 0., 14., 0., 2.);
	setrgb(0.7,0.7,0.7);
	grid(1,1);
	color("fore");
	height(50);
	title();
}

void VoltGraph::configureAnalyticalPlot(){
	int ic;
	
	metafl("pdf");
	disini();
	pagera();
	complx();
	axspos(450,1800);
	axslen(2200,1200);
	name("Time t (s)","x");
	name("Voltage (Volts)","y");
	labdig(-1,"fx");
	ticks(9,"x");
	ticks(10,"y");
	titlin("Analytical Voltages",1);
	ic=intrgb(0.95,0.95,0.95);
	axsbgd(ic);
	graf(0., 10., 0., 2., 0., 14., 0., 2.);
	setrgb(0.7,0.7,0.7);
	grid(1,1);
	color("fore");
	height(50);
	title();
}

// Graph Plot Functions
void VoltGraph::plotCurves(){
	char charBuffer[50];
    marker(21);
    incmrk(500);
    color("green");
    curve(floatTimes, floatAVolts, size); 
    color("red");
    curve(floatTimes, floatVolts, size); 
    legini(charBuffer,2,30); 
    leglin(charBuffer,"Analytical Voltages",1);
    leglin(charBuffer,"Numerical Voltages",2);
    legtit("Legend"); 
    legend(charBuffer,6);
  
    disfin();
}

void VoltGraph::plotNumericalCurves(){
    color("red");
    curve(floatTimes, floatVolts, size);  
  
    disfin();
}

void VoltGraph::plotAnalyticalCurves(){
    color("green");
    curve(floatTimes, floatAVolts, size); 
  
    disfin();
}

// Converts Double Array to Float
float* VoltGraph::toFloatArr(double arr[]) {
	float *floatArr;
	
	floatArr = new float[size];
	
	for (int i = 0; i < size; i++) {
		floatArr[i] = (float) arr[i];
	}	
	
	return floatArr;
}

void VoltGraph::setFloat(double times[], double volts[], double aVolts[]) {
	floatTimes = toFloatArr(times);
	floatVolts = toFloatArr(volts);
	floatAVolts = toFloatArr(aVolts);
}

// Mutator for Size
void VoltGraph::setSize(int size) {
	this->size = size;
}

// Generates User Interface
void userInterface() {
	int answer;
	bool loop, loop2, loop3;
	loop3 = true;
	
	OutputVoltageModel oVmodel;
	VoltGraph vGraph;
	
	do {
		loop = true;
		loop2 = true;
		loop3 = true;
		oVmodel.inputValues();
		oVmodel.calculate();
		oVmodel.calculateMinMax();
		vGraph.setSize(oVmodel.getSize());
		vGraph.setFloat(oVmodel.getTimes(), oVmodel.getVolts(), oVmodel.getAVolts());
		
		do {
			cout << "SELECT AN ACTION TO PERFORM" << endl;
			cout << "Maximum Voltage: " << oVmodel.getMax() << " V at " << oVmodel.getMaxVTime() << " seconds." << endl;
			cout << "Minimum Voltage: " << oVmodel.getMin() << " V at " << oVmodel.getMinVTime() << " seconds." << endl << endl;
			cout << "1. Print Table in Console" << endl;
			cout << "2. Print Table in Output File" << endl;
			cout << "3. Print Table in Console and File" << endl;
			cout << "4. Create Graph of Numerical Solution" << endl;
			cout << "5. Create Graph of Analytical Solution" << endl;
			cout << "6. Create Graph of both Numerical and Analytical Solutions" << endl;
			cout << "7. Enter New Values" << endl;
			cout << "8. End the Program" << endl << endl;
			
			do {
				cout << "Selection: ";
				cin >> answer;
				if (answer <= 0 || answer > 8) {
					cout << "Invalid Selection! (Please choose a number between 1-8)" << endl;	
				} else {
					loop2 = false;	
				}
			} while (loop2);
		
			switch(answer) {
			case 1:
				cout << endl;
				oVmodel.printTable();
				break;
			case 2:
				cout << "\nTable written to " << oVmodel.getFilename() << endl;
				oVmodel.writeTable();
				break;
			case 3:
				cout << endl;
				oVmodel.printTable();
				oVmodel.writeTable();
				cout << "\nTable written to " << oVmodel.getFilename() << endl;
				break;
			case 4:
				cout << "\nGraph Created" << endl;
				vGraph.configureNumericalPlot();
				vGraph.plotNumericalCurves();
				break;
			case 5:
				cout << "\nGraph Created" << endl;
				vGraph.configureAnalyticalPlot();
				vGraph.plotAnalyticalCurves();
				break;
			case 6:
				cout << "\nGraph Created" << endl;
				vGraph.configurePlot();
				vGraph.plotCurves();
				break;
			case 7:
				loop = false;
				break;
			default:
				cout << "Goodbye!";
				loop = false;
				loop3 = false;
			}
			
			cout << endl;
			
		} while(loop);
	} while(loop3);
}
