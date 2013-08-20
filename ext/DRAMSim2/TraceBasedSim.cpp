/*********************************************************************************
*  Copyright (c) 2010-2011, Elliott Cooper-Balis
*                             Paul Rosenfeld
*                             Bruce Jacob
*                             University of Maryland 
*                             dramninjas [at] gmail [dot] com
*  All rights reserved.
*  
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*  
*     * Redistributions of source code must retain the above copyright notice,
*        this list of conditions and the following disclaimer.
*  
*     * Redistributions in binary form must reproduce the above copyright notice,
*        this list of conditions and the following disclaimer in the documentation
*        and/or other materials provided with the distribution.
*  
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*********************************************************************************/



//TraceBasedSim.cpp
//
//File to run a trace-based simulation
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include <map>
#include <list>

#include "SystemConfiguration.h"
#include "MemorySystem.h"
#include "MultiChannelMemorySystem.h"
#include "Transaction.h"
#include "IniReader.h"


using namespace DRAMSim;
using namespace std;

#define RETURN_TRANSACTIONS 1

// Flag to indicate whether previous request has returned
bool flag = 1;
bool flag1 = 1;

#ifndef _SIM_
int SHOW_SIM_OUTPUT = 1;
ofstream visDataOut; //mostly used in MemoryController

#ifdef RETURN_TRANSACTIONS
class TransactionReceiver
{
	private: 
		map<uint64_t, list<uint64_t> > pendingReadRequests; 
		map<uint64_t, list<uint64_t> > pendingWriteRequests; 

	public: 
		void add_pending(const Transaction &t, uint64_t cycle)
		{
			// C++ lists are ordered, so the list will always push to the back and
			// remove at the front to ensure ordering
			if (t.transactionType == DATA_READ)
			{
				pendingReadRequests[t.address].push_back(cycle); 
			}
			else if (t.transactionType == DATA_WRITE)
			{
				pendingWriteRequests[t.address].push_back(cycle); 
			}
			else
			{
				ERROR("This should never happen"); 
				exit(-1);
			}
		}

		void read_complete(unsigned id, uint64_t address, uint64_t done_cycle, uint64_t threadID)
		{
			map<uint64_t, list<uint64_t> >::iterator it;
			it = pendingReadRequests.find(address); 
			if (it == pendingReadRequests.end())
			{
				ERROR("Cant find a pending read for this one"); 
				exit(-1);
			}
			else
			{
				if (it->second.size() == 0)
				{
					ERROR("Nothing here, either"); 
					exit(-1); 
				}
			}

			uint64_t added_cycle = pendingReadRequests[address].front();
			uint64_t latency = done_cycle - added_cycle;
			
			if (threadID == 0)
			{
				flag = 1;
			}
			else if (threadID == 1)
			{
				flag1 = 1;
			}
			else
			{
				ERROR("Wrong threadID found");
				exit(0);
			}

			pendingReadRequests[address].pop_front();
			cout << "Read Callback:  0x"<< std::hex << address << std::dec << " latency="<<latency<<"cycles ("<< done_cycle<< "->"<<added_cycle<<") "<<threadID<<endl;
		}
		void write_complete(unsigned id, uint64_t address, uint64_t done_cycle, uint64_t threadID)
		{
			map<uint64_t, list<uint64_t> >::iterator it;
			it = pendingWriteRequests.find(address); 
			if (it == pendingWriteRequests.end())
			{
				ERROR("Cant find a pending read for this one"); 
				exit(-1);
			}
			else
			{
				if (it->second.size() == 0)
				{
					ERROR("Nothing here, either"); 
					exit(-1); 
				}
			}
			
			if (threadID == 0)
			{
				flag = 1;
			}
			else if (threadID == 1)
			{
				flag1 = 1;
			}
			else
			{
				ERROR("Wrong threadID found");
				exit(0);
			}

			uint64_t added_cycle = pendingWriteRequests[address].front();
			uint64_t latency = done_cycle - added_cycle;

			pendingWriteRequests[address].pop_front();
			cout << "Write Callback: 0x"<< std::hex << address << std::dec << " latency="<<latency<<"cycles ("<< done_cycle<< "->"<<added_cycle<<") "<<threadID<<endl;
		}
};
#endif

