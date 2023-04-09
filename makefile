all : toolkit.x mytimeout.x

toolkit.x : toolkit.c
	gcc toolkit.c -o toolkit.x
	
mytimeout.x : mytimeout.c
	gcc mytimeout.c -o mytimeout.x

sample_run :
	./toolkit.x < testcase.tc
	
clean: 
	rm mytimeout.x toolkit.x
