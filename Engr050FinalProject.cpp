#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

// OUTPUT VOLTAGE MODEL CLASS
class OutputVoltageModel {
	public:
		// Gets Data from User
		void getInputs();
		void setDelta();
		void setMaxTime();
		void setFilename();
		void setSize();
		
		// Calculates Numerical and Analytical Voltage Values
		void calculate();
		double calculateVolt(double t, int i);
		double calculateAVolt(double t);
		void initializeValues();
		
		// Print Out Values
		void printTable();
		
		// Encapsulating Members
		double* getVolts();
		double* getAVolts();
		double* getTimes();
		int getSize();
		
	private:
		double delta, maxTime, *volts, *aVolts, *times;
		int size;	
		string filename;
		ofstream write;
};

// VOLT GRAPH CLASS
class VoltGraph {
	public:
		void configurePlot();
		void plotCurves();
		void setFloat(double times[], double volts[], double aVolts[]);
		void setSize(int size);
		float* toFloatArr(double arr[]);
		
	private:
		int size;
		float *floatTimes, *floatAVolts, *floatVolts;	
};

// MAIN FUNCTION
int main() {
	OutputVoltageModel oVmodel;
	VoltGraph vGraph;
	
	oVmodel.getInputs();
	oVmodel.calculate();
	oVmodel.printTable();
	
//	vGraph.configurePlot();
//	vGraph.setSize(oVmodel.getSize());
//	vGraph.setFloat(oVmodel.getTimes(), oVmodel.getVolts(), oVmodel.getAVolts());
//	vGraph.plotCurves();
	
	return 0;
}

// Setters
void OutputVoltageModel::setDelta() {
	cout << "Enter your delta value: ";
	cin >> delta;
}

void OutputVoltageModel::setMaxTime() {
	cout << "Enter your max time: ";
	cin >> maxTime;
}

void OutputVoltageModel::setFilename() {
	cout << "Enter the filename of your output: ";
	cin >> filename;	
}

void OutputVoltageModel::setSize() {
	size = (int) (1 + (maxTime / delta));
}

// Getters
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

// Value Initializers
void OutputVoltageModel::getInputs() {
	setFilename();
	setDelta();
	setMaxTime();
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

// OUTPUT VOLTAGE MODEL FUNCTIONS
void OutputVoltageModel::calculate() {
	// Sets Initial Array Values
	initializeValues();
    
    // This function
    double t, Vo, Va;
    t = 0;
    
    for (int i = 2; i < size + 1; i++){
		// Calculates Numerical Voltage
        Vo = calculateVolt(t, i);
        
        // Increments time
        t += delta;
        times[i] = t;
        
        // Calculates Analytical Voltage
        Va = calculateAVolt(t);
        
        volts[i] = Vo;
        aVolts[i] = Va;
        cout << "Volt " << volts[i] << " ";
        cout << "aVolt " << aVolts[i] << endl;
    }
}

void OutputVoltageModel::printTable() {
	// Table Title
	cout << setw(20) << "Time" << setw(20) << "Numerical Voltage" << setw(20) << "Analytical Voltage" << endl << endl;
	
	// Initial Table Values
    cout << setw(20) << times[0] << setw(20) << volts[0] << setw(20) << "0" << endl;
    
    // Print Values
    for (int i = 2; i < size; i++) {
	    if (i < 101){  
	        cout << setw(20) << times[i] << setw(20) << volts[i] << setw(20) << aVolts[i] << endl; 
	    }
	}
}
