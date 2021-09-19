#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "two_d_tree.h"

#define TRUE 1
#define NULLBIT 1
#define RETURN 1
#define SPACE 1
#define BEGINING 0
#define IMPOSSIBLE -1
#define TURN_ON -1

/****************************************************************/

//function for making a new empty 2d tree
tree_t *make_empty_tree(){
    tree_t *tree;
	tree = (tree_t*)malloc(sizeof(*tree));
	assert(tree!=NULL);
	tree -> root = NULL;
	return tree;
};

//function for inserting the new nodes into 2d tree. 
tree_t* insert_node(tree_t* tree, node_t* new_node){
	tree -> root = recursive_insert(tree -> root, new_node);
    return tree;
};
//use the recursive function to insert the nodes
static node_t* recursive_insert(node_t* root, node_t* new_node){
	if(root == NULL){
        return new_node;
    }else if((new_node -> depth) % 2 == BEGINING){
    	(new_node -> depth) ++;
    	if((new_node -> x_for_tree) < (root -> x_for_tree)){
    		root -> left = recursive_insert(root -> left, new_node);
    	}else{
    		//put the nodes that have same location in a linked list
    		if(((new_node -> x_for_tree) == (root -> x_for_tree)) && ((new_node -> y_for_tree) == (root -> y_for_tree))){
    			if(root -> list_foot == NULL){
    				root -> next = new_node;
    				root -> list_foot = new_node;
    			}else{
    				root -> list_foot -> next = new_node;
    				root -> list_foot = new_node;
    			}
    			return root;
    		}
    		root -> right = recursive_insert(root -> right, new_node);
    	}
    }else if ((new_node -> depth) % 2 == 1){
    	(new_node -> depth) ++;
       	if((new_node -> y_for_tree) < (root -> y_for_tree)){
    		root -> left = recursive_insert(root -> left, new_node);
    	}else{
    		//put the nodes that have same location in a linked list
    		if(((new_node -> x_for_tree) == (root -> x_for_tree))&& ((new_node -> y_for_tree) == (root -> y_for_tree))){
    			if(root -> list_foot == NULL){
    				root -> next = new_node;
    				root -> list_foot = new_node;
    			}else{
    				root -> list_foot -> next = new_node;
    				root -> list_foot = new_node;
    			}
    			return root;
    		}
    		root -> right = recursive_insert(root -> right, new_node);
    	}
    }
    return root;
};

//read the key file and launch the searching  
void output_result(tree_t* tree, FILE* output_file){
	int initial_size = 1;
	int k = BEGINING;
	int j = BEGINING;
	int c;
    int new_count = BEGINING;
    char* input_data = NULL;
    input_data = (char*)malloc(initial_size + NULLBIT);
	assert(input_data != NULL);
	//get the key words
	while((c = getchar()) != EOF){
		input_data[k++] = c;
		if(k >=  initial_size){
			initial_size *= 2;
			input_data = (char*)realloc(input_data,initial_size + NULLBIT);
			assert(input_data != NULL);
		}
	}
    //set the stop point for strtok
    input_data[k++] = '\n';
    //search the coordinates in the tree to find the nearest business
	char one_key[k];
	for(int i = BEGINING; i< k; i++){
		if((input_data[i] != '\n') && (input_data[i] != '\r')){
			one_key[j++] =  input_data[i];
		}else{
			one_key[j++] = '\0';
			if(one_key[BEGINING] != '\0'){
				search_tree(tree, one_key, output_file);
			}
			memset(one_key,BEGINING,sizeof(one_key));
			j = BEGINING; 
		}
	}
    //free the space we used to store the test words 
    free(input_data);
    input_data = NULL;
};

// we search the coordinates in the 2d tree 
void search_tree(tree_t* tree, char* one_key, FILE* output_file){
	int times = BEGINING;
	printf("%s", one_key);
	//use the recursive to traverse the 2d tree
	recursive_print(output_file, tree -> root, one_key, &times);
    // printf search times 
    fprintf(output_file,"\n");
    printf( " --> ");
    printf("%d\n", times);
};

