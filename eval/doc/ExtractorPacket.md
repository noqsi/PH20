### Event extractor daemon (evextd) interface

The event extractor 

####Messages client->daemon

#####Start message

Field | Length | Content
----- | -----: | -------
id  | 1      | 0xC0DE0001
shape | 1 | Shaping time in samples
trigger | 1 | Trigger threshhold ADU
hysteresis | 1 | Trigger hysteresis ADU
dump | 1 | Dump threshold ADU

#####Dump message

Field | Length | Content
----- | -----: | -------
id  | 1      | 0xC0DE0002

####Messages daemon->client

#####Data message
 
Field | Length | Content
----- | -----: | -------
id  | 1      | 0xC0DE0003
rise | 1 | time of trigger
peak | 1 | time of peak
fall | 1 | time of fall
height | 1 | height of peak
data  | 4*shape  | ADC samples

#####Dump request

Field | Length | Content
----- | -----: | -------
id  | 1      | 0xC0DE0004
