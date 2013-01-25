

//World stuff
	#define WORLD_SIZE     4
	#define WORLD_AREA     WORLD_SIZE * WORLD_SIZE
	#define WORLD_VOLUME   WORLD_AREA * WORLD_SIZE
	#define WORLD_TOTAL_SIZE   WORLD_SIZE * CHUNK_TOTAL_SIZE
	#define getWorldIndex(x,y,z) ((x) + (y) * WORLD_SIZE + (z) * WORLD_AREA)
	
//Chunk stuff
	#define CHUNK_SIZE 	 8
	#define CHUNK_AREA 	 CHUNK_SIZE * CHUNK_SIZE
	#define CHUNK_VOLUME CHUNK_SIZE_AREA  * CHUNK_SIZE
	#define CHUNK_TOTAL_SIZE CHUNK_SIZE * BLOCK_SIZE

//Block stuff
	#define BLOCK_SIZE 1
	
//Vector
	extern guVector UP_VECTOR;
	extern guVector DOWN_VECTOR;
