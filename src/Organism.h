#ifndef ORGANISM_H
#define ORGNAISM_H

#include "Triangle.h"

#include <vector>

using namespace std;

static uint32_t organismID = 0;
static uint32_t getOrganismID() { return organismID++; };

struct Organism
{
  vector<Triangle> genome;
  float fitness = 0.0;
  uint32_t id = getOrganismID();
};

#endif
