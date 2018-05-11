# Sample NodeJS_PI OMF applicaiton  

Before running this sample OMF application, register it with PI Data Collection Manager, obtain "Producer Token" and "Relay Ingress URL", and update "config.json" file. For more information about registration process, please see [PI Connector Administration Guide.](https://techsupport.osisoft.com/Downloads/File/40489fc5-e515-4669-b185-8866a9f9f616)  


## Samples for on-premises PI System back end

The set contains three OMF v1.0 sample applications written in Microsoft C#, Python and Node.js. Each language sample is platform and operating system agnostic, and demonstrate the most common usage of OMF version 1.0 with an on-premises PI System back end. Each language sample creates the same back end structure; the only difference between them is the programming language used.

The samples illustrate OMF features that are specific to on-premises PI System. The samples demonstrate support for metadata, which is sent by OMF applications to the OMF ingress endpoint.  The metadata represents customer assets (such as plants, machines, devices, data I/O points, and so on) as Asset Framework "reference models". The model is a tree-like structure of AF Elements with Attributes, which may store their values as time-series data events in Data Archive PI points.

There are a few limitations you should be aware of developing OMF applications for on-premises PI System:  
- Compound indexing is not supported. You should specify only one OMF type property with the keyword "isindex" set to "true". For OMF types with keyword "classification" set to "dynamic", the "isindex" property must be formatted as "date-time", representing timestamp for a data stream.  
- Nested properties and array properties of type "object" are not supported.

For more information about writing OMF applications for on-premises PI System, please see [OMF Developer Companion Guide.](http://omf-companion-docs.osisoft.com)