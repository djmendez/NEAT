#include <neatmain.h>

using namespace std;
using namespace NEAT;

#include <link.h>
#include <nnode.h>
#include <map>

Network *generateNetwork(vector<vector<int>> nodes, vector<Conn> links) {

    //Inputs and outputs will be collected here for the network
    //All nodes are collected in an all_list-
    //this will be used for later safe destruction of the net
    std::vector<NNode *> inlist;
    std::vector<NNode *> outlist;
    std::vector<NNode *> all_list;

    map<int, NNode*> idNodeMap;

    //////////////////create node
    for (int nc = 0; nc < nodes.size(); nc++) {
        if (nodes[nc][1] == 1) {
            NNode *newnode = new NNode(SENSOR, nc, INPUT);
            inlist.push_back(newnode);
            all_list.push_back(newnode);
            idNodeMap[nodes[nc][0]] = newnode;
        } else if (nodes[nc][1] == 2) {
            NNode *newnode = new NNode(NEURON, nc, OUTPUT);
            outlist.push_back(newnode);
            all_list.push_back(newnode);
            idNodeMap[nodes[nc][0]] = newnode;
        } else {
            NNode *newnode = new NNode(NEURON, nc, HIDDEN);
            all_list.push_back(newnode);
            idNodeMap[nodes[nc][0]] = newnode;
        }
    }


    for (int nl = 0; nl < links.size(); nl++) {
           Link *newlink = new Link(links[nl].weight, idNodeMap[links[nl].from], idNodeMap[links[nl].to], false);
           //std::cout<<links[nl].from<<"->"<<links[nl].to<<links[nl].weight<<std::endl;
           idNodeMap[links[nl].from]->outgoing.push_back(newlink);
           idNodeMap[links[nl].to]->incoming.push_back(newlink);
       }
    /////////////////

    //Create the new network
    //The new network
    Network *newnet = new Network(inlist, outlist, all_list, 1);

    return newnet;
}


bool activate(Network *net, double in[], double out[]) {
    net->load_sensors(in);

    //Relax net and get output
    bool success = net->activate();//Check for successful activation

    //use depth to ensure relaxation
    int net_depth = net->max_depth();
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

    //net->flush();
    return true;
}

bool hasLink(Link *link, const std::vector<Conn> &vec) {
    for (auto con: vec) {
        if (link->in_node->node_id == con.from && link->out_node->node_id == con.to) {
            return true;
        }
    }
    return false;
}

void exporttofile(char *filename, Network *net) {
    std::ofstream outFile(filename);

    std::vector<NNode *>::iterator curnode;

    std::vector<Conn> links;

    outFile << net->all_nodes.size() << std::endl;

    for (auto nodes: net->all_nodes) {
        outFile << nodes->node_id << " " << nodes->placement << std::endl;

        for (auto inLinks : nodes->incoming) {
            if (!hasLink(inLinks, links)) {
                links.push_back(Conn{inLinks->in_node->node_id, inLinks->out_node->node_id, inLinks->weight});
            }
        }
        for (auto outLinks : nodes->incoming) {
            if (!hasLink(outLinks, links)) {
                links.push_back(Conn{outLinks->in_node->node_id, outLinks->out_node->node_id, outLinks->weight});
            }
        }
    }

    outFile << links.size() << std::endl;
    for (auto lnk: links) {
        outFile << lnk.from << " " << lnk.to << " " << lnk.weight << std::endl;
    }
    outFile.close();
}


pair<bool,Network*> importformfile(const char *filename) {
    ifstream infile(filename, ios::in);
    vector<vector<int>> nodes;
    vector<Conn> links;

    std::string line;
    int a;

    if (!std::getline(infile, line)) {
        return {false, nullptr};
    }

    std::istringstream iss(line);
    iss >> a;

    int b, c;
    double d;
    for (int i = 0; i < a; i++) {
        if (!std::getline(infile, line)) {
            return {false, nullptr};
        }
        std::istringstream iss(line);
        iss >> b;
        iss >> c;
        nodes.push_back(vector<int>{b, c});
    }

    if (!std::getline(infile, line)) {
        return {false, nullptr};
    }

    std::istringstream is2(line);
    is2 >> a;

    for (int i = 0; i < a; i++) {

        if (!std::getline(infile, line)) {
            return {false, nullptr};
        }
        std::istringstream iss(line);
        iss >> b;
        iss >> c;
        iss >> d;
        links.push_back(Conn{b, c, d});
    }
    Network *net=  generateNetwork(nodes, links);

    for (auto n :nodes){
        std::cout << n[0] << "," << n[1] << std::endl;
    }

    for (auto l :links){
        std::cout << l.from << " " << l.to << " " << l.weight << std::endl;
    }

    infile.close();
    return {true, net};
}

int mymain(int argc, char *argv[]) {

    double in[4][3] = {{1.0, 0.0, 0.0},
                       {1.0, 0.0, 1.0},
                       {1.0, 1.0, 0.0},
                       {1.0, 1.0, 1.0}};

    ////////////0, 1, 2, 3, 4, 5
    vector<vector<int>> nodes{vector<int>{0, 1}, vector<int>{1, 1}, vector<int>{2, 1}, vector<int>{3, 2}, vector<int>{4, 0}, vector<int>{5, 0},
                              vector<int>{6, 0}};

    vector<Conn> conns{Conn{0, 3, -6.14299},
                       Conn{2, 3, 5.49373},
                       Conn{1, 4, 4.68038},
                       Conn{4, 3, 8},
                       Conn{2, 4, -5.75357},
                       Conn{2, 5, -3.7923},
                       Conn{5, 4, -3.03612},
                       Conn{1, 6, -1.32465},
                       Conn{6, 3, 2.47735},};

//    Network *net = generateNetwork(nodes, conns);

    const pair<bool, Network *> &p = importformfile("test");
    if (!p.first) {
        return -1;
    }
    Network *net = p.second;

    double out[1];
    net->flush();

    for (int i = 0; i < 4; i++) {
        activate(net, in[i], out);
        cout << (out[0] > 0.5 ? 1 : 0) << "," << out[0] << endl;
    }


    return 0;
}
