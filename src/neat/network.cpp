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
#include "network.h"

using namespace NEAT;

Network::Network(std::vector<NNode*> in,std::vector<NNode*> out,std::vector<NNode*> all,int netid) {
  inputs=in;
  outputs=out;
  all_nodes=all;
  name=0;   //Defaults to no name  ..NOTE: TRYING TO PRINT AN EMPTY NAME CAN CAUSE A CRASH
  numnodes=-1;
  numlinks=-1;
  net_id=netid;
}


Network::Network(const Network& network)
{
	std::vector<NNode*>::const_iterator curnode;

	// Copy all the inputs
	for(curnode = network.inputs.begin(); curnode != network.inputs.end(); ++curnode) {
		NNode* n = new NNode(**curnode);
		inputs.push_back(n);
		all_nodes.push_back(n);
	}

	// Copy all the outputs
	for(curnode = network.outputs.begin(); curnode != network.outputs.end(); ++curnode) {
		NNode* n = new NNode(**curnode);
		outputs.push_back(n);
		all_nodes.push_back(n);
	}

	if(network.name)
		name = strdup(network.name);
	else
		name = 0;

	numnodes = network.numnodes;
	numlinks = network.numlinks;
	net_id = network.net_id;
}

Network::~Network() {
			if (name!=0)
				delete [] name;

			destroy();  // Kill off all the nodes and links

		}

// Puts the network back into an initial state
void Network::flush() {
	std::vector<NNode*>::iterator curnode;

	for(curnode=outputs.begin();curnode!=outputs.end();++curnode) {
		(*curnode)->flushback();
	}
}

// Debugger: Checks network state
void Network::flush_check() {
	std::vector<NNode*>::iterator curnode;
	std::vector<NNode*>::iterator location;
	std::vector<NNode*> seenlist;  //List of nodes not to doublecount

	for(curnode=outputs.begin();curnode!=outputs.end();++curnode) {    
        location= std::find(seenlist.begin(),seenlist.end(),(*curnode));
		if (location==seenlist.end()) {
			seenlist.push_back(*curnode);
			(*curnode)->flushback_check(seenlist);
		}
	}
}

// If all output are not active then return true
bool Network::outputsoff() {
	std::vector<NNode*>::iterator curnode;

	for(curnode=outputs.begin();curnode!=outputs.end();++curnode) {
		if (((*curnode)->activation_count)==0) return true;
	}

	return false;
}

// Print the connections weights to a file separated by only carriage returns
//void Network::print_links_tofile(char *filename) {
//	std::vector<NNode*>::iterator curnode;
//	std::vector<Link*>::iterator curlink;
//
//    std::ofstream oFile(filename);
//
//	for(curnode=all_nodes.begin();curnode!=all_nodes.end();++curnode) {
//		if (((*curnode)->type)!=SENSOR) {
//			for(curlink=((*curnode)->incoming).begin(); curlink!=((*curnode)->incoming).end(); ++curlink) {
//                oFile << (*curlink)->in_node->node_id << " -> " <<( *curlink)->out_node->node_id << " : " << (*curlink)->weight << std::endl;
//			} // end for loop on links
//		} //end if
//	} //end for loop on nodes
//
//	oFile.close();
//
//} //print_links_tofile

// Activates the net such that all outputs are active
// Returns true on success;
bool Network::activate() {
	std::vector<NNode*>::iterator curnode;
	std::vector<Link*>::iterator curlink;
	double add_amount;  //For adding to the activesum
	bool onetime; //Make sure we at least activate once
	int abortcount=0;  //Used in case the output is somehow truncated from the network

	//cout<<"Activating network: "<<this->genotype<<endl;

	//Keep activating until all the outputs have become active 
	//(This only happens on the first activation, because after that they
	// are always active)

	onetime=false;

	while(outputsoff()||!onetime) {

		++abortcount;

		if (abortcount==20) {
			return false;
			//cout<<"Inputs disconnected from output!"<<endl;
		}
		//std::cout<<"Outputs are off"<<std::endl;

		// For each node, compute the sum of its incoming activation 
		for(curnode=all_nodes.begin();curnode!=all_nodes.end();++curnode) {
			//Ignore SENSORS

			//cout<<"On node "<<(*curnode)->node_id<<endl;

			if (((*curnode)->type)!=SENSOR) {
				(*curnode)->activesum=0;
				(*curnode)->active_flag=false;  //This will tell us if it has any active inputs

				// For each incoming connection, add the activity from the connection to the activesum 
				for(curlink=((*curnode)->incoming).begin();curlink!=((*curnode)->incoming).end();++curlink) {

						add_amount=((*curlink)->weight)*(((*curlink)->in_node)->get_active_out());
						if ((((*curlink)->in_node)->active_flag)||
							(((*curlink)->in_node)->type==SENSOR)) (*curnode)->active_flag=true;
						(*curnode)->activesum+=add_amount;
						//std::cout<<"Node "<<(*curnode)->node_id<<" adding "<<add_amount<<" from node "<<((*curlink)->in_node)->node_id<<std::endl;


				} //End for over incoming links

			} //End if (((*curnode)->type)!=SENSOR) 

		} //End for over all nodes

		// Now activate all the non-sensor nodes off their incoming activation 
		for(curnode=all_nodes.begin();curnode!=all_nodes.end();++curnode) {

			if (((*curnode)->type)!=SENSOR) {
				//Only activate if some active input came in
				if ((*curnode)->active_flag) {
					//cout<<"Activating "<<(*curnode)->node_id<<" with "<<(*curnode)->activesum<<": ";

					//Keep a memory of activations for potential time delayed connections

					(*curnode)->last_activation=(*curnode)->activation;


						//Now run the net activation through an activation function
						if ((*curnode)->ftype==SIGMOID)
							(*curnode)->activation=fsigmoid((*curnode)->activesum,4.924273,2.4621365);  //Sigmoidal activation- see comments under fsigmoid

					//cout<<(*curnode)->activation<<endl;

					//Increment the activation_count
					//First activation cannot be from nothing!!
					(*curnode)->activation_count++;
				}
			}
		}

		onetime=true;
	}

	return true;  
}

