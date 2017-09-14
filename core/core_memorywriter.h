#ifndef MEMORYWRITER_H_

#define MEMORYWRITER_H_



#include <windows.h>

#include <string>

#include <random>



class MemoryWriter {



public:



  MemoryWriter(const std::wstring& name, size_t size);

  std::string createRandomData() const;

  void write(const std::string& data);



private:



  char getRandomCharacter() const;

  void createSharedMemory();



private:



  std::wstring m_memoryName;

  size_t m_memorySize = 0;

  HANDLE m_shmHandler = 0;

};

#endif // !MEMORYWRITER_H_