//use the recursive and print the result
void recursive_print(FILE* output_file, node_t* root, char* one_key, int* times){
	double xyr_for_serch[3];
	double x_for_serch;
	double y_for_serch;
	double threshold;
	char keep_one_key[strlen(one_key)];
	char* locate = NULL;
	int count = BEGINING;
	int exist = BEGINING;
	int switchs = TRUE;
	int i = BEGINING;
	int k = BEGINING;
	int s = BEGINING;
	//use a string to keep the one_key
	for(int j = 0; j< 3; j++){
		xyr_for_serch[j] = IMPOSSIBLE;
	}
	//seperate the x and y coordinates
	while(1){
		if(one_key[i] == ' ' || one_key[i] == '\0'){
			if(keep_one_key[0] != '\0' || keep_one_key[0] !=' '){
				keep_one_key[s] = '\0';
				xyr_for_serch[k++] = atof(keep_one_key);
			}
			if(one_key[i] == '\0'){
				break;
			}
			s = BEGINING;
			memset(keep_one_key,BEGINING,sizeof(keep_one_key));
			i++;
		} else {
			keep_one_key[s++] = one_key[i++];
		}
	}
	//assign the value to x,y,r separately 
	x_for_serch = xyr_for_serch[0];
	y_for_serch = xyr_for_serch[1];
	threshold = xyr_for_serch[2];
	// we just initialise distance once
	double distance = compare_distance (&(root -> x_for_tree), &(root -> y_for_tree), &(x_for_serch), &(y_for_serch));
	// ceate an arry to store the pointer of the nodes for nearest businesses 
	node_t* search_node = root;
	//if there is no no radius input
	if(threshold == IMPOSSIBLE){
		recursive_search_nearest(root, &search_node, &distance, x_for_serch, y_for_serch, times, switchs);
		output_tree(search_node, output_file, one_key);
	//there is radius input, but we can not find any place in the radius
	}else{ 
		recursive_search_in_radius(output_file, root, x_for_serch, y_for_serch, threshold, times, one_key, &exist, switchs);
		if(exist == BEGINING){
			fprintf(output_file,"%s", one_key);
			fprintf(output_file," --> ");
		    fprintf(output_file,"NOTFOUND");
		}
	}
};
/* This algorithm is provided by the instruction of the assignment 2.
//use the recursive to search the nerest node in 2d tree
void recursive_search_nearest( node_t* root, node_t** search_node, double* distance, double x_for_serch, double y_for_serch, int* times, int switchs){
	double temp;
	double zero = BEGINING;
	if(root){
	    if(switchs > BEGINING){
	    	switchs *= TURN_ON;
	    	//in the depth level which were calculated by the value of x, if y bigger than x, y may effect the result of the output  	
	    	if((root -> x_for_tree) <= (*distance)){
				recursive_search_nearest(root -> left, search_node, distance, x_for_serch, y_for_serch, times, switchs);
				recursive_search_nearest(root -> right, search_node, distance, x_for_serch, y_for_serch, times, switchs);
			} 
	    	else if(x_for_serch < (root -> x_for_tree)){
	    		recursive_search_nearest(root -> left, search_node, distance, x_for_serch, y_for_serch, times, switchs);
	    	}else{
	    		recursive_search_nearest(root -> right, search_node, distance, x_for_serch, y_for_serch, times, switchs);
	    	}
	    }else if(switchs == BEGINING){
	    	recursive_search_nearest(root -> left, search_node, distance, x_for_serch, y_for_serch, times, switchs);
			recursive_search_nearest(root -> right, search_node, distance, x_for_serch, y_for_serch, times, switchs);
		}else{
	    	switchs *= TURN_ON;
	    	//in the depth level which were calculated by the value of y, if x bigger than y, x may effect the result of the output  	
	    	if((root -> y_for_tree) <= (*distance)){
				recursive_search_nearest(root -> left, search_node, distance, x_for_serch, y_for_serch, times, switchs);
				recursive_search_nearest(root -> right, search_node, distance, x_for_serch, y_for_serch, times, switchs);
			} 
	       	else if(y_for_serch < (root -> y_for_tree)){
	    		recursive_search_nearest(root -> left, search_node, distance, x_for_serch, y_for_serch, times, switchs);
	    	}else{
	    		recursive_search_nearest(root -> right, search_node, distance, x_for_serch, y_for_serch, times, switchs);
	    	}
	    }
		temp = compare_distance (&(root -> x_for_tree), &(root -> y_for_tree), &(x_for_serch), &(y_for_serch));
		//if there is no radius input, we keep the nearest place
		(*times) ++;
		if((*distance) > temp){
			*distance = temp;
			*search_node = root;
		}
	}
};*/

