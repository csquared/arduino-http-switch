# Arduino HTTP Switch 

This is the arduino code for a project that implements a HTTP-controllable
power supply. 

It works by turning pin 8 on when it recieves a GET request to `/on`.
It turns that pin off when it receives a GET request to `/off`.
This can be used to trip a relay which toggles a power strip.

It also sends GET requests to `/heartbeat` for monitoring uptime.

## Configuration

    ip -> IP of the arduino
    heartbeat_server -> IP of server to send heartbeats to
    heartbeat_delay_millis -> #of millis to wait between heartbeats
    LED -> number of pin to toggle
 