void usage()
{
	cout << "DRAMSim2 Usage: " << endl;
	cout << "DRAMSim -t tracefile -s system.ini -d ini/device.ini [-c #] [-p pwd] [-q] [-S 2048] [-n] [-o OPTION_A=1234,tRC=14,tFAW=19]" <<endl;
	cout << "\t-t, --tracefile=FILENAME \tspecify a tracefile to run  "<<endl;
	cout << "\t-s, --systemini=FILENAME \tspecify an ini file that describes the memory system parameters  "<<endl;
	cout << "\t-d, --deviceini=FILENAME \tspecify an ini file that describes the device-level parameters"<<endl;
	cout << "\t-c, --numcycles=# \t\tspecify number of cycles to run the simulation for [default=30] "<<endl;
	cout << "\t-q, --quiet \t\t\tflag to suppress simulation output (except final stats) [default=no]"<<endl;
	cout << "\t-o, --option=OPTION_A=234,tFAW=14\t\t\toverwrite any ini file option from the command line"<<endl;
	cout << "\t-p, --pwd=DIRECTORY\t\tSet the working directory (i.e. usually DRAMSim directory where ini/ and results/ are)"<<endl;
	cout << "\t-S, --size=# \t\t\tSize of the memory system in megabytes [default=2048M]"<<endl;
	cout << "\t-n, --notiming \t\t\tDo not use the clock cycle information in the trace file"<<endl;
	cout << "\t-v, --visfile \t\t\tVis output filename"<<endl;
    cout << "\t-r, --rseult \t\t\tResult output filename"<<endl;
}
#endif

