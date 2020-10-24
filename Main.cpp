// TODO: Clean code lol.
#include <iostream>
#include <stdlib.h>
#include <chrono> 
#include <fstream>
#include <vector>
#include <cstring>

// if on windows include windows header for Console handle for colors
// TODO: Use Colors.
#ifdef _WIN32
	#include <Windows.h>
#endif

std::vector<int> durations;

using namespace std::chrono;
using namespace std;

// get the average of all the elements in a vector, tbh idk how it works
double compute_average(std::vector<int>& vi) {

	double sum = 0;
	for (int p : vi) {
		sum = sum + p;
	}

	return (sum / vi.size());
}

int main(int argc, char* argv[]) {
	// if the user is on anything but windows tell them it might not work (which it doesnt)
	// TODO: 
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#elif defined ___linux___
	cout << "You are running linux. Some things may be broken. as this is a native app to windows.\n";
#else
	cout << "You are not running windows. Some things may be broken. as this is a native app to windows.\n";
#endif
	int count = 0;
	int lastdur = 0;
	const char* output = "";

	// if there are too few arguments then print usage message
	// TODO: Make it actually tell if you didnt do something good with the arguments.
	if (argc < 2) {
		cout << "Incorrect usage. \n-o [filename]\tWrite output to file\n-c [times]\tThe amount of times to run the program.\n\nExample: runtime -o Output.txt -c 100 funny.exe [this runs funny.exe 100 times and writes the output to Output.txt and the console.]";
		return 0;
	}
	// loop through the arguments
	for (int i = 0; i < argc + 1; i++)
	{
		// if the argument is -o set output to next argument
		if (strcmp(argv[i], "-o") == 0) {
			output = argv[i + 1];
		}
		// if the argument is -c set the count to next argument
		if (strcmp(argv[i], "-c") == 0) {
			count = atoi(argv[i + 1]);
		}
	}
	// if the count is less than or equal to 0 set it to 100
	if (count <= 0) {
		cout << "Count was not defined. Defaulting to 100.\n";
		count = 100;
	}
	// if the ouput is set to nothing, set it to logs.txt
	if (output == "") {
		cout << "Output was not defined. Defaulting to logs.txt.\n";
		output = "logs.txt";
	}

	// open the output file and write title
	ofstream outputStream;
	outputStream.open(output);
	outputStream << "--==Begin ProcRuntime Logs==--\n";


	// set the file to the last argument
	const char* file = argv[sizeof(argv) + 1];
	cout << "Running " << file << " " << count << " times.\n";
	// loop however many times count is
	for (int i = 0; i <= count; ++i) {
		// set start to the time now
		auto start = high_resolution_clock::now();
	
		//execute file 
		// TODO: Get a better solution to system(); because just the call to system probably takes alot of time
		system(file);
		
		//set stop time to the current time
		auto stop = high_resolution_clock::now();

		// set the duration to the difference of start and stop
		auto duration = duration_cast<microseconds>(stop - start);
		// if the last time equals the current
		if (lastdur == duration.count()) {
			cout << "[/] Execution " << i << " took " << duration.count() << " Microseconds. Same as last run.\n";
			outputStream << "[/] Execution " << i << " took " << duration.count() << " Microseconds. Same as last run.\n";
		}
		// if the last time is less than current
		else if (lastdur < duration.count()) {
			cout << "[-] Execution " << i << " took " << duration.count() << " Microseconds. " << duration.count() - lastdur << " Microseconds slower from last run.\n";
			outputStream << "[-] Execution " << i << " took " << duration.count() << " Microseconds. " << duration.count() - lastdur << " Microseconds slower from last run.\n";
		}
		// if the last time is greater than current
		else if (lastdur > duration.count()) {
			cout << "[+] Execution " << i << " took " << duration.count() << " Microseconds. " << lastdur - duration.count() << " Microseconds faster than last run. \n";
			outputStream << "[+] Execution " << i << " took " << duration.count() << " Microseconds. " << lastdur - duration.count() << " Microseconds faster than last run. \n";
		}
		// anything else
		else {
			cout << "[?] Execution " << i << " took " << duration.count() << " Microseconds.\n";
			outputStream << "[?] Execution " << i << " took " << duration.count() << " Microseconds.\n";
		}
		// push the time to the durations vector and set the last duration to the current
		durations.push_back(duration.count());
		lastdur = duration.count();
	}

	// get average of the durations
	float f = compute_average(durations);

	cout << "Calculating average time...";
	cout << "\rAverage runtime of " << file << " is " << f << " Microseconds (" << f / 1000.f << " MS).\n";
	outputStream << "Average runtime of " << file << " is " << f << " Microseconds (" << f / 1000.f << " MS).\n";
	
	// write footer, close file, and exit program.
	outputStream << "--==End ProcRuntime Logs==--\n";
	outputStream.close();
	return 0;
}