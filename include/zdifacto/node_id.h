
#ifndef ZDIFACTO_NODE_ID_H_
#define ZDIFACTO_NODE_ID_H_
namespace zdifacto {

class NodeID {
 public:
  static const int kScheduler = 1;
  static const int kServerGroup = 2;
  static const int kWorkerGroup = 4;

  static int Encode(int group, int rank) {
    return group + (rank+1) * 8;
  }

  static int GetGroup(int id) {
    return (id % 8);
  }

};
} 

#endif 