void *parseTraceFileLine(string &line, uint64_t &addr, enum TransactionType &transType, uint64_t &clockCycle, uint64_t &pid, TraceType type, bool useClockCycle)
{
	size_t previousIndex=0;
	size_t spaceIndex=0;
	uint64_t *dataBuffer = NULL;
	string addressStr="", cmdStr="", dataStr="", ccStr="", pidStr="";

	switch (type)
	{
	case k6:
	{
		spaceIndex = line.find_first_of(" ", 0);

		addressStr = line.substr(0, spaceIndex);
		previousIndex = spaceIndex;

		spaceIndex = line.find_first_not_of(" ", previousIndex);
		cmdStr = line.substr(spaceIndex, line.find_first_of(" ", spaceIndex) - spaceIndex);
		previousIndex = line.find_first_of(" ", spaceIndex);

		spaceIndex = line.find_first_not_of(" ", previousIndex);
		ccStr = line.substr(spaceIndex, line.find_first_of(" ", spaceIndex) - spaceIndex);

		if (cmdStr.compare("P_MEM_WR")==0 ||
		        cmdStr.compare("BOFF")==0)
		{
			transType = DATA_WRITE;
		}
		else if (cmdStr.compare("P_FETCH")==0 ||
		         cmdStr.compare("P_MEM_RD")==0 ||
		         cmdStr.compare("P_LOCK_RD")==0 ||
		         cmdStr.compare("P_LOCK_WR")==0)
		{
			transType = DATA_READ;
		}
		else
		{
			ERROR("== Unknown Command : "<<cmdStr);
			exit(0);
		}

		istringstream a(addressStr.substr(2));//gets rid of 0x
		a>>hex>>addr;

		//if this is set to false, clockCycle will remain at 0, and every line read from the trace
		//  will be allowed to be issued
		if (useClockCycle)
		{
			istringstream b(ccStr);
			b>>clockCycle;
		}
		break;
	}
	case mase:
	{
		spaceIndex = line.find_first_of(" ", 0);

		addressStr = line.substr(0, spaceIndex);
		previousIndex = spaceIndex;

		spaceIndex = line.find_first_not_of(" ", previousIndex);
		cmdStr = line.substr(spaceIndex, line.find_first_of(" ", spaceIndex) - spaceIndex);
		previousIndex = line.find_first_of(" ", spaceIndex);

		spaceIndex = line.find_first_not_of(" ", previousIndex);
		ccStr = line.substr(spaceIndex, line.find_first_of(" ", spaceIndex) - spaceIndex);
		previousIndex = line.find_first_of(" ", spaceIndex);
		
		// Thread id
		spaceIndex = line.find_first_not_of(" ", previousIndex);
		pidStr = line.substr(spaceIndex, line.find_first_of(" ", spaceIndex) - spaceIndex);

		if (cmdStr.compare("IFETCH")==0||
		        cmdStr.compare("READ")==0)
		{
			transType = DATA_READ;
		}
		else if (cmdStr.compare("WRITE")==0)
		{
			transType = DATA_WRITE;
		}
		else
		{
			ERROR("== Unknown command in tracefile : "<<cmdStr);
		}

		istringstream a(addressStr.substr(2));//gets rid of 0x
		a>>hex>>addr;
		
		istringstream p(pidStr);
		p>>pid;

		//if this is set to false, clockCycle will remain at 0, and every line read from the trace
		//  will be allowed to be issued
		if (useClockCycle)
		{
			istringstream b(ccStr);
			b>>clockCycle;
		}

		break;
	}
	case spec:
	{
		spaceIndex = line.find_first_of(" ", 0);

		addressStr = line.substr(0, spaceIndex);
		previousIndex = spaceIndex;

		spaceIndex = line.find_first_not_of(" ", previousIndex);
		cmdStr = line.substr(spaceIndex, line.find_first_of(" ", spaceIndex) - spaceIndex);
		previousIndex = line.find_first_of(" ", spaceIndex);

		spaceIndex = line.find_first_not_of(" ", previousIndex);
		ccStr = line.substr(spaceIndex, line.find_first_of(" ", spaceIndex) - spaceIndex);
		previousIndex = line.find_first_of(" ", spaceIndex);

		if (cmdStr.compare("DATA_READ")==0)
		{
			transType = DATA_READ;
		}
		else if (cmdStr.compare("DATA_WRITE")==0)
		{
			transType = DATA_WRITE;
		}
		else
		{
			ERROR("== Unknown command in tracefile : "<<cmdStr);
		}

		istringstream a(addressStr.substr(2));//gets rid of 0x
		a>>hex>>addr;

		//if this is set to false, clockCycle will remain at 0, and every line read from the trace
		//  will be allowed to be issued
		if (useClockCycle)
		{
			istringstream b(ccStr);
			b>>clockCycle;
		}

		break;
	}
	case misc:
		spaceIndex = line.find_first_of(" ", spaceIndex+1);
		if (spaceIndex == string::npos)
		{
			ERROR("Malformed line: '"<< line <<"'");
		}

		addressStr = line.substr(previousIndex,spaceIndex);
		previousIndex=spaceIndex;

		spaceIndex = line.find_first_of(" ", spaceIndex+1);
		if (spaceIndex == string::npos)
		{
			cmdStr = line.substr(previousIndex+1);
		}
		else
		{
			cmdStr = line.substr(previousIndex+1,spaceIndex-previousIndex-1);
			dataStr = line.substr(spaceIndex+1);
		}

		//convert address string -> number
		istringstream b(addressStr.substr(2)); //substr(2) chops off 0x characters
		b >>hex>> addr;

		// parse command
		if (cmdStr.compare("read") == 0)
		{
			transType=DATA_READ;
		}
		else if (cmdStr.compare("write") == 0)
		{
			transType=DATA_WRITE;
		}
		else
		{
			ERROR("INVALID COMMAND '"<<cmdStr<<"'");
			exit(-1);
		}
		if (SHOW_SIM_OUTPUT)
		{
			DEBUGN("ADDR='"<<hex<<addr<<dec<<"',CMD='"<<transType<<"'");//',DATA='"<<dataBuffer[0]<<"'");
		}

		//parse data
		//if we are running in a no storage mode, don't allocate space, just return NULL
#ifndef NO_STORAGE
		if (dataStr.size() > 0 && transType == DATA_WRITE)
		{
			// 32 bytes of data per transaction
			dataBuffer = (uint64_t *)calloc(sizeof(uint64_t),4);
			size_t strlen = dataStr.size();
			for (int i=0; i < 4; i++)
			{
				size_t startIndex = i*16;
				if (startIndex > strlen)
				{
					break;
				}
				size_t charsLeft = min(((size_t)16), strlen - startIndex + 1);
				string piece = dataStr.substr(i*16,charsLeft);
				istringstream iss(piece);
				iss >> hex >> dataBuffer[i];
			}
			PRINTN("\tDATA=");
			BusPacket::printData(dataBuffer);
		}

		PRINT("");
#endif
		break;
	}
	return dataBuffer;
}

#ifndef _SIM_

void alignTransactionAddress(Transaction &trans)
{
	// zero out the low order bits which correspond to the size of a transaction

	unsigned throwAwayBits = dramsim_log2((BL*JEDEC_DATA_BUS_BITS/8));

	trans.address >>= throwAwayBits;
	trans.address <<= throwAwayBits;
}

