/*
 Copyright 2001 The University of Texas at Austin

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <algorithm>
#include <vector>
#include "nnode.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <math.h>

namespace NEAT {

	// ----------------------------------------------------------------------- 
	// A NETWORK is a LIST of input NODEs and a LIST of output NODEs           
	//   The point of the network is to define a single entity which can evolve
	//   or learn on its own, even though it may be part of a larger framework 
	class Network {

	public:

		int numnodes; // The number of nodes in the net (-1 means not yet counted)
		int numlinks; //The number of links in the net (-1 means not yet counted)

		std::vector<NNode*> all_nodes;  // A list of all the nodes

		void destroy();  // Kills all nodes and links within
		void destroy_helper(NNode *curnode,std::vector<NNode*> &seenlist); // helper for above

		void nodecounthelper(NNode *curnode,int &counter,std::vector<NNode*> &seenlist);
		void linkcounthelper(NNode *curnode,int &counter,std::vector<NNode*> &seenlist);

	public:

		char *name; // Every Network or subNetwork can have a name
		std::vector<NNode*> inputs;  // NNodes that input into the network
		std::vector<NNode*> outputs; // Values output by the network

		int net_id; // Allow for a network id

		Network(std::vector<NNode*> in,std::vector<NNode*> out,std::vector<NNode*> all,int netid);

		// Copy Constructor
		Network(const Network& network);

		~Network();

		// Puts the network back into an inactive state
		void flush();
		
		// Verify flushedness for debugging
		void flush_check();

		// Activates the net such that all outputs are active
		bool activate();

		// SIGMOID FUNCTION ********************************
		// This is a signmoidal activation function, which is an S-shaped squashing function
		// It smoothly limits the amplitude of the output of a neuron to between 0 and 1
		// It is a helper to the neural-activation function get_active_out
		// It is made inline so it can execute quickly since it is at every non-sensor
		// node in a network.
		// NOTE:  In order to make node insertion in the middle of a link possible,
		// the signmoid can be shifted to the right and more steeply sloped:
		// slope=4.924273
		// constant= 2.4621365
		// These parameters optimize mean squared error between the old output,
		// and an output of a node inserted in the middle of a link between
		// the old output and some other node.
		// When not right-shifted, the steepened slope is closest to a linear
		// ascent as possible between -0.5 and 0.5
		double fsigmoid(double,double,double);

		// Prints the values of its outputs
		void show_activation();

		void show_input();

		// Add a new input node
		void add_input(NNode*);

		// Add a new output node
		void add_output(NNode*);

		// Takes an array of sensor values and loads it into SENSOR inputs ONLY
		void load_sensors(double*);
		void load_sensors(const std::vector<float> &sensvals);

		// Name the network
		void give_name(char*);

		// Counts the number of nodes in the net if not yet counted
		int nodecount();

		// Counts the number of links in the net if not yet counted
		int linkcount();

		// This checks a POTENTIAL link between a potential in_node
		// and potential out_node to see if it must be recurrent 
		// Use count and thresh to jump out in the case of an infinite loop 
		bool is_recur(NNode *potin_node,NNode *potout_node,int &count,int thresh);

		// If all output are not active then return true
		bool outputsoff();

		int max_depth();


	};

} // namespace NEAT

#endif
