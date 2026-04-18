LDLIBS=-liio

get_samples : get_samples.o ad4080.o
	$(CC) $^ $(LDLIBS) -o $@
	
binary_samples : binary_samples.o ad4080.o
	$(CC) $^ $(LDLIBS) -o $@
