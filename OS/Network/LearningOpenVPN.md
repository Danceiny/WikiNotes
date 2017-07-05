Chapter1 
VPN sofwares takes IP packets or Ethernet frames and wraps them into another packet . 

IPsec：
Protocol implemented on OSI layer 3

SSL,SSH,TLS:
Protocols implemented on OSI layer 4


Chapter2 VPN Security


# Generating certificates and keys 
1. Create a Certification Authority certificate for your CA, which will sign and revolke client certificates.
2. Create a key and a certrificate signing request for the clients(or users), or let the users create them.
3. SIgn the requests using the CA certificate, thereby making them valid.
4. Provide keys and certificates to the VPN partners.

![](http://opkk27k9n.bkt.clouddn.com/17-7-5/77286945.jpg)
 
 ![](http://opkk27k9n.bkt.clouddn.com/17-7-5/95766892.jpg)
