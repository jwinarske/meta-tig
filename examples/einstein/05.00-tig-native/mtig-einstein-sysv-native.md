# Objectives

Add [https://grafana.com/](Grafana) to an exisiting Yocto Project(R) based product. 

This product did many thing right, like using Open Source meta layers which is what we will use here.

The product is something like [https://www.victronenergy.com/blog/2020/02/06/cerbo-gx-now-on-e-order-and-ready-for-pre-order/](this).

The relevant meta-layer is [https://github.com/victronenergy/venus](here) and the machine is calles `einstein`.

As an add on and in order to come up with some demo we'll also use [https://www.influxdata.com/time-series-platform/telegraf/](Telegraf) which will collect
e.g. CPU and RAM usage and feed the collected data into [https://www.influxdata.com/products/influxdb/](InfluxDB).


# Challenges

- [https://www.influxdata.com/time-series-platform/telegraf/](Telegraf), [https://www.influxdata.com/products/influxdb/](InfluxDB), [https://grafana.com/](Grafana), are written in [https://golang.org/](Go).
  - Pretty much impossible to compile from sources with old Yocto(R) versions like the one used in the product
  - Even if the product is updated to a newer version, like `dunfell` it's pretty hard to compile [https://grafana.com/](Grafana) since in addition to all the [https://golang.org/](Go) fun we would also need to use [https://yarnpkg.com/](yarn), which is not quite supported out of the box.
- The existing product uses `system-v` as an init system, which I never tried with those apps before.
- Normally I would run all this in containers, but for this experiment we'll skip this and run the apps natively, which I did not try before as well, except for [https://www.influxdata.com/time-series-platform/telegraf/](Telegraf)

# Solution

- use precompiled binaries for [https://golang.org/](Go) packages
  - implications
    - License Compliance
    - No compiler optimizations
    - harder to debug
- create users/groups/directories/init scripts and so on

# Demo

## Download/Flash

Note: You will wipe whatever is on your board!

Download this image, write it to the SD card, plug it into you board and reboot.

On the serial console you should see something like that

```
Main loop Daemon
Version 0.1.0
Software updated successfully
Please reboot the device to start the new software
[NOTIFY] : SWUPDATE successful ! 
Installing firmware...
Open On-Chip Debugger 0.10.0+dev-00809-g7ee618692-dirty (2021-04-19-19:34)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
SysfsGPIO num: swdio = 269
SysfsGPIO num: srst = 270
SysfsGPIO num: swclk = 271
none separate
adapter speed: 400 kHz
cortex_m reset_config sysresetreq
srst_only separate srst_gates_jtag srst_open_drain connect_deassert_srst
adapter_nsrst_delay: 100
adapter_nsrst_assert_width: 100
Info : SysfsGPIO JTAG/SWD bitbang driver
Info : SWD only mode enabled (specify tck, tms, tdi and tdo gpios to add JTAG mode)
Info : This adapter doesn't support configurable speed
Info : SWD DPIDR 0x0bc11477
Info : at91samc21g18.cpu: hardware has 4 breakpoints, 2 watchpoints
Info : at91samc21g18.cpu: external reset detected
Error: couldn't bind gdb to socket on port 3333: Cannot assign requested address
    TargetName         Type       Endian TapName            State       
--  ------------------ ---------- ------ ------------------ ------------
 0* at91samc21g18.cpu  cortex_m   little at91samc21g18.cpu  running
Info : SWD DPIDR 0x0bc11477
target halted due to debug-request, current mode: Thread 
xPSR: 0x61000000 pc: 0x00003208 psp: 0x20001400
** Programming Started **
auto erase enabled
Info : SAMD MCU: SAMC21G17A (128KB Flash, 16KB RAM)
wrote 34304 bytes from file /opt/victronenergy/firmware/samc21_slcan_adc.bin in 5.270360s (6.356 KiB/s)
** Programming Finished **
** Verify Started **
verified 34300 bytes in 0.104399s (320.847 KiB/s)
** Verified OK **
** Resetting Target **
Info : SWD DPIDR 0x0bc11477
shutdown command invoked
Installation complete
Remove installer medium and power cycle system
```

So let's do what it says: `Remove installer medium and power cycle system`.

## Run new software

You should see something like this on the serial console:
```
U-Boot SPL 2018.05 (Apr 20 2021 - 08:00:17 +0000)                                                                                                                                             
DRAM: 1024 MiB                                                                                                                                                                                
CPU: 912000000Hz, AXI/AHB/APB: 3/2/2                                                                                                                                                          
Trying to boot from MMC2                                                                                                                                                                      
                                                                                                                                                                                              
                                                                                                                                                                                              
U-Boot 2018.05 (Apr 20 2021 - 08:00:17 +0000) Allwinner Technology                                                                                                                            
                                                                                                                                                                                              
CPU:   Allwinner A20 (SUN7I)                                                                                                                                                                  
Model: Cubietech Cubieboard2                                                                                                                                                                  
I2C:   ready                                                                                                                                                                                  
DRAM:  1 GiB                                                                                                                                                                                  
MMC:   SUNXI SD/MMC: 0, SUNXI SD/MMC: 1                                                                                                                                                       
Loading Environment from MMC... OK                                                                                                                                                            
In:    serial@01c28000                                                                                                                                                                        
Out:   serial@01c28000                                                                                                                                                                        
Err:   serial@01c28000                                                                                                                                                                        
Net:   No ethernet found.                                                                                                                                                                     
Setting bus to 1                                                                                                                                                                              
Hit any key to stop autoboot:  0                                                                                                                                                              
29518 bytes read in 7 ms (4 MiB/s)                                                                                                                                                            
3983096 bytes read in 228 ms (16.7 MiB/s)                                                                                                                                                     
## Flattened Device Tree blob at 43000000                                                                                                                                                     
   Booting using the fdt blob at 0x43000000                                                                                                                                                   
   Loading Device Tree to 49ff5000, end 49fff34d ... OK                                                                                                                                       
                                                                                                                                                                                              
Starting kernel ...                                                                                                                                                                           
                                                                                                                                                                                              
[    0.000000] Booting Linux on physical CPU 0x0                                                                                                                                              
[    0.000000] Linux version 4.19.123 (oe-user@oe-host) (gcc version 9.2.0 (GCC)) #1 SMP Mon Apr 19 17:53:23 UTC 2021                                                                         
[    0.000000] CPU: ARMv7 Processor [410fc074] revision 4 (ARMv7), cr=10c5387d                                                                                                                
[    0.000000] CPU: div instructions available: patching division code                                                                                                                        
[    0.000000] CPU: PIPT / VIPT nonaliasing data cache, VIPT aliasing instruction cache       
...
[   33.129155] vcc3v0: disabling                                                                                                                                                              
[   33.132139] vcc5v0: disabling                                                                                                                                                              
Initializing AES buffer                                                                                                                                                                       
                                                                                                                                                                                              
Enabling JITTER rng support                                                                                                                                                                   
                                                                                                                                                                                              
Initializing entropy source jitter                                                                                                                                                            
                                                                                                                                                                                              
.                                                                                                                                                                                             
Starting haveged: haveged: listening socket at 3                                                                                                                                              
haveged: haveged starting up                                                                                                                                                                  
[  OK  ]                                                                                                                                                                                      
Starting bluetooth: bluetoothd.                                                                                                                                                               
starting DNS forwarder and DHCP server: dnsmasq... done.                                                                                                                                      
[   37.187628] NET: Registered protocol family 38                                                                                                                                             
starting DNS forwarder and DHCP server: dnsmasq.ap... done.                                                                                                                                   
Starting syslogd/klogd: done                                                                                                                                                                  
Starting php-fpm  done                                                                                                                                                                        
Starting Connection Manager                                                                                                                                                                   
ls: /data/var/lib/connman: No such file or directory                                                                                                                                          
Starting Hiawatha Web Server: hiawatha.                                                                                                                                                       
starting IPv4LL configuration daemon: avahi-autoipd... done.                                                                                                                                  
starting resolv.conf manager: resolv-watch... [   37.913981] IPv6: ADDRCONF(NETDEV_UP): ll-eth0: link is not ready                                                                            
done.                                                                                                                                                                                         
dbus-daemon[797]: [system] Activating service name='fi.w1.wpa_supplicant1' requested by ':1.1' (uid=0 pid=857 comm="connmand --nodnsproxy --nodaemon ") (using servicehelper)                 
Starting crond: dbus-daemon[797]: [system] Successfully activated service 'fi.w1.wpa_supplicant1'                                                                                             
OK                                                                                                                                                                                            
Checking available software versions...                                                                                                                                                       
Active rootfs: 1                                                                                                                                                                              
Active version: 20210702182640 v2.70~5                                                                                                                                                        
Filesystem errors detected on backup rootfs                                                                                                                                                   
/etc/rc5.d/S99custom-rc-late.sh: line 5: /data/rc.local: No such file or directory                                                                                                            
Starting Grafana Server                                                                                                                                                                       
[   39.989345] device eth0 entered promiscuous mode                                                                                                                                           
[   39.994325] IPv6: ADDRCONF(NETDEV_UP): eth0: link is not ready                                                                                                                             
exit 1                                                                                                                                                                                        
Starting influxdb...                                                                                                                                                                          
[   41.249272] sun4i-emac 1c0b000.ethernet eth0: Link is Up - 100Mbps/Full - flow control off                                                                                                 
[   41.257770] IPv6: ADDRCONF(NETDEV_CHANGE): eth0: link becomes ready                                                                                                                        
[   41.267177] IPv6: ADDRCONF(NETDEV_CHANGE): ll-eth0: link becomes ready                                                                                                                     
influxdb process was unable to start [ FAILED ]                                                                                                                                               
Starting the process telegraf [ OK ]                                                                                                                                                          
telegraf process was started [ OK ]                   root@einstein:~#                                                                                                                                                  ```



```
## Are daemons running?
Let's see if our new daemons are running

```
root@einstein:~#
root@einstein:~# /etc/init.d/telegraf status
telegraf Process is running [ OK ]
root@einstein:~# /etc/init.d/influxdb status
influxdb process is running [ OK ]
root@einstein:~# /etc/init.d/grafana-server status
grafana is running [ OK ]
root@einstein:~# 
```

Yes they are running!

## Is InfluxDB being fed?
Now we shuld see that telegraf feeds into the InfluxDB:

```
[root@einstein:~#]
root@einstein:~# cat /var/log/influxdb/influxd.log                                                                                                                                            
ts=2021-07-02T19:20:10.681136Z lvl=info msg="InfluxDB starting" log_id=0V6ATdjG000 version=1.8.6 branch=1.8 commit=v1.8.6                                                                     
ts=2021-07-02T19:20:10.681460Z lvl=info msg="Go runtime" log_id=0V6ATdjG000 version=go1.13.8 maxprocs=2                                                                                       
ts=2021-07-02T19:20:10.910527Z lvl=info msg="Using data dir" log_id=0V6ATdjG000 service=store path=/var/lib/influxdb/data                                                                     
ts=2021-07-02T19:20:10.911448Z lvl=info msg="Compaction settings" log_id=0V6ATdjG000 service=store max_concurrent_compactions=1 throughput_bytes_per_second=50331648 throughput_bytes_per_second_burst=50331648                                                                                                                                                                             
ts=2021-07-02T19:20:10.911827Z lvl=info msg="Open store (start)" log_id=0V6ATdjG000 service=store trace_id=0V6ATecl000 op_name=tsdb_open op_event=start                                       
ts=2021-07-02T19:20:10.912675Z lvl=info msg="Open store (end)" log_id=0V6ATdjG000 service=store trace_id=0V6ATecl000 op_name=tsdb_open op_event=end op_elapsed=0.974ms                        
ts=2021-07-02T19:20:10.931479Z lvl=info msg="Opened service" log_id=0V6ATdjG000 service=subscriber                                                                                            
ts=2021-07-02T19:20:10.932537Z lvl=info msg="Starting monitor service" log_id=0V6ATdjG000 service=monitor                                                                                     
ts=2021-07-02T19:20:10.932695Z lvl=info msg="Registered diagnostics client" log_id=0V6ATdjG000 service=monitor name=build                                                                     
ts=2021-07-02T19:20:10.932792Z lvl=info msg="Registered diagnostics client" log_id=0V6ATdjG000 service=monitor name=runtime                                                                   
ts=2021-07-02T19:20:10.932873Z lvl=info msg="Registered diagnostics client" log_id=0V6ATdjG000 service=monitor name=network                                                                   
ts=2021-07-02T19:20:10.932975Z lvl=info msg="Registered diagnostics client" log_id=0V6ATdjG000 service=monitor name=system                                                                    
ts=2021-07-02T19:20:10.949883Z lvl=info msg="Starting precreation service" log_id=0V6ATdjG000 service=shard-precreation check_interval=10m advance_period=30m                                 
ts=2021-07-02T19:20:10.950192Z lvl=info msg="Starting snapshot service" log_id=0V6ATdjG000 service=snapshot                                                                                   
ts=2021-07-02T19:20:10.950384Z lvl=info msg="Starting continuous query service" log_id=0V6ATdjG000 service=continuous_querier                                                                 
ts=2021-07-02T19:20:10.950551Z lvl=info msg="Starting HTTP service" log_id=0V6ATdjG000 service=httpd authentication=false                                                                     
ts=2021-07-02T19:20:10.950667Z lvl=info msg="opened HTTP access log" log_id=0V6ATdjG000 service=httpd path=stderr                                                                             
ts=2021-07-02T19:20:10.951295Z lvl=info msg="Listening on HTTP" log_id=0V6ATdjG000 service=httpd addr=[::]:8086 https=false                                                                   
ts=2021-07-02T19:20:10.951647Z lvl=info msg="Starting retention policy enforcement service" log_id=0V6ATdjG000 service=retention check_interval=30m                                           
ts=2021-07-02T19:20:10.980784Z lvl=info msg="Storing statistics" log_id=0V6ATdjG000 service=monitor db_instance=_internal db_rp=monitor interval=10s                                          
ts=2021-07-02T19:20:10.981170Z lvl=info msg="Listening for signals" log_id=0V6ATdjG000                                                                                                        
ts=2021-07-02T19:20:10.982388Z lvl=info msg="Sending usage statistics to usage.influxdata.com" log_id=0V6ATdjG000                                                                             
ts=2021-07-02T19:20:15.221085Z lvl=info msg="Executing query" log_id=0V6ATdjG000 service=query query="CREATE DATABASE \"einstein-02:17:06:00:c1:20\""                                         
[httpd] 127.0.0.1 - - [02/Jul/2021:19:20:15 +0000] "POST /query HTTP/1.1 {'q': 'CREATE DATABASE "einstein-02:17:06:00:c1:20"'}" 200 57 "-" "Telegraf/1.18.2 Go/1.16.2" 87852624-db6a-11eb-8001-02170600c120 101939                                                                                                                                                                          
[httpd] 127.0.0.1 - - [02/Jul/2021:19:20:25 +0000] "POST /write?db=einstein-02%3A17%3A06%3A00%3Ac1%3A20 HTTP/1.1 " 204 0 "-" "Telegraf/1.18.2 Go/1.16.2" 8daf9eef-db6a-11eb-8002-02170600c120 384375                                                                                                                                                                                        
[httpd] 127.0.0.1 - - [02/Jul/2021:19:20:35 +0000] "POST /write?db=einstein-02%3A17%3A06%3A00%3Ac1%3A20 HTTP/1.1 " 204 0 "-" "Telegraf/1.18.2 Go/1.16.2" 93a57446-db6a-11eb-8003-02170600c120 133477                                                                                                                                                                                        
[httpd] 127.0.0.1 - - [02/Jul/2021:19:20:45 +0000] "POST /write?db=einstein-02%3A17%3A06%3A00%3Ac1%3A20 HTTP/1.1 " 204 0 "-" "Telegraf/1.18.2 Go/1.16.2" 999b655f-db6a-11eb-8004-02170600c120 48889                                                                                                                                                                                         
[httpd] 127.0.0.1 - - [02/Jul/2021:19:20:55 +0000] "POST /write?db=einstein-02%3A17%3A06%3A00%3Ac1%3A20 HTTP/1.1 " 204 0 "-" "Telegraf/1.18.2 Go/1.16.2" 9f92bd38-db6a-11eb-8005-02170600c120 35660                                                                                                                                                                                         
[httpd] 127.0.0.1 - - [02/Jul/2021:19:21:05 +0000] "POST /write?db=einstein-02%3A17%3A06%3A00%3Ac1%3A20 HTTP/1.1 " 204 0 "-" "Telegraf/1.18.2 Go/1.16.2" a5879d7c-db6a-11eb-8006-02170600c120 79669                                                                                                                                                                                         
[httpd] 127.0.0.1 - - [02/Jul/2021:19:21:15 +0000] "POST /write?db=einstein-02%3A17%3A06%3A00%3Ac1%3A20 HTTP/1.1 " 204 0 "-" "Telegraf/1.18.2 Go/1.16.2" ab7e40c5-db6a-11eb-8007-02170600c120 23909                                                                                                                                                                                         
[httpd] 127.0.0.1 - - [02/Jul/2021:19:21:25 +0000] "POST /write?db=einstein-02%3A17%3A06%3A00%3Ac1%3A20 HTTP/1.1 " 204 0 "-" "Telegraf/1.18.2 Go/1.16.2" b173fba7-db6a-11eb-8008-02170600c120 33910                                                                                                                                                                                         
[httpd] 127.0.0.1 - - [02/Jul/2021:19:21:35 +0000] "POST /write?db=einstein-02%3A17%3A06%3A00%3Ac1%3A20 HTTP/1.1 " 204 0 "-" "Telegraf/1.18.2 Go/1.16.2" b7695bbf-db6a-11eb-8009-02170600c120 72564                                                                                                      
...
```

Please note, that I might replace e.g. Telegraf/1.18.2 with a newer version.

You can see that InfluxDB is being fed with data from Telegraf.

## Grafana

Finally we arrived at Grafana.

### Which IP address?

```
[root@einstein:~#]
root@einstein:~# ip addr | grep eth0
2: eth0: <BROADCAST,MULTICAST,DYNAMIC,UP,LOWER_UP> mtu 1450 qdisc pfifo_fast state UP group default qlen 1000
    inet 192.168.42.233/24 brd 192.168.42.255 scope global eth0
```

### Let's connect to it

Browse to <ip-address-of-above>:3000

e.g.

```
http://192.168.42.233:3000/login
```

And you should see

![alt text](img/login-1.png "grafana login")

