# Sample CSharp_PI OMF applicaiton  

Before running this sample OMF application, register it with PI Data Collection Manager, obtain "Producer Token" and "Relay Ingress URL", and update "config.json" file. For more information about registration process, please see [PI Connector Administration Guide.](https://techsupport.osisoft.com/Downloads/File/40489fc5-e515-4669-b185-8866a9f9f616)  


## Samples for on-premise PI System back end

The set contains three OMF v1.0 sample applications written in Microsoft C#, Python and Node.js. All of them are platform and Operating System agnostic, and demonstrate the most common usage of OMF version 1.0 with on-premise PI System back end. All create the same back end structures, with the only difference in programming language been used to write them.

The samples in this set illustrate OMF features that are specific to on-premise PI System back end. The main such feature is the support for "metadata," which is sent by OMF applications to the OMF ingress end point. This metadata represents customer assets (such as plants, machines, devices, data I/O points, and so on) as Asset Framework "reference models". The model is a tree-like structure of AF Elements with Attributes, which may store their values as time-series data events in Data Archive PI points.

There are a few limitations you should be aware of developing OMF applications for on-premise PI System:  
- There is no support for compound indexing. You should specify only one OMF type property with the keyword "isindex" set to "true". For OMF types with keyword "classification" set to "dynamic", the "isindex" property must be formatted as "date-time", representing timestamp for a data stream.  
- There is no support for nested properties and array properties of type "object".

For more information about writing OMF applications for on-premise PI System back end, please see [OMF Developer Companion Guide.](http://omf-https://techsupport.osisoft.com/Downloads/File/40489fc5-e515-4669-b185-8866a9f9f616), and readme file for the sample. 