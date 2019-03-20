#ifndef DISK_STORAGE_FACTORY
#define DISK_STORAGE_FACTORY

#include <list>
#include <map>
#include <vector>
#include <string>
#include <sstream>

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

#include <tile/matrixstorage.hpp>

#include <utils/singleton.hpp>
#include <utils/stringbuilder.hpp>

template<PixelStructure ps>
class DiskStorage;

template<PixelStructure ps>
class DiskStorageFactory
{
  struct Chunk
  {
    unsigned int cacheid;
    unsigned int file;
    unsigned char* chunk;
  };

  static unsigned int m_chunkidgen;

  std::list<Chunk> m_freeChunks;
  std::map<unsigned int, Chunk> m_allocatedChunks;

  unsigned int m_filesize;
  std::string m_filenamepattern;
  std::vector<std::pair<unsigned int, unsigned char*> > m_fileDescriptors;

  unsigned int m_size;

  void AddNewCacheFile();
public:
  DiskStorageFactory(const std::string& FilenamePattern,// = "cache",
		     unsigned int FileSize,// = 1000,
		     unsigned int Size);// = 100);
  virtual ~DiskStorageFactory();
  
  typename DiskStorage<ps>::Ptr AllocateStorage();
  typename DiskStorage<ps>::Ptr AllocateStorage(const MatrixStorage<ps>& init);
  void ReleaseStorage(unsigned int StorageID);
};

template<PixelStructure ps>
unsigned int DiskStorageFactory<ps>::m_chunkidgen(0);

#include "diskstorage.hpp"

template<PixelStructure ps>
void DiskStorageFactory<ps>::AddNewCacheFile()
{
  if(m_filesize < m_size*m_size*(ps+1))
    throw std::string("DiskStorageFactory::AddNewCacheFile()"
		      " - Filesize too small for the tile size requested.");
  unsigned int CacheFileNumber(m_fileDescriptors.size());
  int fd(open(StringBuilder(m_filenamepattern)
	      (CacheFileNumber)
	      (".cache").Str().c_str(),
		       O_RDWR|O_CREAT|O_TRUNC,
		       (mode_t)0600));

  if(fd < 0)
    throw std::string("DiskStorageFactory::AddNewCacheFile()"
		      " - Failed to allocate new disk space.");

  lseek(fd, m_filesize-1, SEEK_SET);
  write(fd,"",1);

  unsigned int ChunkNumber(m_filesize/(m_size*m_size*(ps+1)));
  unsigned char* data(reinterpret_cast<unsigned char*>(mmap(NULL,
							    m_filesize,
							    PROT_READ|PROT_WRITE,
							    MAP_SHARED,
							    fd,
							    0)));

  m_fileDescriptors.push_back(std::pair<unsigned int, unsigned char*>(fd,data));
  for(unsigned int n(0);n<ChunkNumber;++n)
    {
      Chunk c = {m_chunkidgen, CacheFileNumber, data+n*(m_size*m_size*(ps+1))};
      m_freeChunks.push_back(c);
      ++m_chunkidgen;
    }
  //std::cout << "Added " << ChunkNumber << " new chunks" << std::endl;
}

template<PixelStructure ps>
DiskStorageFactory<ps>::DiskStorageFactory(const std::string& FilenamePattern,
					  unsigned int FileSize,
					  unsigned int Size):m_filesize(FileSize),
							       m_filenamepattern(FilenamePattern),
							       m_size(Size)
{ AddNewCacheFile(); }

template<PixelStructure ps>
DiskStorageFactory<ps>::~DiskStorageFactory()
{
  if(!m_allocatedChunks.empty())
    throw std::string("DiskStorageFactory<ps>::~DiskStorageFactory()"
		      " - All the memory chunks have not been released.");
  for(unsigned int i(0);i<m_fileDescriptors.size();++i)
    {
      munmap(m_fileDescriptors[i].second, m_filesize);
      close(m_fileDescriptors[i].first);
    }
}
  
template<PixelStructure ps>
typename DiskStorage<ps>::Ptr DiskStorageFactory<ps>::AllocateStorage()
{
  if(m_freeChunks.empty())
    AddNewCacheFile();
  
  Chunk c(m_freeChunks.back());
  m_freeChunks.pop_back();
  m_allocatedChunks[c.cacheid] = c;
  
  return typename DiskStorage<ps>::Ptr(new DiskStorage<ps>(c.cacheid, c.chunk, m_size,  *this));
}

template<PixelStructure ps>
typename DiskStorage<ps>::Ptr DiskStorageFactory<ps>::AllocateStorage(const MatrixStorage<ps>& init)
{
  typename DiskStorage<ps>::Ptr p(AllocateStorage());
  for(unsigned int i(0);i<m_size;++i)
    for(unsigned int j(0);j<m_size;++j)
      p->setPixel(i,j,init.getPixel(i,j));

  return p;
}

template<PixelStructure ps>
void DiskStorageFactory<ps>::ReleaseStorage(unsigned int StorageID)
{
  typename std::map<unsigned int, Chunk>::iterator c(m_allocatedChunks.find(StorageID));
  if(c==m_allocatedChunks.end())
    throw std::string("DiskStorageFactory<ps>::ReleaseStorage()"
		      " - Request for releasing a non allocated storage.");
  m_freeChunks.push_back(c->second);
  m_allocatedChunks.erase(c);
}

#endif
