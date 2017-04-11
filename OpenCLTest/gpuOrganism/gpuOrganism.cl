#ifdef STRINGIFY
STRINGIFY(
#endif
          
#define MAX_CELLS 200
          
typedef struct gpuPoint {
  float x;
  float y;
} gpuPoint;
  
typedef struct gpuCell {
  gpuPoint pos;
  gpuPoint velocity;
  gpuPoint expectedPos;
} gpuCell;

typedef struct gpuOrganism {
  int numCells;
  gpuCell cells[200];
} gpuOrganism;

// This is a test. This is only a test.
kernel void processPhysics(global gpuOrganism *gpuOrganism) {
  int gid = get_global_id(0);
  
  gpuOrganism = &gpuOrganism[gid];

  int numCells = gpuOrganism->numCells;
  for (int loop = 0; loop < 100; loop++) {
    global gpuCell * cells = gpuOrganism->cells;
    for (int i = 0; i < numCells; i++) {
      cells[i].pos.x += cells[i].velocity.x;
      cells[i].pos.y += cells[i].velocity.y;
      
      if (loop > 0) {
        cells[i].pos.x -= cells[i].velocity.x;
        cells[i].pos.y -= cells[i].velocity.y;
      }
    }
  }
}
 
#ifdef STRINGIFY
)
#endif