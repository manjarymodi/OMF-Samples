#SendOMFDataToPISystem.py

This is a single-fall all-in-one OMF example script.  Within this script, OMF types are defined and sent along to a target system, OMF streams are instantiated, using those OMF types (which instructs the receiving system to be able to properly parse the messages it receives, using the type schemas that were previously supplied), and OMF-formatted values are sent to those streams, in order to a) create a new PI AF Element, b) position that element within a PI AF database, c) link that PI AF Element to the PI Points that are created to hold new sensor values, and then finally, d) actually send along those sensor values, and in turn, create PI Points to store those values.  

This script is structured so that the first few lines are where a user can customize this script to fit his or her specific needs--for example, to call third-party libraries to actually send data from specific attached sensors, or to customize the OMF message types to send along data for additional sensors beyond the two sensors listed in the example.  The user also has the option to edit the values of variables that will define the name of the PI AF Element and PI AF Element Template that will be created.  

In short, it is hoped that by cloning this file onto a device, with a few slight modifications to this file, a user can quickly set up an arbitrary device to send along data to am OMF endpoint.  For additional information on OMF, including details that can help explain the role and structure of the message types used in this script, please consult the online OMF documentation, which can be found at http://omf-docs.osisoft.com.

##License

Copyright 2016 OSIsoft, LLC

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

