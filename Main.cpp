#include <cstdlib>
#include <chrono> 
#include <fstream>
#include <vector>
#include <cstring>

#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/color.h>

std::vector<int> durations;

// get the average of all the elements in a vector, tbh idk how it works
double compute_average(std::vector<int>& vi) {
	double sum = 0;
	for (int p : vi) {
		sum = sum + p;
	}

	return (sum / vi.size());
}

int main(int argc, char* argv[]) {
	// if the user is on anything but windows tell them it might not work
	#ifdef _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	#endif
	int count = 0;
	int lastdur = 0;
	const char* output = "";

	// if there are too few arguments then print usage message
	if (argc < 2) {
		fmt::print("Incorrect usage: \n\t-o [filename]\tWrite output to file\n\t-c [times]\tThe amount of times to run the program.\n\nExample: runtime -o output.txt -c 100 test.exe (this runs test.exe 100 times and writes the output to both output.txt and stdout.)\n");
		return 0;
	} 

	// loop through the arguments
	for (int i = 0; i < argc; ++i)
	{
		// if the argument is -o set output to next argument
		if (strcmp(argv[i], "-o") == 0) {
			output = argv[i + 1];
		}
		// checks if -c is supplied an integer
		if (strcmp(argv[i], "-c") == 0) {
			char *temp;
			strtol(argv[i + 1], &temp, 0);
			if (*temp != '\0'){
        		fmt::print(fmt::emphasis::bold | fg(fmt::color::red), "Incorrect usage:\n\tCount should only be an integer.\n");
				return -1;
			} else {
				// set the count to next argument after the -c
				count = atoi(argv[i + 1]);
			}
		}
	}

	// if the count is less than or equal to 0 set it to 100
	if (count <= 0) {
		fmt::print("Count was not defined. Defaulting to 100.\n");
		count = 100;
	} else {
		fmt::print("Looping {} times.\n", count);
	}
	// if the ouput is set to nothing, set it to logs.txt
	if (strcmp(output, "") == 0) {
		fmt::print("Output was not defined. Defaulting to logs.txt.\n");
		output = "logs.txt";
	} else {
		fmt::print("Output is {}.\n", output);
	}

	// open the output file and write title
	std::ofstream outputStream;
	outputStream.open(output);
	outputStream << "--==Begin ProcRuntime Logs==--\n";

	char file[100];
	// set the file to the last argument
    #ifdef _WIN32
		strcpy(file, argv[argc - 1]);
    #else
        if (argv[argc - 1][0] == '.' || argv[argc - 1][0] == '/'){
            strcpy(file, argv[argc - 1]);
        } else {
            strcpy(file, "./");
            strcat(file, argv[argc - 1]);
        }
    #endif
	//TODO: check if file is accesible, and use ask for super user/administrator in that case.
	std::ifstream ifile(file);
	if (!(bool)ifile){
        fmt::print(stderr, fmt::emphasis::bold | fg(fmt::color::red), "Incorrect usage:\n\tWas provided with an invalid or unreadable file. Check the file path to make sure it is correct.\n");
		return -1;
	}
	
	fmt::print("Running {} {} time(s).\n", file, count);
	// loop however many times count is
	for (int i = 1; i <= count; ++i) {
		// set start to the time now
		auto start = std::chrono::high_resolution_clock::now();
		
		//execute file 
		system(file);

		//set stop time to the current time
		auto stop = std::chrono::high_resolution_clock::now();

		// set the duration to the difference of start and stop
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		// if the last time equals the current
		if (lastdur == duration.count()) {
			fmt::print(fg(fmt::color::yellow), "[/] Execution {} took {} microseconds. Same as last run.\n", i, duration.count());
			// Using old writing for files, becuase ofstream can't be easily converted to a FILE*
			outputStream << "[/] Execution " << i << " took " << duration.count() << " microseconds. Same as last run.\n";
		}
		// if the last time is less than current
		else if (lastdur < duration.count()) {
			fmt::print(fg(fmt::color::red), "[/] Execution {} took {} microseconds. {} microseconds slower than last run.\n", i, duration.count(), duration.count() - lastdur);
			outputStream << "[-] Execution " << i << " took " << duration.count() << " Microseconds. " << duration.count() - lastdur << " microseconds slower than last run.\n";
		}
		// if the last time is greater than current
		else if (lastdur > duration.count()) {
			fmt::print(fg(fmt::color::green), "[/] Execution {} took {} microseconds. {} microseconds faster than last run.\n", i, duration.count(), lastdur - duration.count());
			outputStream << "[+] Execution " << i << " took " << duration.count() << " Microseconds. " << lastdur - duration.count() << " Microseconds faster than last run. \n";
		}
		// anything else
		else {
			fmt::print(fg(fmt::color::yellow), "[/] Execution {} took {} microseconds.\n", i, duration.count());
			outputStream << "[?] Execution " << i << " took " << duration.count() << " Microseconds.\n";
		}
		// push the time to the durations vector and set the last duration to the current
		durations.push_back(duration.count());
		lastdur = duration.count();
	}

	// get average of the durations
	float f = compute_average(durations);

	fmt::print("Calculating average time...\n");
	fmt::print("\rAverage runtime of {} is {} Microseconds ({} ms).\n", file, f, f / 1000.0f);
	
	// write footer, close file, and exit program.
	outputStream << "--==End ProcRuntime Logs==--\n";
	outputStream.close();
	return 0;
}