//this algorithm was created by mayself which will increase some complexity of other coordinates to reduce the time complexity of 0,0. :)
//use the recursive to search the nerest node in 2d tree
void recursive_search_nearest( node_t* root, node_t** search_node, double* distance, double x_for_serch, double y_for_serch, int* times, int switchs){
	double temp;
	double zero = BEGINING;
	if(root){
		//Find the parents of the leaves
		if(switchs != BEGINING){
			if(root -> left  == NULL || root -> right== NULL){
				switchs = BEGINING;
			}else{
				if((root -> left -> left  == NULL) || (root -> left -> right  == NULL) || (root -> right -> left  == NULL)||(root -> right -> right  == NULL)){
					switchs = BEGINING;
				}
			}
		}
	    if(switchs > BEGINING){
	    	switchs *= TURN_ON;
	    	//in the depth level which were calculated by the value of x, if y bigger than x, y may effect the result of the output  	
	    	if(compare_distance (&(root -> x_for_tree), &zero, &(x_for_serch), &zero) < compare_distance (&zero, &(root -> y_for_tree), &zero, &(y_for_serch))){
				recursive_search_nearest(root -> left, search_node, distance, x_for_serch, y_for_serch, times, switchs);
				recursive_search_nearest(root -> right, search_node, distance, x_for_serch, y_for_serch, times, switchs);
			} 
	    	if(x_for_serch < (root -> x_for_tree)){
	    		recursive_search_nearest(root -> left, search_node, distance, x_for_serch, y_for_serch, times, switchs);
	    	}else{
	    		recursive_search_nearest(root -> right, search_node, distance, x_for_serch, y_for_serch, times, switchs);
	    	}
	    }else if(switchs == BEGINING){
	    	recursive_search_nearest(root -> left, search_node, distance, x_for_serch, y_for_serch, times, switchs);
			recursive_search_nearest(root -> right, search_node, distance, x_for_serch, y_for_serch, times, switchs);
		}else{
	    	switchs *= TURN_ON;
	    	//in the depth level which were calculated by the value of y, if x bigger than y, x may effect the result of the output  	
	    	if(compare_distance (&(root -> x_for_tree), &zero, &(x_for_serch), &zero) > compare_distance (&zero, &(root -> y_for_tree), &zero, &(y_for_serch))){
				recursive_search_nearest(root -> left, search_node, distance, x_for_serch, y_for_serch, times, switchs);
				recursive_search_nearest(root -> right, search_node, distance, x_for_serch, y_for_serch, times, switchs);
			} 
	       	if(y_for_serch < (root -> y_for_tree)){
	    		recursive_search_nearest(root -> left, search_node, distance, x_for_serch, y_for_serch, times, switchs);
	    	}else{
	    		recursive_search_nearest(root -> right, search_node, distance, x_for_serch, y_for_serch, times, switchs);
	    	}
	    }
		temp = compare_distance (&(root -> x_for_tree), &(root -> y_for_tree), &(x_for_serch), &(y_for_serch));
		//if there is no radius input, we keep the nearest place
		(*times) ++;
		if((*distance) > temp){
			*distance = temp;
			*search_node = root;
		}
	}
};

