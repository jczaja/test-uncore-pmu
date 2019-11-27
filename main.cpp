#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>


int main()
{
  const int sized = 1000;
   float* buffer = nullptr;
   float result = 0.0f;
  uint32_t* mapped_space= nullptr;
    char errmsg[1000];

  int ret = posix_memalign((void**)&buffer,64,sized*sizeof(float));
  if (ret != 0) {
    std::cout << "Allocation error of bottom!" << std::endl;
    exit(-1);
  }
  // Init with some random data
  for(unsigned int i=0; i< (unsigned int) sized; ++i) {
      buffer[i] = i%13;
  }

  // GET counter
  const uint32_t bar = 0xfed10001; // Base Adress Register for:  Intel(R) Core(TM) i5-8350U CPU @ 1.70GHz   

  int fd = open("/dev/mem",O_RDONLY );
  if(fd == -1) {
    perror(errmsg);
    std::cerr<< "Error mapping PMU: " <<  errmsg << std::endl;
    throw std::string("Could not open /dev/mem file"); 
  }

  size_t pagesize = getpagesize();

  mapped_space = (uint32_t*)mmap(0, pagesize  , PROT_READ , MAP_PRIVATE, fd, bar);

  if(mapped_space == (void*)-1)
    perror(errmsg);
    std::cerr<< "Error mapping PMU: " <<  errmsg << std::endl;
    
    throw std::string("Could not map PMU register space"); 

  // Code to measure
  for(unsigned int i = 0; i< sized; ++i) {
    result += buffer[i];
  }


  munmap(mapped_space, 5140);

}