double Network::fsigmoid(double activesum,double slope,double constant) {
	//RIGHT SHIFTED ---------------------------------------------------------
	//return (1/(1+(exp(-(slope*activesum-constant))))); //ave 3213 clean on 40 runs of p2m and 3468 on another 40
	//41394 with 1 failure on 8 runs

	//LEFT SHIFTED ----------------------------------------------------------
	//return (1/(1+(exp(-(slope*activesum+constant))))); //original setting ave 3423 on 40 runs of p2m, 3729 and 1 failure also

	//PLAIN SIGMOID ---------------------------------------------------------
	//return (1/(1+(exp(-activesum)))); //3511 and 1 failure

	//LEFT SHIFTED NON-STEEPENED---------------------------------------------
	//return (1/(1+(exp(-activesum-constant)))); //simple left shifted

	//NON-SHIFTED STEEPENED
	return (1/(1+(exp(-(slope*activesum))))); //Compressed
}

// Prints the values of its outputs
void Network::show_activation() {
	std::vector<NNode*>::iterator curnode;
	int count;

	//if (name!=0)
	//  cout<<"Network "<<name<<" with id "<<net_id<<" outputs: (";
	//else cout<<"Network id "<<net_id<<" outputs: (";

	count=1;
	for(curnode=outputs.begin();curnode!=outputs.end();++curnode) {
		//cout<<"[Output #"<<count<<": "<<(*curnode)<<"] ";
		count++;
	}

	//cout<<")"<<endl;
}

void Network::show_input() {
	std::vector<NNode*>::iterator curnode;
	int count;

	//if (name!=0)
	//  cout<<"Network "<<name<<" with id "<<net_id<<" inputs: (";
	//else cout<<"Network id "<<net_id<<" outputs: (";

	count=1;
	for(curnode=inputs.begin();curnode!=inputs.end();++curnode) {
		//cout<<"[Input #"<<count<<": "<<(*curnode)<<"] ";
		count++;
	}

	//cout<<")"<<endl;
}

// Add an input
void Network::add_input(NNode *in_node) {
	inputs.push_back(in_node);
}

// Add an output
void Network::add_output(NNode *out_node) {
	outputs.push_back(out_node);
}

// Takes an array of sensor values and loads it into SENSOR inputs ONLY
void Network::load_sensors(double *sensvals) {
	//int counter=0;  //counter to move through array
	std::vector<NNode*>::iterator sensPtr;

	for(sensPtr=inputs.begin();sensPtr!=inputs.end();++sensPtr) {
		//only load values into SENSORS (not BIASes)
		if (((*sensPtr)->type)==SENSOR) {
			(*sensPtr)->sensor_load(*sensvals);
			sensvals++;
		}
	}
}

void Network::load_sensors(const std::vector<float> &sensvals) {
	//int counter=0;  //counter to move through array
	std::vector<NNode*>::iterator sensPtr;
	std::vector<float>::const_iterator valPtr;

	for(valPtr = sensvals.begin(), sensPtr = inputs.begin(); sensPtr != inputs.end() && valPtr != sensvals.end(); ++sensPtr, ++valPtr) {
		//only load values into SENSORS (not BIASes)
		if (((*sensPtr)->type)==SENSOR) {
			(*sensPtr)->sensor_load(*valPtr);
			//sensvals++;
		}
	}
}

void Network::give_name(char *newname) {
	char *temp;
	char *temp2;
	temp=new char[strlen(newname)+1];
	strcpy(temp,newname);
	if (name==0) name=temp;
	else {
		temp2=name;
		delete temp2;
		name=temp;
	}
}

// The following two methods recurse through a network from outputs
// down in order to count the number of nodes and links in the network.
// This can be useful for debugging genotype->phenotype spawning 
// (to make sure their counts correspond)

