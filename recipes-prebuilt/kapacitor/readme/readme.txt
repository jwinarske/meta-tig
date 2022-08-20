db:

imx6q-phytec-mira-rdk-50:2d:f4:16:2d:bd

tick script:

stream
    |from()
        .measurement('cpu_native')
    |alert()
        .crit(lambda: "usage_user" > 10)
        .message('{{ .Time }}: CPU usage over 90%')
        .mqtt('alerts')
        .qos(2)

tail -f /var/log/mosquitto.log

stress-ng --cpu 4 --io 4 --vm 2 --vm-bytes 128M --fork 4 --timeout 10s

660919342: Sending PINGRESP to localhost 
1660919372: Received PINGREQ from localhost 
1660919372: Sending PINGRESP to localhost 
1660919374: Received PUBLISH from localhost (d0, q2, r0, m1, 'alerts', ... (49 bytes)) 
1660919374: Sending PUBREC to localhost (m1, rc0) 
1660919374: Received PUBREL from localhost (Mid: 1) 
1660919374: Sending PUBCOMP to localhost (m1) 
1660919374: Received PUBLISH from localhost (d0, q2, r0, m1, 'alerts', ... (49 bytes)) 
1660919374: Sending PUBREC to localhost (m1, rc0) 
1660919374: Received PUBREL from localhost (Mid: 1) 
1660919374: Sending PUBCOMP to localhost (m1) 
1660919374: Received PUBLISH from localhost (d0, q2, r0, m1, 'alerts', ... (49 bytes)) 
1660919374: Sending PUBREC to localhost (m1, rc0) 
1660919374: Received PUBREL from localhost (Mid: 1) 
1660919374: Sending PUBCOMP to localhost (m1) 
1660919374: Received PUBLISH from localhost (d0, q2, r0, m1, 'alerts', ... (49 bytes)) 
1660919374: Sending PUBREC to localhost (m1, rc0) 
1660919374: Received PUBREL from localhost (Mid: 1) 
1660919374: Sending PUBCOMP to localhost (m1) 
1660919374: Received PUBLISH from localhost (d0, q2, r0, m1, 'alerts', ... (49 bytes)) 
1660919374: Sending PUBREC to localhost (m1, rc0) 
1660919374: Received PUBREL from localhost (Mid: 1) 
1660919374: Sending PUBCOMP to localhost (m1) 
1660919384: Received PUBLISH from localhost (d0, q2, r0, m1, 'alerts', ... (49 bytes)) 
1660919384: Sending PUBREC to localhost (m1, rc0) 
1660919384: Received PUBREL from localhost (Mid: 1) 
1660919384: Sending PUBCOMP to localhost (m1) 


mosquitto_sub -v -h localhost -p 1883 -t '#'
alerts 2022-08-19 14:32:30 +0000 UTC: CPU usage over 90%
alerts 2022-08-19 14:32:30 +0000 UTC: CPU usage over 90%
alerts 2022-08-19 14:32:30 +0000 UTC: CPU usage over 90%
alerts 2022-08-19 14:32:30 +0000 UTC: CPU usage over 90%
alerts 2022-08-19 14:32:40 +0000 UTC: CPU usage over 90%
alerts 2022-08-19 14:32:40 +0000 UTC: CPU usage over 90%
alerts 2022-08-19 14:32:40 +0000 UTC: CPU usage over 90%
alerts 2022-08-19 14:32:40 +0000 UTC: CPU usage over 90%
alerts 2022-08-19 14:32:40 +0000 UTC: CPU usage over 90%
alerts 2022-08-19 14:32:50 +0000 UTC: CPU usage over 90%

if you want to access not only from localhost, you can hack /etc/mosquitto/mosquitto.conf like this:

# --> rber
listener 1883
allow_anonymous true
# <-- rber
