#include "core_common.h"

#include <random>

#include <iostream>



///////////////////////////////////////////////////////////////////////////////

// USING SECTION                                                             //

///////////////////////////////////////////////////////////////////////////////



using std::string;



///////////////////////////////////////////////////////////////////////////////

// CONSTANTS SECTION                                                         //

///////////////////////////////////////////////////////////////////////////////



const char MinCharacter{ 'A' };

const char MaxCharacter{ 'z' };



///////////////////////////////////////////////////////////////////////////////

// PUBLIC SECTION                                                            //

///////////////////////////////////////////////////////////////////////////////



MemoryWriter::MemoryWriter(const std::wstring& name, size_t size) :

m_memoryName(name),

m_memorySize(size) {

  createSharedMemory();

}



string MemoryWriter::createRandomData() const {

  string data;

  for (size_t i = 0; i < m_memorySize; i++) {

    data += getRandomCharacter();

  }

  return data;

}



void MemoryWriter::write(const string& data) {

  if (!m_shmHandler) {

    return;

  }

  auto buffer = MapViewOfFile(m_shmHandler, FILE_MAP_ALL_ACCESS, 0, 0, m_memorySize);

  if (NULL == buffer) {

    std::cerr << "Cannot use MapViewOfFile: null buffer." << std::endl;

    return;

  }

  CopyMemory(buffer, data.c_str(), data.size());

}



//////////////////////////////////////////////////////////////////////////////

// PRIVATE SECTION                                                          //

//////////////////////////////////////////////////////////////////////////////



char MemoryWriter::getRandomCharacter() const {

  return MinCharacter + rand() % 24;

}



void MemoryWriter::createSharedMemory() {

	m_shmHandler = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, (LPTSTR) m_memoryName.c_str());



  if (!m_shmHandler) {

    m_shmHandler = CreateFileMapping(

      INVALID_HANDLE_VALUE,

      NULL,

      PAGE_READWRITE,

      0,

      m_memorySize,

	  (LPTSTR) m_memoryName.c_str());

  }

}
