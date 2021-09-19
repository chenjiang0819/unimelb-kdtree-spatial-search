/* EInformation Retrieval Using a Linked list:
 *
 * This code is written by Chen Jiang, student ID: REDACTED_STUDENT_ID , 
 * for Assignment 2 of COMP20003 Algorithms and Data Structures 
 * in 09 September 2020 at the University of MElbourne
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "two_d_tree.h"
#include "processingstring.h"

#define COMMA 1
#define NULLBIT 1
#define BRACKET 1
#define INITIALISE 0
#define BEGINING 0
#define BLOCK_ID 1
#define PROPERTY_ID 2
#define BASE_PRPERTY_ID 3
#define INDUSTRY_CODE 6
#define X_COORDINATE 8
#define Y_COORDINATE 9
#define REMOVE_EXTRA 10
#define DOUBLE_FOR_TREE 11

/****************************************************************/
//the main function
int main(int argc, char** argv){
	int read;
	int count;
	int i;
	int switchs;
	//let computer to decide the size of the momery spaces that assigned to the buffer 
	size_t len = BEGINING;
	char* buffer = NULL;
    //create a empty tree for further use
    tree_t* tree = make_empty_tree();
    //using read-only to read the csv file from the argv 
    FILE* reading = fopen(argv[1],"r");
    assert(reading != NULL);
    //open the output file from the argv for further use 
    FILE* output_file = fopen(argv[2],"w");
    //skip the first line sicnce it is the titles of the columns
    getline(&buffer, &len, reading);
    //write the data in the nodes and insert the nodes in the tree
	while((read = getline(&buffer, &len, reading)) != EOF){
		count = BEGINING;
		i = BEGINING;
		switchs = BEGINING;
		//create the new nodes
		node_t* new_node = (node_t*)malloc(sizeof(*new_node));
    	assert(new_node != NULL);
    	//create a temporary string ro store information 
    	char temp[strlen(buffer) + NULLBIT];
    	//insert the data in the nodes
    	while(buffer){
    		if(buffer[count] == ',' && switchs == BEGINING){
    			temp[i] = '\0';
    			if(temp[0] == '\0'){
    				new_node -> census_year = 0;
    			}else{
    				new_node -> census_year = atoi(temp);
    			}
    			memset(temp, BEGINING, sizeof(temp));
    			i = BEGINING;
    			count++;
    			switchs++;
    		}else if(buffer[count] == ',' && switchs == BLOCK_ID){
    			temp[i] = '\0';
    			if(temp[0] == '\0'){
    				new_node -> block_id = 0;
    			}else{
    				new_node -> block_id = atoi(temp);
    			}
    			memset(temp ,BEGINING,sizeof(temp));
    			i = BEGINING;
    			count++;
    			switchs++;
    		}else if(buffer[count] == ',' && switchs == PROPERTY_ID){
    			temp[i] = '\0';
    			if(temp[0] == '\0'){
    				new_node -> property_id = 0;
    			}else{
    				new_node -> property_id = atoi(temp);
    			}
    			memset(temp ,BEGINING,sizeof(temp));
    			i = BEGINING;
    			count++;
    			switchs++;
    		}else if(buffer[count] == ',' && switchs == BASE_PRPERTY_ID){
    			temp[i] = '\0';
    			if(temp[0] == '\0'){
    				new_node -> base_property_id = 0;
    			}else{
    				new_node -> base_property_id = atoi(temp);
    			}
    			memset(temp ,BEGINING,sizeof(temp));
    			i = BEGINING;
    			count++;
    			switchs++;
    			//process clue_small_area individually, in case there is special situation  
				processing_clue_small_area(buffer, &count, new_node );
				switchs++;
				//process trading_name individually, in case there is special situation  
   				processing_trading_name(buffer, &count, new_node );
   				switchs++;
    		}else if(buffer[count] == ',' && switchs == INDUSTRY_CODE){
    			temp[i] = '\0';
    			if(temp[0] == '\0'){
    				new_node -> industry_code = 0;
    			}else{
    				new_node -> industry_code = atoi(temp);
    			}
    			memset(temp ,BEGINING,sizeof(temp));
    			i = BEGINING;
    			count++;
    			switchs++;
    			//process industry_description individually, in case there is special situation
	    		processing_industry_description(buffer, &count, new_node );
	    		switchs++;
	    	}else if(buffer[count] == ',' && switchs == X_COORDINATE){
	    		temp[i] = '\0';
	    		if(temp[0] == '\0'){
    				new_node -> x_coordinate = 0;
    			}else{
    				new_node -> x_coordinate = atof(temp);
    			}
    			memset(temp ,BEGINING,sizeof(temp));
    			i = BEGINING;
    			count++;
    			switchs++;
    		}else if(buffer[count] == ',' && switchs == Y_COORDINATE){
    			temp[i] = '\0';
    			if(temp[0] == '\0'){
    				new_node -> y_coordinate = 0;
    			}else{
    				new_node -> y_coordinate = atof(temp);
    			}
    			memset(temp ,BEGINING,sizeof(temp));
    			i = BEGINING;
    			count++;
    			switchs++;
    		//remove extra chars (we don't need to store in the momery) to save the space of memory
    		}else if(buffer[count] == '(' && switchs == REMOVE_EXTRA){
    			memset(temp ,BEGINING,sizeof(temp));
    			i = BEGINING;
    			count++;
    			switchs++;
    		}else if(buffer[count] == '\"' && switchs == DOUBLE_FOR_TREE){
    			//for accuracy, we store the location as string instead of float or double.
    			temp[i] = '\0';
    			new_node -> location = (char*)malloc(strlen(temp) + NULLBIT);
    			assert(new_node -> location != NULL);
	    		strcpy(new_node -> location, temp);
	    		char coordinates[strlen(temp) + NULLBIT];
    			i = BEGINING;
    			count = BEGINING;
    			//convert the string to double for 2d tree
    			while(temp[count] != '\0' ){
    				if(temp[count] == ',' ){
    					coordinates[i] = '\0';
    					new_node -> y_for_tree = atof(coordinates);
    					i = BEGINING;
    					count++;
    				}else if(temp[count] == ')'){
    					coordinates[i] = '\0';
    					new_node -> x_for_tree = atof(coordinates);
    					break;
    				}else{
    					coordinates[i++] = temp[count++];
    				}
    			}
    			break;
    		}else{
    			temp[i++] = buffer[count++];
    		}

    	}

/*We also can use this more straightforward method as a solution for reading input. ：）
    	
    	//insert the data in the nodes
		char *temp = NULL;
        temp = strtok(buffer, ",");
		count = count + strlen(temp)+COMMA;
		new_node -> census_year = atoi(temp);

		temp = strtok(buffer + count, ",");
		count = count + strlen(temp)+COMMA;
	    new_node -> block_id = atoi(temp);

	    temp = strtok(buffer + count, ",");
	    count = count + strlen(temp)+COMMA;
	    new_node -> property_id = atoi(temp);

	    temp = strtok(buffer + count, ",");
	    count = count + strlen(temp)+COMMA;
		new_node -> base_property_id = atoi(temp);
		
		//process clue_small_area individually, in case there is special situation  
		processing_clue_small_area(buffer, &count, new_node );
		//process trading_name individually, in case there is special situation  
   		processing_trading_name(buffer, &count, new_node );

		temp = strtok(buffer + count, ",");
	    count = count + strlen(temp)+COMMA;
	    new_node -> industry_code = atoi(temp);

	    //process industry_description individually, in case there is special situation
	    processing_industry_description(buffer, &count, new_node );

		temp = strtok(buffer + count, ",");
	    count = count + strlen(temp)+COMMA;
	    new_node -> x_coordinate = atof(temp);
		
	    temp = strtok(buffer + count, ",");
	    count = count + strlen(temp)+COMMA;
	    new_node -> y_coordinate = atof(temp);

	    //remove extra chars (we don't need to store in the momery) to save the space of memory
	    temp = strtok(buffer + count, "(");
        count = count + strlen(temp)+BRACKET;

		//for accuracy, we store the location as string instead of float or double.
	    temp = strtok(buffer + count, "\"");
	    new_node -> location = (char*)malloc(strlen(temp) + NULLBIT);
	    strcpy(new_node -> location, temp);

	    //convert the string to double for 2d tree
	    char* coordinate = strtok(temp, ",");
	    count = strlen(coordinate)+COMMA;
	    new_node -> y_for_tree = atof(coordinate);
	    coordinate = strtok(temp + count, ")");
	    new_node -> x_for_tree = atof(coordinate);*/
	   
	    //initialise the left, right and depth of the new nodes
		new_node -> left = NULL;
		new_node -> right = NULL;
		new_node -> next = NULL;
		new_node -> list_foot = NULL;
		new_node -> depth = INITIALISE;
		//insert the nodes into the 2d tree
		tree = insert_node(tree, new_node);
	}
	//read the coordinates from the keyfile and print the results into the outputfile
	output_result(tree, output_file);
	//close the input file
	fclose(reading);
	//close the output file
	fclose(output_file);
	//free buffer
    free(buffer);
    buffer = NULL;
	//free all memory space associated with the tree structure
	free_tree(tree);
    return 0;
}