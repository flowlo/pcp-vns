#include "header/main.hpp"

using namespace std;
using namespace pcp;
using namespace boost;
using namespace boost::program_options;

int DEBUG_LEVEL = 2;

int main(int argc, char* argv[]) {
	string units, printFile;
	int unsuccessfulShake, shakeStart, shakeIncrement, maxTime, rSeed;
	
	options_description options("General options");
	options.add_options()
		("help,h", "produce help message")
		("print,p", value<string>(&printFile), "set print file (*.gv)")
		("debug,d", value<int>(&DEBUG_LEVEL)->default_value(0), "set debug level")
	;
	
	options_description vnsOptions("Variable Neighborhood Search options");
	vnsOptions.add_options()
		("units,n", value<string>(&units)->default_value("nc"), "set units")
		("shakeStart,s", value<int>(&shakeStart)->default_value(0), "set shake start")
		("shakeIncrement,i", value<int>(&shakeIncrement)->default_value(10), "set shake increment")
		("unsuccessfulShake,u", value<int>(&unsuccessfulShake)->default_value(10), "set unsuccessful shake threshold")
		("maxTime,t", value<int>(&maxTime)->default_value(10), "set VNS running time (seconds)")
		("checkFinal,c", "disable final check after VNS has finished")
		("checkIntermediate,m", "enable check after each improvement/shake")
		("seed,r", value<int>(&rSeed)->default_value(time(NULL)), "set seed for random number generator")
	;
	
	options_description all("Allowed options");
	all.add(options).add(vnsOptions);
	
	variables_map vm;
	store(parse_command_line(argc, argv, all), vm);
	notify(vm);
	
	if (vm.count("help")) {
		cout << all << endl;
		return 0;
	}
	
	srand(rSeed);
	
	if (DEBUG_LEVEL > 4) {
		DEBUG_LEVEL = 4;
		
		cout << "The specified debug level (-d) exceeds"
		<< " the maximum. It was set to " << DEBUG_LEVEL
		<< "." << endl;
	}
	else if (DEBUG_LEVEL < 0) {
		DEBUG_LEVEL = 0;
	}
	
	Solution fullG = Solution::fromPcpStream(cin);
	
/*	if (fullG == NULL) {
		cerr << "Error reading from stdin!" << endl;
		return -1;
	} */

	if (DEBUG_LEVEL > 3) {
		cout<<"Begin Onestep"<<endl;
	}
	
	Solution *onestep = onestepCD(fullG);
	
	if (DEBUG_LEVEL > 2)
		cout << "Onestep solution uses " << onestep->colorsUsed << " colors." << endl;
	
	Solution *best = vnsRun(*onestep, fullG, units, unsuccessfulShake, shakeStart, shakeIncrement, maxTime, vm.count("checkIntermediate"), vm.count("checkFinal"));
	
	if (best == NULL)
		return -1;
	
	if (vm.count("print")) {
		ofstream out(vm["print"].as<string>());
		
		if (out.fail()) {
			cerr << "Error when trying to access file: " << vm["print"].as<string>() << endl;
			return -1;
		}
		best->print(out);
		out.flush();
		out.close();
		
		if (DEBUG_LEVEL > 1)
			cout << "Printing to '" << vm["print"].as<string>() << "' done!" << endl;
	}

	return 0;
}
