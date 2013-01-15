// set_associative_cache_sim.cpp : Defines the entry point for the console application.

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


int ihit_count = 0, imiss_count = 0, dmiss_count = 0, dhit_count = 0;

int set = 8; ////set associativity

struct cache{
			  int address;
			  int age;
} ;

struct cache *icacheArray;
struct cache *dcacheArray;

void check_icache(int new_tag, cache *icacheArray, int address);
void check_dcache(int new_tag, cache *dcacheArray, int address);


int counter=0;
///////////
//Start Main()
///////////

int _tmain(int argc, _TCHAR* argv[])
{
FILE *fp;
FILE  *fp2;
  int i, j, L, k, type, tag_shift, trace_length;

  float ihit_percent, imiss_percent, dmiss_percent, dhit_percent;
  unsigned int address, tag, instr, new_tag;
  unsigned int masked_addr;

  //calloc some space for the arrays
  icacheArray = (cache*)(calloc(32768,sizeof(cache)));
  dcacheArray = (cache*)(calloc(32768,sizeof(cache)));

  if(icacheArray == NULL) printf("ran out of space to alloc icacheArray\n");
  if(dcacheArray == NULL) printf("ran out of space to alloc dcacheArray\n");


  // open trace file for reading

  fp = fopen("C:\\gcc_trace2.txt", "r");
  if (fp == NULL) printf("data file open error");

  trace_length = 1000000; ///////set number of instructions to parse
 

  ///////////set cache size
 L = 1024;
 

  imiss_count = 0;
  ihit_count = 0;
  dmiss_count = 0;
  dhit_count = 0;
  //printf("cache reset\n");

(int)tag_shift = (log((double)L))/log((double)2); //this does log2(L)

for (i=0; i<=trace_length; i++) {

    // read 1 line of trace file

    fscanf_s(fp, "%d %x %x", &type, &address, &instr);

	//printf("instruction is %x\n\n", instr );

	tag = address >> 2;
	new_tag = tag & (L-1); 

	////printf("tag shift is %i\n", tag_shift);
	//printf("new tag is %i\n", new_tag);


	if(type == 2){ //fetch
		
		check_icache(new_tag, icacheArray, tag);

}

	if(type == 0){ //load instruction
		check_dcache(new_tag, dcacheArray, tag);

	}
  
	if(type == 1) ; //store
	
  // Delay a bit to see output
	//Sleep(1000);

	//increase the age of everything by 1
	for(int q = 0; q<=L; q++){ 
		icacheArray[q].age++;
		dcacheArray[q].age++;
	}




} //end of file parse loop
	

 ihit_percent = (((float)(ihit_count))/((float)(imiss_count+ihit_count)))*100;
  imiss_percent = (((float)(imiss_count))/((float)(imiss_count+ihit_count)))*100;
   dhit_percent = (((float)(dhit_count))/((float)(dmiss_count+dhit_count)))*100;
  dmiss_percent = (((float)(dmiss_count))/((float)(dmiss_count+dhit_count)))*100;

  printf("I hit count is %i\n", ihit_count);
  printf("I miss count is %i\n", imiss_count);
  printf("dividing by %f\n", imiss_count+ihit_count);
  printf("decimal version is %f\n", ihit_count/(imiss_count+ihit_count));
  //  printf("l is %i\n", l);
  printf("I hit percentage for %i cache is %f\n", L, ihit_percent);
  printf("I miss percentage for %i cache is %f\n", L, imiss_percent);

  //printf("D hit count is %i\n", dhit_count);
  //printf("D miss count is %i\n", dmiss_count);
  //printf("D hit percentage for %i cache is %f\n", L, dhit_percent);
  printf("D miss percentage for %i cache is %f\n", L, dmiss_percent);
	

	//} //end of loop of caches

	  Sleep(15000); //////set how long to wait before exiting after printing results
return 0;

} //end of main()

void check_icache(int new_tag, cache *icacheArray, int address){
	int row_offset, row, current_age, age_index, found = 0;
	//row = new_tag;
	row_offset = new_tag*set;
	counter++;


	for(int m = 0; m < (set); m++){

		if(icacheArray[row_offset+m].address == address){
			ihit_count++;
			found = 1;
			icacheArray[row_offset+m].age = 0;
			//printf("hit\n");
			return;
		}

	//	if(m == set-1){ //not sure why this is here, if it's not a hit, it must be a miss
			//miss_count++ used to be here
		//printf("miss\n");
		
	} //end of for loop
	
	if(!found){
	imiss_count++;
	}


	for( int n = 0; n < (set); n++){

		if(icacheArray[row_offset+n].address == 0){
			icacheArray[row_offset+n].address = address;
			icacheArray[row_offset+n].age = 0;
			break;
		}else{
			//no open spaces, so need to look for oldest thing to kick out
			current_age = 0;
			age_index = 0;
			
			for(int p = 0; p < (set); p++){
				
				if(icacheArray[row_offset+p].age > current_age){
					current_age = icacheArray[row_offset+p].age;
					age_index = row_offset+p;
				}

			}
			//at this point, current_age stores the age of the oldest thing and age_index is where in the array the oldest thing is
			//printf("age index is %i\n", age_index);
			//Sleep(1); //no idea why it needs this to work, but it does.
			
			icacheArray[age_index].address = address; //so store the address where the oldest thing was
			icacheArray[age_index].age = 0;
			
		} //end of if statement

} //end of for loop
	//printf("counter is %i\n", counter);
}





void check_dcache(int new_tag, cache *dcacheArray, int address){

	int row_offset, row, current_age, age_index, found = 0;
	//row = new_tag;
	row_offset = new_tag*set;
	counter++;


	for(int m = 0; m < (set); m++){

		if(dcacheArray[row_offset+m].address == address){
			dhit_count++;
			found = 1;
			dcacheArray[row_offset+m].age = 0;
			//printf("hit\n");
			return;
		}

	//	if(m == set-1){ //not sure why this is here, if it's not a hit, it must be a miss
			//miss_count++ used to be here
		//printf("miss\n");
		
	} //end of for loop
	
	if(!found){
	dmiss_count++;
	}


	for( int n = 0; n < (set); n++){

		if(dcacheArray[row_offset+n].address == 0){
			dcacheArray[row_offset+n].address = address;
			dcacheArray[row_offset+n].age = 0;
			break;
		}else{
			//no open spaces, so need to look for oldest thing to kick out
			current_age = 0;
			age_index = 0;
			
			for(int p = 0; p < (set); p++){
				
				if(dcacheArray[row_offset+p].age > current_age){
					current_age = dcacheArray[row_offset+p].age;
					age_index = row_offset+p;
				}

			}
			//at this point, current_age stores the age of the oldest thing and age_index is where in the array the oldest thing is
			//printf("age index is %i\n", age_index);
			//Sleep(1); //no idea why it needs this to work, but it does.
			
			dcacheArray[age_index].address = address; //so store the address where the oldest thing was
			dcacheArray[age_index].age = 0;
			
		} //end of if statement

} //end of for loop
	//printf("counter is %i\n", counter);
}
