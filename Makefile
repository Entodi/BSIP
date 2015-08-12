CC := g++
CFLAGS := -c -Wall
LDFLAGS :=
SOURCES := main.cpp adaboost.cpp configuration.cpp feature.cpp image.cpp sample.cpp samples_handler.cpp strong_classifier.cpp training_module.cpp
OBJDIR := source
OBJECTS := $(addprefix $(OBJDIR)/, $(SOURCES))
EXECUTABLE=Boosting-Sex-Identification-Performance

all: $(EXECUTABLE)
    																																																																																																									
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
