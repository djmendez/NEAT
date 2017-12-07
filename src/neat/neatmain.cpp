#include <iostream>
#include "nnode.h"
#include "network.h"
#include "link.h"

using namespace NEAT;


struct conn {
    int from;
    int to;
    double weight;
};

Network *generateNetwork(int nodes[], int ncount, conn links[], int linkcount) {

    //Inputs and outputs will be collected here for the network
    //All nodes are collected in an all_list-
    //this will be used for later safe destruction of the net
    std::vector<NNode *> inlist;
    std::vector<NNode *> outlist;
    std::vector<NNode *> all_list;

    //////////////////create node
    for (int nc = 0; nc < ncount; nc++) {
        if (nodes[nc] == 1) {
            NNode *newnode = new NNode(SENSOR, nc, INPUT);
            inlist.push_back(newnode);
            all_list.push_back(newnode);
        } else if (nodes[nc] == 2) {
            NNode *newnode = new NNode(NEURON, nc, OUTPUT);
            outlist.push_back(newnode);
            all_list.push_back(newnode);
        }
    }


    for (int nl = 0; nl < linkcount; nl++) {
        Link *newlink = new Link(links[nl].weight, all_list.at(links[nl].from), all_list.at(links[nl].to), false);
        //std::cout<<links[nl].from<<"->"<<links[nl].to<<links[nl].weight<<std::endl;
        all_list.at(links[nl].from)->outgoing.push_back(newlink);
        all_list.at(links[nl].to)->incoming.push_back(newlink);
    }
    /////////////////

    //Create the new network
    //The new network
    Network *newnet = new Network(inlist, outlist, all_list, 1);

    newnet->maxweight = 1; //absolute magnitude for adaptationi purpose

    return newnet;
}


bool evaluate(Network *net, double in[], double out[]) {

    bool success;  //Check for successful activation

    int net_depth = 2; //The max depth of the network to be activated


    net_depth = net->max_depth();
    net->load_sensors(in);

    //Relax net and get output
    success = net->activate();

    //use depth to ensure relaxation
    for (int relax = 0; relax <= net_depth; relax++) {
        success = net->activate();
    }

    if (!success) {
        return false;
    }


    int count = 0;
    for (auto curnode: net->outputs) {
        out[count] = (curnode)->activation;
        count++;
    }

    net->flush();


    return true;
}


int mainula() {

    double in[4][3] = {{0.0, 0.0, 1.0},
                       {0.0, 1.0, 1.0},
                       {1.0, 0.0, 1.0},
                       {1.0, 1.0, 1.0}};


    int nodes[]{1, 1, 1, 2, 2};
    conn links[]{conn{0, 3, 1.0}, conn{2, 3, 1.0}, conn{1, 4, 1.0}, conn{2, 4, -1.0}};

    double out[2];

    Network *n = generateNetwork(nodes, 5, links, 4);
    std::cout<<n->linkcount()<<","<<n->nodecount()<<std::endl;


    for (int j = 0; j < 4; j++) {
        evaluate(n, in[j], out);

        for (int i = 0; i < 2; i++) {
            if (out[i] < 0.0001) {
                std::cout << 0 << ",";
            } else {
                std::cout << out[i] << ",";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}

