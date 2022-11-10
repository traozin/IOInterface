lib = libs/display
$(main): $(main).c $(lib).o
        gcc -o $(main) $(main).c $(lib).o $(others)
$(lib).o: $(lib).s
        as -g -o $(lib).o $(lib).s