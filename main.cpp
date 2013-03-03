#include "include/main.hpp"
#include <boost/algorithm/string/predicate.hpp>
using namespace std;
using namespace pcp;
using namespace boost;
using namespace boost::program_options;
using namespace boost::algorithm;

int DEBUG_LEVEL = 2;

int main(int argc, char* argv[]) {
	string units, printFile, inputFile;
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
		("checkFinal,c", value<bool>()->default_value(true)->implicit_value(false)->zero_tokens() , "disable final check after VNS has finished")
		("checkIntermediate,m", value<bool>()->default_value(false)->implicit_value(true)->zero_tokens(), "enable check after each improvement/shake")
		("seed,r", value<int>(&rSeed)->default_value(time(NULL)), "set seed for random number generator")
		#ifdef ubigraph
		("delay", value<bool>()->default_value(false)->implicit_value(true)->zero_tokens(), "delay execution after loading the graph and finishing onestepCD")
		("full", value<bool>()->default_value(false)->implicit_value(true)->zero_tokens(), "render the full graph after the VNS terminates")
		#endif
	;
	
	options_description all("Allowed options");
	all.add(options).add(vnsOptions);
	
	variables_map vm;
	try {
		store(parse_command_line(argc, argv, all), vm);
	}
	catch(const boost::program_options::error& ex) {
		cerr << ex.what() << endl;
		return -1;
	}
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

	Solution* fullG;

	if (isdigit(cin.peek())) {
		string buffer = "";
		getline(cin, buffer);
		cin.seekg(0);
		
		if (buffer.find(' ') == string::npos) {
			if (DEBUG_LEVEL > 2)
				cout << "Detected .col.b input." << endl;
			fullG = Solution::fromColBStream(cin);
		}
		else {
			if (DEBUG_LEVEL > 2)
				cout << "Detected .pcp input." << endl;
			fullG = Solution::fromPcpStream(cin);
		}
	}
	else {
		if (DEBUG_LEVEL > 2)
			cout << "Detected .col input." << endl;
	
		fullG = Solution::fromColStream(cin);
	}
	
	if (fullG == NULL) {
		cerr << "Failed to parse input!" << endl;
		return -1;
	}

	#ifdef ubigraph
	if (vm.count("delay"))
		usleep(3000000);
	#endif

	if (DEBUG_LEVEL > 3) {
		cout << "Constructing initial solution ..." << endl;
	}
	
	Solution *onestep = onestepCD(*fullG);
	
	if (DEBUG_LEVEL > 2)
		cout << "Initial solution uses " << onestep->colorsUsed << " colors." << endl;

	#ifdef ubigraph
	Solution *onecopy = new Solution(onestep);

	if (vm.count("delay"))
		usleep(3000000);
	#endif
	
	Solution *best = vnsRun(*onestep, *fullG, units, unsuccessfulShake, shakeStart, shakeIncrement, maxTime, vm.count("checkIntermediate"), vm.count("checkFinal"));
	
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
	
	#ifdef ubigraph
	int offset = num_vertices(*fullG->g);
	onecopy->redraw(offset);
	if (vm.count("full"))
		fullG->redraw(offset * 2);
	delete onecopy;
	#endif

	delete best;
	delete fullG;

	return 0;
}
