
Purpose:
Bash script to send sample data formatted in the OSIsoft Message Format (OMF) to a REST endpoint

Requirement: 
1. bash shell
2. curl - command line tool for transferring data with URL syntax
3. files listed below
4. security token

Setup:
1. extract files and folders
2. set execute permissions for scripts runit and omfDemo:
   $ chmod +ux runit omfDemo
3. update file token with security token for destination REST endpoint

Usage:
See examples in the runit file and/or run the script to get usage information:
$ ./omfDemo

Files:
readme
runit - example usage of omfDemo script
omfDemo - script to send OMF data:w
token - specify the security token to use
data - directory of OMF object defintions
data/type
data/container
data/data-links
data/data-assets
data/data
data/template.data - used to generate data/data file

See also:
http://omf-docs.readthedocs.io - OSIsoft Message format specification

Tested on Ubuntu 16.04
