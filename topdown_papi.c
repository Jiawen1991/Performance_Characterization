//compile:
//	gcc -shared -o libtopdown_papi.so -fPIC topdown_papi.c

#include <stdio.h>
#include <papi.h>
#include <stdlib.h>

int EventSet;
int num_papi_events;
int * events;
long long * valors;
int EventCode;
char * event_names[] = {"CPU_CLK_THREAD_UNHALTED","IDQ_UOPS_NOT_DELIVERED:CORE","UOPS_ISSUED:ANY", "UOPS_RETIRED:RETIRE_SLOTS","INT_MISC:RECOVERY_CYCLES", "BR_MISP_RETIRED:ALL_BRANCHES","MACHINE_CLEARS:COUNT", "IDQ:MS_UOPS", "CYCLE_ACTIVITY:CYCLES_NO_EXECUTE","RS_EVENTS:EMPTY_CYCLES","UOPS_EXECUTED:CORE","CYCLE_ACTIVITY:STALLS_LDM_PENDING","RESOURCE_STALLS:SB","CYCLE_ACTIVITY:STALLS_L1D_PENDING","CYCLE_ACTIVITY:STALLS_L2_PENDING","MEM_LOAD_UOPS_RETIRED:L3_HIT","MEM_LOAD_UOPS_RETIRED:L3_MISS"};

void topdown_papi_init(){
        EventSet = PAPI_NULL;
        num_papi_events = 17;
        events = (int *) malloc(num_papi_events * sizeof(int));
        valors = (long long *) malloc(num_papi_events * sizeof(long long));
        EventCode = PAPI_NULL;

        PAPI_library_init(PAPI_VER_CURRENT);
        PAPI_create_eventset(&EventSet);
}

void topdown_papi_start(){
        int i;
        for(i = 0; i<num_papi_events; i++){
        PAPI_event_name_to_code(event_names[i] ,&EventCode);
        events[i] = EventCode;
        }
        PAPI_add_events( EventSet, events, num_papi_events);
        PAPI_start(EventSet);
        PAPI_reset(EventSet);
}

void topdown_papi_end(){
	PAPI_read(EventSet, valors);

	int i;
	for(i = 0; i<num_papi_events; i++)
        printf("%s = %llu\n\n",event_names[i], valors[i]);
        PAPI_stop(EventSet, NULL);
	
	long long Clocks, Slots, Ext_Memory_Bound;
	float Frontend_Bound, Bad_Speculation, Retiring, Backend_Bound, Frontend_Latency_Bound, BrMispredFraction, MicroSequencer, ExecutionStalls, Memory_Bound, L1_Bound, L2_Bound, L3HitFraction, L3_Bound, MEM_Bandwidth, MEM_Latency;

	Clocks = valors[0];
	Slots = 4 * Clocks;
	Frontend_Bound = (float)valors[1] / Slots;
	Bad_Speculation = ((float)valors[2] - valors[3] + (4 * valors[4]))/Slots;
	Retiring = (float)valors[3] / Slots;
	Backend_Bound = 1 - (Frontend_Bound + Bad_Speculation + Retiring);	

	Frontend_Latency_Bound = (float)valors[1] / Clocks;
	BrMispredFraction = (float)valors[5] / (valors[5] + valors[6]);
	MicroSequencer = ((float)valors[3] / valors[2]) * valors[7] / Slots; 
	ExecutionStalls = ((float)valors[8] - valors[9] + valors[10]) / Clocks;
	Memory_Bound = ((float)valors[11] + valors[12]) / Clocks;
	L1_Bound = ((float)valors[11] - valors[13]) / Clocks;
	L2_Bound = ((float)valors[13] - valors[14]) / Clocks;
	L3HitFraction = (float)valors[15] / (valors[15] + (7 * valors[16]));
	L3_Bound = (1 - L3HitFraction) * valors[14] / Clocks;
	Ext_Memory_Bound = valors[11];

	printf("----------\n");
	printf("Frontend Bound = %.6f\n\n",Frontend_Bound);
	printf("Bad_Speculation = %.6f\n\n",Bad_Speculation);
	printf("Retiring = %.6f\n\n",Retiring);
	printf("Backend_Bound = %.6f\n\n",Backend_Bound);
	printf("----------\n");
	printf("Clocks = %llu\n\n",Clocks);
	printf("Slots = %llu\n\n", Slots);
	printf("Frontend_Latency_Bound = %.6f\n\n", Frontend_Latency_Bound);
	printf("BrMispredFraction = %.6f\n\n", BrMispredFraction);
	printf("MicroSequencer = %.6f\n\n", MicroSequencer);
	printf("ExecutionStalls = %.6f\n\n", ExecutionStalls);
	printf("Memory_Bound = %.6f\n\n", Memory_Bound);
	printf("L1_Bound = %.6f\n\n", L1_Bound);
	printf("L2_Bound = %.6f\n\n", L2_Bound);
	printf("L3HitFraction = %.6f\n\n", L3HitFraction);
	printf("L3_Bound = %.6f\n\n", L3_Bound);
	printf("Ext_Memory_Bound = %llu\n\n", Ext_Memory_Bound);
	
}

void topdown_papi_finish(){

}

/*
int main(int argc, char **argv){
	topdown_papi_init();

	topdown_papi_start();	

	float pi=0.0;
	int i;
	for(i=0;i<NUM_PONTOS;i++){
		pi += 4.0/(4.0*i+1.0);
		pi -= 4.0/(4.0*i+3.0);
	}
	
	topdown_papi_end();

	topdown_papi_finish();

	return 0;
}
*/
