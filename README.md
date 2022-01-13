# MelbourneCensusDataset-ImplementingMapFunctionsWithKDTrees  

Makefile will produce an executable program called map2. This program should take two command line arguments:  
(1) the name of the data file used to build the tree, and  
(2) the name of anoutput file.
  
Input of map2 program should:  
• map2 datafile outputfile then type in queries; or  
• map2 datafile outputfile < queryfile  
Queries should be entered as x,y,radius triplets separated by spaces: x y r  
Example output  
This is an example of what might be output to the file after two queries:  
144.967058 -37.817313 0.0005 −− > Census year: 2018 || Block ID: 15 || Property ID: 109260 
|| Base property ID: 109260 || CLUE small area: Melbourne (CBD) || Trading Name: Hungry Jack’s Pty Ltd 
|| Industry (ANZSIC4) code: 4511 || Industry (ANZSIC4) description: Cafes and Restaurants || x coordinate: 
144.9668 || y coordinate: -37.8171 || Location: (-37.81711586, 144.9668418) ||   
144.967058 -37.817313 0.0005 −− > Census year: 2018 || Block ID: 15 || Property ID: 109258 
|| Base property ID: 109258 || CLUE small area: Melbourne (CBD) || Trading Name: McDonalds || Industry 
(ANZSIC4) code: 4511 || Industry (ANZSIC4) description: Cafes and Restaurants || x coordinate: 144.9669 
|| y coordinate: -37.8172 || Location: (-37.81724484, 144.9669126) ||  
144.967058 -37.817313 0.0005 −− > Census year: 2018 || Block ID: 15 || Property ID: 104015 
|| Base property ID: 104015 || CLUE small area: Melbourne (CBD) || Trading Name: Dangerfield || Industry 
(ANZSIC4) code: 4251 || Industry (ANZSIC4) description: Clothing Retailing || x coordinate: 144.9668 
|| y coordinate: -37.8174 || Location: (-37.81741866, 144.9668092) ||  
144.967058 -37.817313 0.0005 −− > Census year: 2018 || Block ID: 15 || Property ID: 109257 
|| Base property ID: 109257 || CLUE small area: Melbourne (CBD) || Trading Name: Young & Jacksons || 
Industry (ANZSIC4) code: 4520 || Industry (ANZSIC4) description: Pubs, Taverns and Bars || x coordinate: 
144.967 || y coordinate: -37.8174 || Location: (-37.81735593, 144.967023) ||  
144.967058 -37.817313 0.0005 −− > Census year: 2018 || Block ID: 15 || Property ID: 109259 
|| Base property ID: 109259 || CLUE small area: Melbourne (CBD) || Trading Name: Souvenir Collection 
|| Industry (ANZSIC4) code: 4310 || Industry (ANZSIC4) description: Non-Store Retailing || x coordinate: 
144.9669 || y coordinate: -37.8172 || Location: (-37.8171602, 144.9668989) ||  
144.963089 -37.799092 0.0005 −− > NOTFOUND  
This is an example of what might be output to stdout:  
144.967058 -37.817313 0.0005 −− > 678  
144.963089 -37.799092 0.0005 −− > 1356  