/** 
 * Override options can be specified on the command line as -o key1=value1,key2=value2
 * this method should parse the key-value pairs and put them into a map 
 **/ 
IniReader::OverrideMap *parseParamOverrides(const string &kv_str)
{
	IniReader::OverrideMap *kv_map = new IniReader::OverrideMap(); 
	size_t start = 0, comma=0, equal_sign=0;
	// split the commas if they are there
	while (1)
	{
		equal_sign = kv_str.find('=', start); 
		if (equal_sign == string::npos)
		{
			break;
		}

		comma = kv_str.find(',', equal_sign);
		if (comma == string::npos)
		{
			comma = kv_str.length();
		}

		string key = kv_str.substr(start, equal_sign-start);
		string value = kv_str.substr(equal_sign+1, comma-equal_sign-1); 

		(*kv_map)[key] = value; 
		start = comma+1;

	}
	return kv_map; 
}

int main(int argc, char **argv)
{
	int c;
	TraceType traceType;
	string traceFileName;
	string traceFileName1;
	string systemIniFilename("system.ini");
    unsigned tpTurnLength=12;
    int num_pids=2;
	string deviceIniFilename;
	string pwdString;
	string *visFilename = NULL;
	string outputFilename;
	unsigned megsOfMemory=2048;
	bool useClockCycle=true;
	
	IniReader::OverrideMap *paramOverrides = NULL; 

	unsigned numCycles=1000;
	//getopt stuff
	while (1)
	{
		static struct option long_options[] =
		{
			{"deviceini", required_argument, 0, 'd'},
			{"tracefile", required_argument, 0, 't'},
			{"tracefile1", required_argument, 0, 'T'},
			{"systemini", required_argument, 0, 's'},
            {"tpturnlength", required_argument, 0, 'l'},
            {"numpids", required_argument, 0, 'P'},

			{"pwd", required_argument, 0, 'p'},
			{"numcycles",  required_argument,	0, 'c'},
			{"option",  required_argument,	0, 'o'},
			{"quiet",  no_argument, &SHOW_SIM_OUTPUT, 'q'},
			{"help", no_argument, 0, 'h'},
			{"size", required_argument, 0, 'S'},
			{"visfile", required_argument, 0, 'v'},
			{"result", required_argument, 0, 'r'},
			{0, 0, 0, 0}
		};
		int option_index=0; //for getopt
		c = getopt_long (argc, argv, "t:T:s:l:P:r:c:d:o:p:S:v:qn", long_options, &option_index);
		if (c == -1)
		{
			break;
		}
		switch (c)
		{
		case 0: //TODO: figure out what the hell this does, cuz it never seems to get called
			if (long_options[option_index].flag != 0) //do nothing on a flag
			{
				printf("setting flag\n");
				break;
			}
			printf("option %s",long_options[option_index].name);
			if (optarg)
			{
				printf(" with arg %s", optarg);
			}
			printf("\n");
			break;
		case 'h':
			usage();
			exit(0);
			break;
		case 't':
			traceFileName = string(optarg);
			break;
		case 'T':
			traceFileName1 = string(optarg);
			break;
		case 's':
			systemIniFilename = string(optarg);
			break;
        case 'l':
            tpTurnLength = atoi(optarg);
            break;
        case 'P':
            num_pids = atoi(optarg);
            break;
		case 'd':
			deviceIniFilename = string(optarg);
			break;
		case 'c':
			numCycles = atoi(optarg);
			break;
		case 'S':
			megsOfMemory=atoi(optarg);
			break;
		case 'p':
			pwdString = string(optarg);
			break;
		case 'q':
			SHOW_SIM_OUTPUT=false;
			break;
		case 'n':
			useClockCycle=false;
			break;
		case 'o':
			paramOverrides = parseParamOverrides(string(optarg)); 
			break;
		case 'v':
			visFilename = new string(optarg);
			break;
		case 'r':
			outputFilename = string(optarg);
			break;
		case '?':
			usage();
			exit(-1);
			break;
		}
	}

	// get the trace filename
	string temp = traceFileName.substr(traceFileName.find_last_of("/")+1);

	//get the prefix of the trace name
	temp = temp.substr(0,temp.find_first_of("_"));
	if (temp=="mase")
	{
		traceType = mase;
	}
	else if (temp=="k6")
	{
		traceType = k6;
	}
	else if (temp=="misc")
	{
		traceType = misc;
	}
	else
	{
		traceType = spec;
	}


	// no default value for the default model name
	if (deviceIniFilename.length() == 0)
	{
		ERROR("Please provide a device ini file");
		usage();
		exit(-1);
	}


	//ignore the pwd argument if the argument is an absolute path
	if (pwdString.length() > 0 && traceFileName[0] != '/')
	{
		traceFileName = pwdString + "/" +traceFileName;
	}
	
	if (pwdString.length() > 0 && traceFileName1[0] != '/')
	{
		traceFileName1 = pwdString + "/" +traceFileName1;
	}

	DEBUG("== Loading trace file '"<<traceFileName<<"' == ");
	DEBUG("== Loading trace file '"<<traceFileName1<<"' == ");

	ifstream traceFile;
	ifstream traceFile1;
	string line;


	MultiChannelMemorySystem *memorySystem = new MultiChannelMemorySystem(
            deviceIniFilename, systemIniFilename, tpTurnLength, 0, 
            pwdString, traceFileName, 
            megsOfMemory, outputFilename, 
            visFilename, paramOverrides,
            num_pids);
	// set the frequency ratio to 1:1
	memorySystem->setCPUClockSpeed(0); 
    //std::ostream &dramsim_logfile = memorySystem->getLogFile(); 
	// don't need this anymore 
	delete paramOverrides;


#ifdef RETURN_TRANSACTIONS
	TransactionReceiver transactionReceiver; 
	/* create and register our callback functions */
	Callback_t *read_cb = new Callback<TransactionReceiver, void, unsigned, uint64_t, uint64_t, uint64_t>(&transactionReceiver, &TransactionReceiver::read_complete);
	Callback_t *write_cb = new Callback<TransactionReceiver, void, unsigned, uint64_t, uint64_t, uint64_t>(&transactionReceiver, &TransactionReceiver::write_complete);
	memorySystem->RegisterCallbacks(read_cb, write_cb, NULL);
#endif


	uint64_t addr;
	uint64_t clockCycle=0;
	uint64_t clockCycle1=0;
	uint64_t pid;
	enum TransactionType transType;

	void *data = NULL;
	int lineNumber = 0;
	int lineNumber1 = 0;
	Transaction *trans=NULL;
	Transaction *trans1=NULL;
	bool pendingTrans = false;
	bool pendingTrans1 = false;

	traceFile.open(traceFileName.c_str());

	if (!traceFile.is_open())
	{
		cout << "== Error - Could not open trace file"<<endl;
		exit(0);
	}

    // This should not be done at runtime, but instead by a script after the run.
    // In general, any input/output files should be chosen at the command line or
    // at least declared in the constructor.	
    // ofstream inputFile;
	// inputFile.open("t0trace");
	
	// For normal traces with absolute time
	if (traceType != spec) {
		for (size_t i=0;i<numCycles;i++)
		{
			if (!pendingTrans)
			{
				if (!traceFile.eof())
				{
					getline(traceFile, line);

					if (line.size() > 0)
					{
						data = parseTraceFileLine(line, addr, transType,clockCycle, pid, traceType,useClockCycle);
						// if (pid == 0 && transType == DATA_READ) 
						//    inputFile << "Address: " << hex << addr << " Arrive time: " << dec << clockCycle << '\n';
						trans = new Transaction(transType, addr, data, pid, 0);
						alignTransactionAddress(*trans); 

						if (i>=clockCycle)
						{
							if (!(*memorySystem).addTransaction(trans))
							{
								pendingTrans = true;
							}
							else
							{
	#ifdef RETURN_TRANSACTIONS
								transactionReceiver.add_pending(*trans, i); 
	#endif
								// the memory system accepted our request so now it takes ownership of it
								trans = NULL; 
							}
						}
						else
						{
							pendingTrans = true;
						}
					}
					else
					{
						DEBUG("WARNING: Skipping line "<<lineNumber<< " ('" << line << "') in tracefile");
					}
					lineNumber++;
				}
				else
				{
					//we're out of trace, set pending=false and let the thing spin without adding transactions
					pendingTrans = false; 
				}
			}

			else if (pendingTrans && i >= clockCycle)
			{
				pendingTrans = !(*memorySystem).addTransaction(trans);
				if (!pendingTrans)
				{
	#ifdef RETURN_TRANSACTIONS
					transactionReceiver.add_pending(*trans, i); 
	#endif
					trans=NULL;
				}
				//PRINTN("Stall transaction " << hex << addr << " enqueue at " << dec << i << endl);
			}

			(*memorySystem).update();
		}
	}
	// For spec traces with relative time
	else {
		traceFile1.open(traceFileName1.c_str());

		if (!traceFile1.is_open())
		{
			cout << "== Error - Could not open trace file 1"<<endl;
			exit(0);
		}
		
		for (size_t i=0;i<numCycles;i++)
		{
			// For thread 0 
			if (!pendingTrans)
			{
				if (!traceFile.eof() && flag == 1)
				{
					getline(traceFile, line);

					if (line.size() > 24)
					{
						data = parseTraceFileLine(line, addr, transType,clockCycle, pid, traceType,useClockCycle);
						// if (pid == 0 && transType == DATA_READ) 
						//    inputFile << "Address: " << hex << addr << " Arrive time: " << dec << clockCycle << '\n';
						trans = new Transaction(transType, addr, data, 0, 0);
						alignTransactionAddress(*trans); 
						clockCycle = clockCycle + i;
						flag = 0;

						if (i>=clockCycle)
						{
							if (!(*memorySystem).addTransaction(trans))
							{
								pendingTrans = true;
							}
							else
							{
	#ifdef RETURN_TRANSACTIONS
								transactionReceiver.add_pending(*trans, i); 
	#endif
								// the memory system accepted our request so now it takes ownership of it
								trans = NULL; 
							}
						}
						else
						{
							pendingTrans = true;
						}
					}
					else
					{
						DEBUG("WARNING: Skipping line "<<lineNumber<< " ('" << line << "') in tracefile");
					}
					lineNumber++;
				}
				else
				{
					//we're out of trace, set pending=false and let the thing spin without adding transactions
					pendingTrans = false; 
				}
			}

			else if (pendingTrans && i >= clockCycle)
			{
				pendingTrans = !(*memorySystem).addTransaction(trans);
				if (!pendingTrans)
				{
	#ifdef RETURN_TRANSACTIONS
					transactionReceiver.add_pending(*trans, i); 
	#endif
					trans=NULL;
				}
				//PRINTN("Stall transaction " << hex << addr << " enqueue at " << dec << i << endl);
			}
			// For thread 1
			if (!pendingTrans1)
			{
				if (!traceFile1.eof() && flag1 == 1)
				{
					getline(traceFile1, line);

					if (line.size() > 24)
					{
						data = parseTraceFileLine(line, addr, transType,clockCycle1, pid, traceType,useClockCycle);
						// if (pid == 0 && transType == DATA_READ) 
						//    inputFile << "Address: " << hex << addr << " Arrive time: " << dec << clockCycle << '\n';
						trans1 = new Transaction(transType, addr, data, 1, 0);
						alignTransactionAddress(*trans1); 
						clockCycle1 = clockCycle1 + i;
						flag1 = 0;

						if (i>=clockCycle1)
						{
							if (!(*memorySystem).addTransaction(trans1))
							{
								pendingTrans1 = true;
							}
							else
							{
	#ifdef RETURN_TRANSACTIONS
								transactionReceiver.add_pending(*trans1, i); 
	#endif
								// the memory system accepted our request so now it takes ownership of it
								trans1 = NULL; 
							}
						}
						else
						{
							pendingTrans1 = true;
						}
					}
					else
					{
						DEBUG("WARNING: Skipping line "<<lineNumber1<< " ('" << line << "') in tracefile");
					}
					lineNumber1++;
				}
				else
				{
					//we're out of trace, set pending=false and let the thing spin without adding transactions
					pendingTrans1 = false; 
				}
			}

			else if (pendingTrans1 && i >= clockCycle1)
			{
				pendingTrans1 = !(*memorySystem).addTransaction(trans1);
				if (!pendingTrans1)
				{
	#ifdef RETURN_TRANSACTIONS
					transactionReceiver.add_pending(*trans1, i); 
	#endif
					trans1=NULL;
				}
				//PRINTN("Stall transaction " << hex << addr << " enqueue at " << dec << i << endl);
			}

			(*memorySystem).update();
			
			if (traceFile.eof() && traceFile1.eof()) {cout<< "finish traces"<<endl; break;}
		}
		traceFile1.close();
	}

	traceFile.close();
	memorySystem->printStats(true);
	// make valgrind happy
	if (trans)
	{
		delete trans;
	}
	delete(memorySystem);
}
#endif
