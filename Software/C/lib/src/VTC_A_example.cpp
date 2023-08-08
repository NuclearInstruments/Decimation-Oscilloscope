#include "Def.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <chrono>
#include <cstdlib> // Inclusione di cstdlib per l'esecuzione di un comando di sistema

using namespace std;

#include  "VTC_A_lib.h"


const string PATH = "../../../python_plot";
const int32_t size_osc = 8192;
const int32_t nch = 32;



class DGTZ {
public:
	NI_HANDLE handle;
	string filename;
	string ip;
	bool connected;
	int target_waves;


	DGTZ(string _ip, string _filename) {
		ip = _ip;
		filename = _filename;
	}

	int Connect() {
		connected = false;
		if (R_ConnectDevice((char*)ip.c_str(), 8888, &handle) != 0) { return (-1); };
		connected = true;
		return 0;
	}
};

void DGTZ_Thread(void *context);

int main(int argc, char* argv[])
{
	vector<DGTZ> dgtz;
	string ip = "192.168.102.219";																//IP OF THE DDEVICE
	double acquisizion_time = 0.005;															//SET ACQUISITION LENGTH IN MS
	int32_t pre_trigger = 10;																	//SET PRETRIGGER IN SAMPLE
	int32_t trigger_source = 1;																	//0: EXTERNAL LEMO 0
																								//1: INTERNAL PERIODIC
	int		acquisition_target = 10;															//NUMBER OF ACQUISITION IN THE RUN

	int32_t decimator = ceil(log( (acquisizion_time * 1.0e9) /(8* size_osc))/log(2)) ;
	double real_acquisition_time = (double)pow(2,decimator) * (8 * size_osc) *1e-9;
	int32_t software_trigger = 0;
	int32_t analog_trigger = 0;
	int32_t digital0_trigger = 0;
	int32_t digital1_trigger = 0;
	int32_t digital2_trigger = 0;
	int32_t digital3_trigger = 0;
	int32_t trigger_channel = 0;
	int32_t trigger_edge = 0;
	int32_t trigger_level = 1000;
	vector<thread> threads;

	R_Init();




	dgtz.push_back(DGTZ(ip, PATH + "/w"));



	for (int i = 0; i < dgtz.size(); i++) {
		if (dgtz[i].Connect() == 0) {
			cout << "Connected to " << dgtz[i].ip << endl;
		}else{
			cout << "Unable to connect to the board " << dgtz[i].ip << endl;
			exit(-1);
		}
	}


	cout << "Oscilloscope will dump " << size_osc << " for " << real_acquisition_time*1000.0 << " ms ";
	

	for (int i = 0; i < dgtz.size(); i++) {

		REG_TRG_SEL_SET(trigger_source, &dgtz[i].handle);
		REG_PULSE_PERIOD_SET(50000000/8, &dgtz[i].handle);

		if (OSCILLOSCOPE_Oscilloscope_0_SET_PARAMETERS(decimator, pre_trigger, software_trigger, analog_trigger, digital0_trigger, digital1_trigger,
			digital2_trigger, digital3_trigger, trigger_channel, trigger_edge, trigger_level, &dgtz[i].handle) != 0) {
			cout << "Unable to configure board " << dgtz[i].ip << endl;
		}
		else {
			cout << "Board " << dgtz[i].ip << " configured." << endl ;
		}
	}

	//SetSyncRegister(0, 0, &dgtz[0].handle);
	for (int i = 0; i < dgtz.size(); i++) {
		dgtz[i].target_waves = acquisition_target;
		if (OSCILLOSCOPE_Oscilloscope_0_START(&dgtz[i].handle) != 0) {
			cout << "Unable to start board " << dgtz[i].ip << endl;
		}
		//threads.emplace_back(DGTZ_Thread, ::std::ref(d));
		threads.push_back( thread(DGTZ_Thread,  (void*) (&dgtz[i])));
		
	}

	//SetSyncRegister(0, 1, &dgtz[0].handle);

	
	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
	//SetSyncRegister(0, 0, &dgtz[0].handle);
	return 0;
}


void DGTZ_Thread(void *context) {
	DGTZ *cntx;
	cntx = (DGTZ*)context;
	FILE *fp;
	int32_t pre_trigger = 100;
	uint32_t status_osc = 0;
	uint32_t read_data_osc;
	uint32_t valid_data_osc;
	int32_t position = 0;

	int32_t timeout_osc = 1000;

	uint32_t *data_osc = (uint32_t*)malloc(2 * size_osc*nch * sizeof(uint32_t));
	uint32_t *read_analog = (uint32_t*)malloc(2 * size_osc*nch * sizeof(uint32_t));
	uint32_t *read_digital0 = (uint32_t*)malloc(2 * size_osc*nch * sizeof(uint32_t));
	uint32_t *read_digital1 = (uint32_t*)malloc(2 * size_osc*nch * sizeof(uint32_t));
	uint32_t *read_digital2 = (uint32_t*)malloc(2 * size_osc*nch * sizeof(uint32_t));
	uint32_t *read_digital3 = (uint32_t*)malloc(2 * size_osc*nch * sizeof(uint32_t));

	if ((data_osc == NULL) ||
		(read_analog == NULL) ||
		(read_digital0 == NULL) ||
		(read_digital1 == NULL) ||
		(read_digital2 == NULL) ||
		(read_digital3 == NULL))
		return;

	int Osc_Events = 10;
	int e = 0;

	char line_string[4096];

	auto start = std::chrono::high_resolution_clock::now();
	for (int j = 0; j < cntx->target_waves; j++) {

		while (status_osc != 1)
			if (OSCILLOSCOPE_Oscilloscope_0_STATUS(&status_osc, &cntx->handle) != 0) cout << "Status Error" << endl;
		if (OSCILLOSCOPE_Oscilloscope_0_POSITION(&position, &cntx->handle) != 0) cout << "Position Error" << endl;
		if (OSCILLOSCOPE_Oscilloscope_0_DOWNLOAD(data_osc, size_osc*nch / 2, timeout_osc, &cntx->handle, &read_data_osc, &valid_data_osc) != 0) cout << "Get Data Error" << endl;
		if (OSCILLOSCOPE_Oscilloscope_0_RECONSTRUCT(data_osc, position, pre_trigger, read_analog, read_digital0, read_digital1, read_digital2, read_digital3) != 0) cout << "Reconstruction Error" << endl;

		string file = cntx->filename + "_" + std::to_string(j) + ".txt";
		int ret = fopen_s(&fp, file.c_str(), "w");
		if (ret)return;

		// IMPORTANT!! 
		// WRITE TEXT FILE IS EXTREMELY SLOW
		// USE A BINARY FILE INSTEAD OTHERWISE YOU WILL NOT
		// KEEP 20 Hz
		// OPTIMIZE THIS PART OF CODE
		// EXECUTION TIME WITHOUT SAVING IS 35 ms per ACQUISITION, 1.5 S WIT FILE SAVING 

		for (int i = 0; i < size_osc - 1; i++) {
			line_string[0] = '\0';
			int pos = 0;
			for (int n = 0; n < nch; n++) {
				pos += sprintf_s(line_string + pos, 2048, "%8d", read_analog[(n*size_osc) + i]);
			}
			sprintf_s(line_string + pos, 2048, "\n");
			fputs(line_string, fp);
		}
		e++;
		fclose(fp);
	}

	auto end = std::chrono::high_resolution_clock::now();


	std::chrono::duration<double, std::milli> elapsed = end - start;
	double elapsedTimeMs = elapsed.count();

	cout << "Acquisition time: " << elapsedTimeMs << endl;


}