NVFLAGS  := -std=c++11

CXXFLAGS := -fopenmp

LDFLAGS  := -lm

MPILIBS  := -I/opt/intel/compilers_and_libraries_2017.3.191/linux/mpi/intel64/include -L/opt/intel/compilers_and_libraries_2017.3.191/linux/mpi/intel64/lib -lmpi

EXES     := APSP_cuda.cu APSP_multiGPU_MPI.cu APSP_multiGPU_OpenMP.cu

alls: $(EXES)

clean:
	rm -f $(EXES)

APSP_cuda: APSP_cuda.cu
	nvcc $(NVFLAGS) -Xcompiler="$(CXXFLAGS)" $(LDFLAGS) -o $@ $?

APSP_multiGPU_OpenMP: APSP_multiGPU_OpenMP.cu
	nvcc $(NVFLAGS) -Xcompiler="$(CXXFLAGS)" $(LDFLAGS) -o $@ $?

APSP_multiGPU_MPI: APSP_multiGPU_MPI.cu
	nvcc `$(NVFLAGS) $(MPILIBS) -Xcompiler="$(CXXFLAGS)" -o $@ $?
