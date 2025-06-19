#ifndef __SET_H
#define __SET_H

#define N 30000

typedef struct nodeSet_s {
  int   size;
  int   set[N];
} nodeSet;

/* Initialize the nodeSet                                             */
void initNodeSet(nodeSet *S);

/* Add a new node to the set of nodes                                 */
void addNodetoNodeSet(int nodeId, nodeSet *S);

/* Print the set of nodes (for debug purposes only)                   */
void printNodeSet(nodeSet *S);

/* Function to create a copy of the node set                          */
void copyNodeSet(nodeSet *dest, nodeSet *src);

/* If nodeId is a member of the given set, return TRUE; else FALSE    */
bool isSetMember(int nodeId, nodeSet *S);

/* Function that returns TRUE if two sets are equal; FALSE otherwise  */
bool setEqual(nodeSet *S1, nodeSet *S2);

/* Function that returns TRUE if set S2 is a subset of set S1;        */
/* FALSE otherwise                                                    */
bool setSubset(nodeSet *S1, nodeSet *S2);

/* Function to delete a given node from the given set of nodes        */
void delNodefromNodeSet(int nodeId, nodeSet *from);

/* Function for union of two sets; result is stored in first argument */
void setUnion(nodeSet *V1, nodeSet *V2);

/* Function to calculate the intersection of two node sets            */
void setIntersect(nodeSet *V1, nodeSet *V2, nodeSet *out);

/* V1 - V2 with the result in V1 */
void setDifference(nodeSet *V1, nodeSet *V2);

/* Function for difference of two sets; result is stored in V3        */
void setDiff(nodeSet *V1, nodeSet *V2, nodeSet *V3);


void initNodeSet(nodeSet *S)
{
  S->size = 0;
  memset(S->set, 0, sizeof(int) * N);

  return;
}


/* Function to determine membership of a node in a given Set */
bool isSetMember(int nodeId, nodeSet *S)
{
  int       i;

  for (i = 0; i < S->size; i++) {
    if (nodeId == S->set[i])
      return true;
  }

  return false;
}

/* Add given node ID to the set of nodes */
void addNodetoNodeSet(int nodeId, nodeSet *S)
{
  if (!isSetMember(nodeId, S)) {
    S->set[S->size] = nodeId;
    (S->size)++;
  }

  return;
}

/* Function to make a copy of a set of nodes */
void copyNodeSet(nodeSet *dest, nodeSet *src)
{
  dest->size = src->size;
  memcpy(dest->set, src->set, N);

  return;
}

/* Function to return TRUE if two sets are equal; FALSE otherwise */
bool setEqual(nodeSet *S1, nodeSet *S2)
{
  int         i, j;
  bool     found;

  if (S1->size != S2->size) return false;

  for (i = 0; i < S1->size; i++) {
    found = false;
    for (j = 0; j < S2->size; j++) {
      if (S1->set[i] == S2->set[j]) {
        found = true;
        break;
      }
    }
    if (!found) return false;
  }

  return true;
}

/* Function that returns TRUE if set S2 is a subset of set S1;        */
/* FALSE otherwise                                                    */
bool setSubset(nodeSet *S1, nodeSet *S2)
{
  int         i, j;
  bool     found = false;

  for (i = 0; i < S2->size; i++) {
    found = false;
    for (j = 0; j < S1->size; j++) {
      if (S2->set[i] == S1->set[j]) {
        found = true;
        break;
      }
    }
    if (!found) return false;
  }

  return(found);
}

/* Function to delete a node from a given set                    */
/* Requires moving all the remaining nodes beyond the given node */
void delNodefromNodeSet(int nodeId, nodeSet *from)
{
  int       i;

  for (i = 0; i < from->size; i++) {
    if (nodeId == from->set[i])
      break;
  }

  if (i != (from->size - 1)) {
    for (; i < (from->size - 1); i++)
      from->set[i] = from->set[i+1];
  }
  from->size--;

  return;
}

/* Function for union of two sets. The union is output in first argument */
void setUnion(nodeSet *V1, nodeSet *V2)
{
  int         i;

  for (i = 0; i < V2->size; i++) {
    addNodetoNodeSet(V2->set[i], V1);
  }

  return;
}

void setIntersect(nodeSet *V1, nodeSet *V2, nodeSet *out)
{
  int         i;

  for (i = 0; i < V1->size; i++) {
    if (isSetMember(V1->set[i], V2)) {
      addNodetoNodeSet(V1->set[i], out);
    }
  }

  return;
}


/* Function to return V1-V2; result is stored in V3                    */
void setDiff(nodeSet *V1, nodeSet *V2, nodeSet *V3)
{
  int     i;

  for (i = 0; i < V1->size; i++) {
    if (!isSetMember(V1->set[i], V2)) {
      addNodetoNodeSet(V1->set[i], V3);
    }
  }
}

/* V1 - V2 with the result in V1 */
void setDifference(nodeSet *V1, nodeSet *V2)
{
  int           i;
  bool       member = true;

  while (member) {
    for (i = 0; i < V1->size; i++) {
      if (isSetMember(V1->set[i], V2)) {
        delNodefromNodeSet(V1->set[i], V1);
        break;
      }
    }
    if (i == V1->size)
      member = false;
  }

  return;
}


/* Function to print the nodes of a given set */
void printNodeSet(nodeSet *S)
{
  int         i;

  for (i = 0; i < S->size; i++)
    printf("%d\t", S->set[i]);
  printf("\n");

  return;
}



#endif /* __SET_H */