//use the recursive to search the nodes exsit in a specific threshold in 2d tree
void recursive_search_in_radius(FILE* output_file, node_t* root, double x_for_serch, double y_for_serch, 
	double threshold, int* times, char* one_key, int* exist, int switchs){
	double temp;
	double zero = BEGINING;
	if(root){
	    if(switchs > BEGINING){
	    	switchs *= TURN_ON;
	    	//Traverse any target may within the range of radius
	    	if(compare_distance (&(root -> x_for_tree), &zero, &(x_for_serch), &zero) <= threshold || 
	    		compare_distance (&zero, &(root -> y_for_tree), &zero, &(y_for_serch)) <= threshold){
	    		recursive_search_in_radius(output_file, root -> left, x_for_serch, y_for_serch, threshold, times, one_key, exist, switchs);
	    		recursive_search_in_radius(output_file, root -> right, x_for_serch, y_for_serch, threshold, times, one_key, exist, switchs);
	    	//otherwise, //we search the nodes in the same way that we inserted the nodes to decrease the time complexity
	    	}else if(x_for_serch < (root -> x_for_tree)){
	    		recursive_search_in_radius(output_file, root -> left, x_for_serch, y_for_serch, threshold, times, one_key, exist, switchs);
	    	}else{
	    		recursive_search_in_radius(output_file, root -> right, x_for_serch, y_for_serch, threshold, times, one_key, exist, switchs);
	    	}
	    }else{
	    	switchs *= TURN_ON;
	    	//Traverse any target may within the range of radius
	    	if(compare_distance (&(root -> x_for_tree), &zero, &(x_for_serch), &zero) <= threshold || 
	    		compare_distance (&zero, &(root -> y_for_tree), &zero, &(y_for_serch)) <= threshold){
	    		recursive_search_in_radius(output_file, root -> left, x_for_serch, y_for_serch, threshold, times, one_key, exist, switchs);
	    		recursive_search_in_radius(output_file, root -> right, x_for_serch, y_for_serch, threshold, times, one_key, exist, switchs);
	    	//we search the nodes in the same way that we inserted the nodes to decrease the time complexity
	    	}else if(y_for_serch < (root -> y_for_tree)){
	    		recursive_search_in_radius(output_file, root -> left, x_for_serch, y_for_serch, threshold, times, one_key, exist, switchs);
	    	}else{
	    		recursive_search_in_radius(output_file, root -> right, x_for_serch, y_for_serch, threshold, times, one_key, exist, switchs);
	    	}
	    }
		temp = compare_distance (&(root -> x_for_tree), &(root -> y_for_tree), &(x_for_serch), &(y_for_serch));
		//we print the all locations within the radius
		(*times) ++;
		if(temp <= threshold){
			(*exist) ++;
			output_tree(root, output_file, one_key);
		}
	}
};

//use the fprint to print the information in the output file.
void output_tree(node_t* curr, FILE* output_file, char* keep_one_key){
	//since we located all nodes and we do not use the foot any more, we use foot to traverse the linked list
	curr -> list_foot = curr;
	while(curr -> list_foot != NULL){
		fprintf(output_file,"%s", keep_one_key);
		fprintf(output_file," --> ");
	    fprintf(output_file,"Census year: %d || ", curr -> list_foot ->census_year);
		fprintf(output_file,"Block ID: %d || ", curr -> list_foot ->block_id);
		fprintf(output_file,"Property ID: %d || ", curr -> list_foot ->property_id);
		fprintf(output_file,"Base property ID: %d || ", curr -> list_foot ->base_property_id);
		fprintf(output_file,"CLUE small area: %s || ", curr -> list_foot ->clue_small_area);
		fprintf(output_file,"Trading Name: %s || " , curr -> list_foot ->trading_name);
		fprintf(output_file,"Industry (ANZSIC4) code: %d || ", curr -> list_foot ->industry_code);
		fprintf(output_file,"Industry (ANZSIC4) description: %s || ", curr -> list_foot ->industry_description);
		fprintf(output_file,"x coordinate: %0.5lf || ", curr -> list_foot ->x_coordinate);
		fprintf(output_file,"y coordinate: %0.5lf || ", curr -> list_foot ->y_coordinate);
		fprintf(output_file,"Location: (%s || \n", curr -> list_foot ->location);
		curr -> list_foot = curr -> list_foot -> next;
	}
};

//compare the distance between current node and the root
double compare_distance(void* node_x, void* node_y, void* root_x, void* root_y){
	double result;
	//comparing
	result = sqrt(pow(*(double*)(node_x) - *(double*)(root_x),2)+pow(*(double*)(node_y) - *(double*)(root_y),2));
	return result;
};

//free all memory space associated with the tree structure
void free_tree(tree_t* tree){
	assert(tree != NULL);
	free_nodes(tree -> root);
	free(tree);
	tree = NULL;
};

//free all nodes in the 2d tree
static void free_nodes(node_t* root){
	node_t* pre;
	if(root != NULL){
		free_nodes(root -> left);
		free_nodes(root -> right);
		//free the momeries that malloced for the properties of the nodes and point it to NULL to prevent the wild pointers
		while(root != NULL){
			free(root -> trading_name);
	       	root -> trading_name = NULL;
	        free(root -> clue_small_area);
	        root -> clue_small_area = NULL;
	        free(root -> industry_description);
	        root -> industry_description = NULL;
	        free(root -> location);
	        root -> location = NULL;
	        pre = root;
	        root = root -> next;
	        free(pre);
			pre = NULL;
		}
	}
};
