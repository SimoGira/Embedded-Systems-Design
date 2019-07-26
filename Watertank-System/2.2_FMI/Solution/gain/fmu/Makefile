FMUNAME=gain
all: createFMU

createFMU: 
	cp ../cpp/lib/gain.so binaries/linux64/
	zip -r ../$(FMUNAME).fmu binaries/linux64 modelDescription.xml

clean:
	rm ../$(FMUNAME).fmu
