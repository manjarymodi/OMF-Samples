
Purpose: C program to send sample data formatted in the OSIsoft Message format (OMF) to a REST endpoint

Requirement:

1. Windows or Linux environment with a c compiler
2. files included in this sample
3. installed and configured PI Data Collection Manager and PI Connector Relay
3. OMF application configured using PI Data Collection Manager 
   The Producer Token and Relay Ingress URL generated by this configuration will be used to run and configure the sample.

Setup:

1. extract files and update main.c, set szProducerToken to the Producer Token specified in the OMF application configuration 
2. build program

2a. Windows: There is a .sln file for use with Visual Studio

2b. Linux: Run make from the c/omfDemo/omfDemo directory

3. After a succesful build, run the program e.g.:
   ./omfDemo relayServerName portNumber
   
    The program will send a set of defintions to the Relay and then send data values for a period of time or until the program is stopped.


See also: http://omf-docs.readthedocs.io - OSIsoft Message format specification
