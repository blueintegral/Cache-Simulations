// simulate_caches.cpp : Defines the entry point for the console application.

//This program simulates a bunch of different kinds of caches


//Hunter Scott, 11/23/10

//Each line in the memory dump has 3 fields. The first is the type of memory reference:

// 0 - load word (memory read)
// 1 - store word (memory written)
// 2 - instruction fetch

// The third field is a 0 for lw and sw, but if it was an insruction fetch, 
// then it's the instruction that was fetched. 

// This trace is known to be 1 million lines long, so that's hard coded in.


#include "stdafx.h"
#include "Windows.h"
#include <math.h>

void check_icache(int new_tag, int icache[], int address);
void check_dcache(int new_tag, int dcache[], int address);



int ihit_count = 0, imiss_count = 0, dmiss_count = 0, dhit_count = 0;



int _tmain(int argc, _TCHAR* argv[])
{
FILE *fp;
FILE  *fp2;
  int i, j, l, k, type, tag_shift, trace_length;
  int icache[32768], dcache[32768];
  float ihit_percent, imiss_percent, dmiss_percent, dhit_percent;
  unsigned int address, tag, instr, new_tag;
  unsigned int masked_addr;

  // open trace file for reading

  fp = fopen("C:\\gcc_trace2.txt", "r");
  if (fp == NULL) printf("data file open error");

  trace_length = 1000000;
 

  //First, simulate nonassociative cache//
  ////////////////////////////////////////

 // for(l=256; l <= 32768; l = l * 2){ //go through each cache size of 256 to 32K
	//for(l=256; l <= 2048; l = l * 2){ //for testing, just do 256, 512, and 1024 sized caches
 l = 128;
  for(j=0; j<=l; j++){ //reset caches to 0 to prevent false positives
		  icache[j] = 0;
		  dcache[j] = 0;
	  }
  imiss_count = 0;
  ihit_count = 0;
  dmiss_count = 0;
  dhit_count = 0;
  //printf("cache reset\n");

(int)tag_shift = (log((double)l))/log((double)2); //this does log2(l)

for (i=0; i<=trace_length; i++) {

    // read 1 line of trace file

    fscanf_s(fp, "%d %x %x", &type, &address, &instr);

	//printf("instruction is %x\n\n", instr );

	tag = address >> 2;
	new_tag = tag & (l-1); //this is L minus one

	////printf("tag shift is %i\n", tag_shift);
	////Sleep(500);
	//new_tag = tag << ((32 - tag_shift));
	//new_tag = new_tag >> ((32 - tag_shift));
	//printf("new tag is %i\n", new_tag);

	if(type == 2){ //fetch
		
		check_icache(new_tag, icache, address);

}

	if(type == 0){ //load instruction
		check_dcache(new_tag, icache, address);

	}
  
	if(type == 1) ; //store
	
  // Delay a bit to see output
	//Sleep(1000);

	
} //end of file parse loop
	

 ihit_percent = (((float)(ihit_count))/((float)(imiss_count+ihit_count)))*100;
  imiss_percent = (((float)(imiss_count))/((float)(imiss_count+ihit_count)))*100;
   dhit_percent = (((float)(dhit_count))/((float)(dmiss_count+dhit_count)))*100;
  dmiss_percent = (((float)(dmiss_count))/((float)(dmiss_count+dhit_count)))*100;

 // printf("I hit count is %i\n", ihit_count);
  //printf("I miss count is %i\n", imiss_count);
//  printf("l is %i\n", l);
  printf("I hit percentage for %i cache is %f\n", l, ihit_percent);
  printf("I miss percentage for %i cache is %f\n", l, imiss_percent);

  //printf("D hit count is %i\n", dhit_count);
  //printf("D miss count is %i\n", dmiss_count);
  //printf("D hit percentage for %i cache is %f\n", l, dhit_percent);
  printf("D miss percentage for %i cache is %f\n", l, dmiss_percent);
	

	//} //end of loop of caches

	  Sleep(15000);
return 0;

} //end of main()

void check_icache(int new_tag, int icache[], int address){
	//printf("new tag is %i\n", new_tag);
	if(icache[new_tag] == 0){
		icache[new_tag] = (address >> 2);
	//	printf("address shifted is %i\n", icache[new_tag]);
		//Sleep(3000);
		imiss_count++;
	}else{
		if(icache[new_tag] == (address >> 2)){
			ihit_count++;
		}else{
			imiss_count++;
			icache[new_tag] = (address >> 2);
		}
	}
}

void check_dcache(int new_tag, int dcache[], int address){
	//printf("new tag is %i\n", new_tag);
	if(dcache[new_tag] == 0){
		dcache[new_tag] = (address >> 2);
	//	printf("address shifted is %i\n", icache[new_tag]);
		//Sleep(3000);
		dmiss_count++;
	}else{
		if(dcache[new_tag] == (address >> 2)){
			dhit_count++;
		}else{
			dmiss_count++;
			dcache[new_tag] = (address >> 2);
		}
	}
}