int Network::nodecount() {
	int counter=0;
	std::vector<NNode*>::iterator curnode;
	std::vector<NNode*>::iterator location;
	std::vector<NNode*> seenlist;  //List of nodes not to doublecount

	for(curnode=outputs.begin();curnode!=outputs.end();++curnode) {

        location = std::find(seenlist.begin(),seenlist.end(),(*curnode));
		if (location==seenlist.end()) {
			counter++;
			seenlist.push_back(*curnode);
			nodecounthelper((*curnode),counter,seenlist);
		}
	}

	numnodes=counter;

	return counter;

}

void Network::nodecounthelper(NNode *curnode,int &counter,std::vector<NNode*> &seenlist) {
	std::vector<Link*> innodes=curnode->incoming;
	std::vector<Link*>::iterator curlink;
	std::vector<NNode*>::iterator location;

	if (!((curnode->type)==SENSOR)) {
		for(curlink=innodes.begin();curlink!=innodes.end();++curlink) {
            location= std::find(seenlist.begin(),seenlist.end(),((*curlink)->in_node));
			if (location==seenlist.end()) {
				counter++;
				seenlist.push_back((*curlink)->in_node);
				nodecounthelper((*curlink)->in_node,counter,seenlist);
			}
		}

	}

}

int Network::linkcount() {
	int counter=0;
	std::vector<NNode*>::iterator curnode;
	std::vector<NNode*> seenlist;  //List of nodes not to doublecount

	for(curnode=outputs.begin();curnode!=outputs.end();++curnode) {
		linkcounthelper((*curnode),counter,seenlist);
	}

	numlinks=counter;

	return counter;

}

void Network::linkcounthelper(NNode *curnode,int &counter,std::vector<NNode*> &seenlist) {
	std::vector<Link*> inlinks=curnode->incoming;
	std::vector<Link*>::iterator curlink;
	std::vector<NNode*>::iterator location;

    location = std::find(seenlist.begin(),seenlist.end(),curnode);
	if ((!((curnode->type)==SENSOR))&&(location==seenlist.end())) {
		seenlist.push_back(curnode);

		for(curlink=inlinks.begin();curlink!=inlinks.end();++curlink) {
			counter++;
			linkcounthelper((*curlink)->in_node,counter,seenlist);
		}

	}

}

// Destroy will find every node in the network and subsequently
// delete them one by one.  Since deleting a node deletes its incoming
// links, all nodes and links associated with a network will be destructed
// Note: Traits are parts of genomes and not networks, so they are not
//       deleted here
void Network::destroy() {
	std::vector<NNode*>::iterator curnode;
	std::vector<NNode*>::iterator location;
	std::vector<NNode*> seenlist;  //List of nodes not to doublecount

	// Erase all nodes from all_nodes list 

	for(curnode=all_nodes.begin();curnode!=all_nodes.end();++curnode) {
		delete (*curnode);
	}
}

void Network::destroy_helper(NNode *curnode,std::vector<NNode*> &seenlist) {
	std::vector<Link*> innodes=curnode->incoming;
	std::vector<Link*>::iterator curlink;
	std::vector<NNode*>::iterator location;

	if (!((curnode->type)==SENSOR)) {
		for(curlink=innodes.begin();curlink!=innodes.end();++curlink) {
            location = std::find(seenlist.begin(),seenlist.end(),((*curlink)->in_node));
			if (location==seenlist.end()) {
				seenlist.push_back((*curlink)->in_node);
				destroy_helper((*curlink)->in_node,seenlist);
			}
		}

	}

}

// This checks a POTENTIAL link between a potential in_node and potential out_node to see if it must be recurrent 
bool Network::is_recur(NNode *potin_node,NNode *potout_node,int &count,int thresh) {
	std::vector<Link*>::iterator curlink;


	++count;  //Count the node as visited

	if (count>thresh) {
		//cout<<"returning false"<<endl;
		return false;  //Short out the whole thing- loop detected
	}

	if (potin_node==potout_node) return true;
	else {
		//Check back on all links...
		for(curlink=(potin_node->incoming).begin();curlink!=(potin_node->incoming).end();curlink++) {
			//But skip links that are already recurrent
			//(We want to check back through the forward flow of signals only
			if (!((*curlink)->is_recurrent)) {
				if (is_recur((*curlink)->in_node,potout_node,count,thresh)) return true;
			}
		}
		return false;
	}
}


//Find the maximum number of neurons between an ouput and an input
int Network::max_depth() {
  std::vector<NNode*>::iterator curoutput; //The current output we are looking at
  int cur_depth; //The depth of the current node
  int max=0; //The max depth
  
  for(curoutput=outputs.begin();curoutput!=outputs.end();curoutput++) {
    cur_depth=(*curoutput)->depth(0,this);
    if (cur_depth>max) max=cur_depth;
  }

  return max;

}

