AVXFLAG := -mavx2

all: cversion avx

cversion: simple_conv_Cversion.cc
	g++ -o $@ $?
avx: simple_conv_AVXversion.cc
	g++ $(AVXFLAG) -o $@ $? 

clean:
	rm cversion avx
