#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "processingstring.h"

#define COMMA 1
#define COMMAAandSLASH 2
#define NULLBIT 1

/****************************************************************/

//processing the trading name to prevent if there is special case.
void processing_trading_name(char* buffer, int* count, node_t* new_node ){
	char* temp;
	//if the quotations in a sentences, the number of the quotations before comma should be odd.  
	int count_quotation = 0; 
	//detect the special case
	if(buffer[(*count)] == '\"'){
	int initial_size = 1;
	int i = 0;
	new_node -> trading_name = (char*)malloc(initial_size + NULLBIT);
	assert(new_node -> trading_name != NULL);
	(*count)++;
	//detect the end of the special case
	while((buffer[(*count)] == '\"' && buffer[(*count)+COMMA] == ',' && count_quotation % 2 == 0) == 0){
		if(buffer[(*count)] != '\"'){
    		new_node -> trading_name[i++] = buffer[(*count)++];
    	}else if(new_node -> trading_name[i-1] != '\"'){
    		count_quotation ++;
    		new_node -> trading_name[i++] = buffer[(*count)++];
    	}else{
    		count_quotation ++;
    		(*count)++;
    	}
		if(i >=  initial_size){
			//add the more space for the trading name
			initial_size *= 2;
			new_node -> trading_name = (char*)realloc(new_node -> trading_name,initial_size + NULLBIT);
			assert(new_node -> trading_name != NULL);
		}
    }
    new_node -> trading_name[i] = '\0'; 
    (*count) += COMMAAandSLASH;
	}else{
		//for the normal situation
		temp = strtok(buffer + *count, ",");
		(*count) = (*count) + strlen(temp)+COMMA;
		new_node -> trading_name = (char*)malloc((*count) + NULLBIT);
		assert(new_node -> trading_name != NULL);
		strcpy(new_node -> trading_name, temp); 
	}
}

//processing the ndustry description to prevent if there is special case.
void processing_industry_description(char* buffer, int* count, node_t* new_node ){
	char* temp;
	//if the quotations in a sentences, the number of the quotations before comma should be odd.  
	int count_quotation = 0; 
	//detect the special case
	if(buffer[(*count)] == '\"'){
	int initial_size = 1;
	int i = 0;
	new_node -> industry_description = (char*)malloc(initial_size + NULLBIT);
	assert(new_node -> industry_description != NULL);
	(*count)++;
	//detect the end of the special case
	while((buffer[(*count)] == '\"' && buffer[(*count)+COMMA] == ',' && count_quotation % 2 == 0) == 0){
		if(buffer[(*count)] != '\"'){
    		new_node -> industry_description[i++] = buffer[(*count)++];
    	}else if(new_node -> industry_description[i-1] != '\"'){
    		count_quotation ++;
    		new_node -> industry_description[i++] = buffer[(*count)++];
    	}else{
    		count_quotation ++;
    		(*count)++;
    	}
		if(i >=  initial_size){
			//add the more space for the trading name
			initial_size *= 2;
			new_node -> industry_description = (char*)realloc(new_node -> industry_description,initial_size + NULLBIT);
			assert(new_node -> industry_description != NULL);
		}
    }
    new_node -> industry_description[i] = '\0'; 
    (*count) += COMMAAandSLASH;
	}else{
		//for the normal situation
		temp = strtok(buffer + *count, ",");
		(*count) = (*count) + strlen(temp)+COMMA;
		new_node -> industry_description = (char*)malloc((*count) + NULLBIT);
		assert(new_node -> industry_description != NULL);
		strcpy(new_node -> industry_description, temp); 
	}
}

//processing the clue small area to prevent if there is special case.
void processing_clue_small_area(char* buffer, int* count, node_t* new_node ){
	char* temp;
	//if the quotations in a sentences, the number of the quotations before comma should be odd. 
	int count_quotation = 0; 
	//detect the special case
	if(buffer[(*count)] == '\"'){
	int initial_size = 1;
	int i = 0;
	new_node -> clue_small_area = (char*)malloc(initial_size + NULLBIT);
	assert(new_node -> clue_small_area != NULL);
	(*count)++;
	//detect the end of the special case
	while((buffer[(*count)] == '\"' && buffer[(*count)+COMMA] == ',' && count_quotation % 2 == 0) == 0){
		if(buffer[(*count)] != '\"'){
    		new_node -> clue_small_area[i++] = buffer[(*count)++];
    	}else if(new_node -> clue_small_area[i-1] != '\"'){
    		count_quotation ++;
    		new_node -> clue_small_area[i++] = buffer[(*count)++];
    	}else{
    		count_quotation ++;
    		(*count)++;
    	}
		if(i >=  initial_size){
			//add the more space for the trading name
			initial_size *= 2;
			new_node -> clue_small_area = (char*)realloc(new_node -> clue_small_area,initial_size + NULLBIT);
			assert(new_node -> clue_small_area != NULL);
		}
    }
    new_node -> clue_small_area[i] = '\0'; 
    (*count) += COMMAAandSLASH;
	}else{
		//for the normal situation
		temp = strtok(buffer + *count, ",");
		(*count) = (*count) + strlen(temp)+COMMA;
		new_node -> clue_small_area = (char*)malloc((*count) + NULLBIT);
		assert(new_node -> clue_small_area != NULL);
			strcpy(new_node -> clue_small_area ,temp); 
	}
}
