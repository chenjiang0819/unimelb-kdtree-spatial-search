map2: main.o two_d_tree.o processingstring.o 
	gcc -o map2 main.o two_d_tree.o processingstring.o -lm -g

main.o: main.c two_d_tree.h processingstring.h
	gcc -c  main.c two_d_tree.h processingstring.h -lm -g

two_d_tree.o: two_d_tree.c two_d_tree.h
	gcc -c  two_d_tree.c two_d_tree.h -lm -g

processingstring.o: processingstring.c processingstring.h
	gcc -c  processingstring.c processingstring.h -lm -g