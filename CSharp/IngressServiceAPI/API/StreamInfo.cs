namespace IngressServiceAPI.API
{
    /// <summary>
    /// This defines the relationship between a stream and a type. Id and Type
    /// property names are defined in the OMF spec.  This class is serialized into 
    /// an OMF message.
    /// </summary>
    public class StreamInfo
    {
        public string Id { get; set; }
        public string Type { get; set; }
    }
}