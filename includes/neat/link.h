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
#ifndef _LINK_H_
#define _LINK_H_

namespace NEAT {

	class NNode;

	// -----------------------------------------------------------------------
	// A LINK is a connection from one node to another with an associated weight
	// It can be marked as recurrent
	// Its parameters are made public for efficiency
	class Link {
	public: 
		double weight; // Weight of connection
		NNode *in_node; // NNode inputting into the link
		NNode *out_node; // NNode that the link affects
		bool is_recurrent;

		Link(double w, NNode *inode, NNode *onode, bool recur) {
			weight = w;
			in_node = inode;
			out_node = onode;
            is_recurrent = recur;
		}
	};

} // namespace NEAT

#endif
