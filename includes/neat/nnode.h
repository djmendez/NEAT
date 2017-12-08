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
#ifndef _NNODE_H_
#define _NNODE_H_

#include <algorithm>
#include <vector>
#include "link.h"
#include <iostream>
#include <sstream>

namespace NEAT {

	enum nodetype {
		NEURON = 0,
		SENSOR = 1
	};

	enum nodeplace {
		HIDDEN = 0,
		INPUT = 1,
		OUTPUT = 2,
	};

	enum functype {
		SIGMOID = 0
	};

	class Link;
	class Network;

	// ----------------------------------------------------------------------- 
	// A NODE is either a NEURON or a SENSOR.  
	//   - If it's a sensor, it can be loaded with a value for output
	//   - If it's a neuron, it has a list of its incoming input signals (List<Link> is used) 
	// Use an activation count to avoid flushing
	class NNode {
		friend class Network;

	protected:
		int activation_count;  // keeps track of which activation the node is currently in
		double last_activation; // Holds the previous step's activation for recurrency

	public:
		functype ftype; // type is either SIGMOID ..or others that can be added
		nodetype type; // type is either NEURON or SENSOR 
		nodeplace placement;

		double activesum;  // The incoming activity before being processed 
		double activation; // The total activation entering the NNode 
		bool active_flag;  // To make sure outputs are active

		std::vector<Link*> incoming; // A list of pointers to incoming weighted signals from other nodes
		std::vector<Link*> outgoing;  // A list of pointers to links carrying this node's signal

		int node_id;  // A node can be given an identification number for saving in files

		NNode(nodetype ntype,int nodeid, nodeplace iplace);

		// Copy Constructor
		NNode (const NNode& nnode);

		~NNode();

		// Just return activation for step
		double get_active_out();

		// Return activation from PREVIOUS time step
		double get_active_out_td();

		// Returns the type of the node, NEURON or SENSOR
		const nodetype get_type();

		// Allows alteration between NEURON and SENSOR.  Returns its argument
		nodetype set_type(nodetype);

		// If the node is a SENSOR, returns true and loads the value
		bool sensor_load(double);

		// Adds a NONRECURRENT Link to a new NNode in the incoming List
		void add_incoming(NNode*,double);

		// Adds a Link to a new NNode in the incoming List
		void add_incoming(NNode*,double,bool);

		// Recursively deactivate backwards through the network
		void flushback();

		// Verify flushing for debugging
		void flushback_check(std::vector<NNode*> &seenlist);

		//Find the greatest depth starting from this neuron at depth d
		int depth(int d,Network *mynet); 

	};


} // namespace NEAT

#endif
