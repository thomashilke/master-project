#include <vector>
#include <cstdlib>

#include <renderer/renderer.hpp>
#include <tile/tile.hpp>

int main()
{
  const unsigned int TileSize(128);
  
  BasicRenderer br(500,500,true);
  DiskStorageFactory<PIX_L8> Df("cache", 40000, TileSize);
  VideoStorageFactory<PIX_L8> Vf(br, TileSize);
  
  {
    unsigned int N(20);
    
    std::vector<Tile<PIX_L8>::Ptr> tiles(N);
    for(unsigned int k(0);k<N;++k)
      {
	tiles[k] = new Tile<PIX_L8>(TileSize, Df, Vf);
	tiles[k]->setStorage(Tile<PIX_L8>::STORAGE_CENTRAL);
	tiles[k]->setStorage(Tile<PIX_L8>::STORAGE_DISK);
      }
    
    for(unsigned int k(0);k<N;++k)
      {
	for(unsigned int i(0);i<TileSize;++i)
	  for(unsigned int j(0);j<TileSize;++j)
	    tiles[k]->setPixel(i,j,Color(65+('z'-'a')/10.*(rand()%10), 0,0,255.));
	tiles[k]->Sync();
	}
  }
  
  return 0;
}
