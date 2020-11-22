cd ~/cisco
# ■common
CiscoConf.py get-templ
CiscoConf.py get-mnem
CiscoConf.py conf    acl center1/L3SW/*.shipacl.txt
CiscoConf.py confcsv acl center1/L3SW/*.shipacl.txt
CiscoConf.py confcsv2 acl center1/L3SW/*.shipacl.txt
CiscoConf.py int2 center1/L3SW/*.shint.txt

CiscoConf.py netwild   192.168.1.0 0.0.0.255
CiscoConf.py ipinnet   192.168.1.0 192.168.1.0/24 
CiscoConf.py ipinwild  192.168.1.0 192.168.1.0 0.0.0.255
CiscoConf.py ipinf     192.168.1.0/26 
CiscoConf.py ipcalc exclude   192.168.1.0/24 192.168.1.0/28 
CiscoConf.py ipcalc overlaps  192.168.1.0/24 192.168.1.128/30 
CiscoConf.py ipcalc subnetsof 192.168.1.128/30 192.168.1.0/24
CiscoConf.py ipcalc subnets   192.168.1.0/24 4 

CiscoConf.py grep interface center1/L3SW/*.shint.txt


# ■route
CiscoConf.py get-eigrp-code
CiscoConf.py get-iprou-code
CiscoConf.py get-cdpnei-code

CiscoConf.py ip_iprou     10.7.128.226       center1/L3SW/*.shiprou.txt
CiscoConf.py if_iprou     GigabitEthernet0/0 center1/L3SW/*.shiprou.txt
CiscoConf.py ip_eigrptop  10.7.128.226       center1/L3SW/*.shipeigrptop.txt
CiscoConf.py if_eigrptop  GigabitEthernet0/0 center1/L3SW/*.shipeigrptop.txt
CiscoConf.py ip_eigrptop2 10.7.128.226       center1/L3SW/*.shipeigrptop.txt
CiscoConf.py if_eigrptop2 GigabitEthernet0/0 center1/L3SW/*.shipeigrptop.txt

CiscoConf.py nextip_eigrp center1 center1/L3SW/*.shipeigrptop.txt 
CiscoConf.py next_cdpnei  center1 center1/L3SW/*.shcdpnei.txt 


# ■acl
CiscoConf.py get-acl   center1/L3SW/l3sw01-fushimi1.besta.shipacl.txt 
CiscoConf.py acls      center1/L3SW/*.shipacl.txt 
CiscoConf.py test-acl  center1/L3SW/l3sw01-fushimi1.besta.shipacl.txt 
CiscoConf.py test-aclf center1/L3SW/l3sw01-fushimi1.besta.shipacl.txt



