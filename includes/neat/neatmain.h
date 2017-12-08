/*
 * neatmain.h
 *
 *  Created on: Dec 7, 2017
 *      Author: avs
 */

#ifndef INCLUDES_NEAT_NEATMAIN_H_
#define INCLUDES_NEAT_NEATMAIN_H_

#include <iostream>
#include "network.h"
#include <neatmain.h>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

struct Conn {
    int from;
    int to;
    double weight;
};


NEAT::Network *generateNetwork(std::vector<std::vector<int>> nodes, std::vector<Conn> links);

bool activate(NEAT::Network *net, double in[], double out[]);

bool hasLink(NEAT::Link *link, const std::vector<Conn> &vec);

void exporttofile(char *filename, NEAT::Network *net);

std::pair<bool,NEAT::Network*> importformfile(const char *filename);



#endif /* INCLUDES_NEAT_NEATMAIN_H_ */
