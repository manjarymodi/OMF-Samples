# OMF-Samples created by OSIsoft specific to PI System backend

One of the set of samples created by OSIsoft, which are platform and programming language agnostic, and show common OMF version 1.0 functionality specific to the PI System backend.

The set contains three OMF v1.0 sample applications written in Microsoft C#, Python and Node.js. The samples demonstrate the most common usage of OMF. They all create the same PI Asset Framework reference models and the same sets of PI Data Archive points. The only difference is the programming language been used.

The samples in this set illustrate OMF features that are specific to the PI System back end. The main such feature is the support for "metadata," which is sent by OMF applications to the OMF ingress endpoint, and stored in PI Asset Framework Server.  

The metadata might represent customer assets (such as plants, machines, devices, data I/O points, and so on) as PI Asset Framework "reference models". The model is a tree-like structure of elements and their associated attributes, which may store their values as time-series data events in PI Data Archive points.  

There are a few limitations you should be aware of, which might prevent successful execution of OMF samples:  
- The PI System back end does not support compound indexing. You can specify only one OMF type property with the keyword "isindex" set to "true". For OMF types with keyword "classification" set to "dynamic", the "isindex" property must be of type "string" and format "date-time", representing timestamp for a data stream.  
- The PI System back end does not support nested and array properties of type "object". For more information about writing OMF applications for the PI System back end, please see [OMF Developer Companion Guide.](http://omf-companion-docs.osisoft.com)  

Each of the samples in this set represents a stand-alone OMF application. Before running the application, you must register the application with PI Data Collection Manager to obtain a "Producer Token" and "Relay Ingress URL," after which you update the appropriate application configuration setting. For more information, see [PI Connector Administration Guide](https://techsupport.osisoft.com/Downloads/File/40489fc5-e515-4669-b185-8866a9f9f616), and readme file for the sample. 

# Sample C# OMF applicaiton specific to PI System backend

Before running this sample OMF application, register it with PI Data Collection Manager, obtain "Producer Token" and "Relay Ingress URL", and update "config.json" file. For more information about registration process, please see [PI Connector Administration Guide.](https://techsupport.osisoft.com/Downloads/File/40489fc5-e515-4669-b185-8866a9f9f616)