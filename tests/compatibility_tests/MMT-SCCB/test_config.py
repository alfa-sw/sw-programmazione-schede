ipecmd = "/opt/microchip/mplabx/v5.50/sys/java/zulu8.40.0.25-ca-fx-jre8.0.222-linux_x64/bin/java " \
         "-jar /opt/microchip/mplabx/v5.50/mplab_platform/mplab_ipe/ipecmd.jar " \
         "-TS{pickit_serial} -{micro} -L -E -M -F{hexfile}"

alfacmd = "alfa_fw_upgrader -v -s serial {hexfile_arg} -d {deviceId} {actions}"

mmt_micro = "P24FJ256GB110"
sccb_micro = "P33FJ32MC204"

# use ipecmd with argument -T to retrieve serial numbers
mmt_pickit_serial = "BUR210573935" #pickit 4
sccb_pickit_serial = "BUR162072890" #pickit 3

sccb_file = "pump-r1-{boot}-dipswitch_{version}.hex"
sccb_variants = {
  "version": {
    "old": "4_1_4",
    "new": "4_2_0"
  },
  "boot": {
    "yes": "boot",
    "no": "noboot"
  }
}

mmt_variants = {
  "version": {
    "old": "4.1.0",
    "new": "4.2.0"
  },
  "boot": {
    "yes": "boot",
    "no": "noboot"
  }
}

mmt_file = "Master_Tinting_{version}-{boot}.hex"

sccb_boot_file = "BootLoaderSCCB_{version}.hex"
sccb_boot_variants = {
   "version": {
       "old": "1.5.1",
       "new": "1.5.2"
   }
}

mmt_boot_file = "bootloaderTINTING_{version}_MASTER.hex"
mmt_boot_variants = {
   "version": {
       "old": "1_9_2",
       "new": "1_9_3"
   }
}


tests = [
{
#N°1
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  >= 1.9.3   >= 4.2.0
#SLAVE 8  |  >= 1.5.2   >= 4.2.0
    "mmt_result_version": (4, 2, 0),
    "sccb_8_result_version": (4, 2, 0),
    "sccb_1_result_version": None,
    "mmt_variants": {"version": "new", "boot": "yes"},
    "mmt_boot_variants": {"version": "new"},
    "sccb_8_variants": {"version": "new", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "new"},
    "consider_sccb_1": False
},
{
#N°2
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  >= 1.9.3   < 4.2.0
#SLAVE 8  |  >= 1.5.2   >= 4.2.0
    "mmt_result_version": None,
    "sccb_8_result_version": None,
    "sccb_1_result_version": None,    
    "mmt_variants": {"version": "old", "boot": "yes"},
    "mmt_boot_variants": {"version": "new"},
    "sccb_8_variants": {"version": "new", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "new"},
    "consider_sccb_1": False    
},
{
#N°3
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  >= 1.9.3   >= 4.2.0
#SLAVE 8  |  >= 1.5.2   < 4.2.0
    "mmt_result_version": (4, 2, 0),
    "sccb_8_result_version": None,
    "sccb_1_result_version": None,        
    "mmt_variants": {"version": "new", "boot": "yes"},
    "mmt_boot_variants": {"version": "new"},
    "sccb_8_variants": {"version": "old", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "new"},
    "consider_sccb_1": False    
},
{
#N°4
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  < 1.9.3   >= 4.2.0
#SLAVE 8  |  >= 1.5.2   >= 4.2.0
    "mmt_result_version": None,
    "sccb_8_result_version": None,
    "sccb_1_result_version": None,    
    "sccb_8_variants": {"version": "new", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "new"},
    "mmt_variants": {"version": "new", "boot": "yes"},
    "mmt_boot_variants": {"version": "old"},
    "consider_sccb_1": False    
},
{
#N°5
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  >= 1.9.3   >= 4.2.0
#SLAVE 8  |  < 1.5.2   >= 4.2.0
    "mmt_result_version": (4, 2, 0),
    "sccb_8_result_version": None,
    "sccb_1_result_version": None,    
    "sccb_8_variants": {"version": "new", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "old"},
    "mmt_variants": {"version": "new", "boot": "yes"},
    "mmt_boot_variants": {"version": "new"},
    "consider_sccb_1": False    
},
{
#N°5_BIS
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  >= 1.9.3   < 4.2.0
#SLAVE 8  |  >= 1.5.2   >= 4.2.0
#SLAVE 1  |  >= 1.5.2   >= 4.2.0
    "mmt_result_version": None,
    "sccb_8_result_version": None,
    "sccb_1_result_version": None,    
    "mmt_variants": {"version": "old", "boot": "yes"},
    "mmt_boot_variants": {"version": "new"},
    "sccb_8_variants": {"version": "new", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "new"},
    "sccb_1_variants": {"version": "new", "boot": "yes"},
    "sccb_1_boot_variants": {"version": "new"},    
    "consider_sccb_1": True  
},
{
#N°6
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  >= 1.9.3   >= 4.2.0
#SLAVE 8  |  >= 1.5.2   >= 4.2.0
#SLAVE 1  |  >= 1.5.2   < 4.2.0
    "mmt_result_version": (4, 2, 0),
    "sccb_8_result_version": (4, 2, 0),
    "sccb_1_result_version": None,    
    "mmt_variants": {"version": "new", "boot": "yes"},
    "mmt_boot_variants": {"version": "new"},
    "sccb_8_variants": {"version": "new", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "new"},
    "sccb_1_variants": {"version": "old", "boot": "yes"},
    "sccb_1_boot_variants": {"version": "new"},    
    "consider_sccb_1": True  
},
{
#N°7
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  < 1.9.3   >= 4.2.0
#SLAVE 8  |  >= 1.5.2   >= 4.2.0
#SLAVE 1  |  >= 1.5.2   >= 4.2.0
    "mmt_result_version": None,
    "sccb_8_result_version": None,
    "sccb_1_result_version": None,    
    "mmt_variants": {"version": "new", "boot": "yes"},
    "mmt_boot_variants": {"version": "old"},
    "sccb_8_variants": {"version": "new", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "new"},
    "sccb_1_variants": {"version": "new", "boot": "yes"},
    "sccb_1_boot_variants": {"version": "new"},    
    "consider_sccb_1": True  
},
{
#N°8
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  >= 1.9.3   >= 4.2.0
#SLAVE 8  |  >= 1.5.2   >= 4.2.0
#SLAVE 1  |  <  1.5.2   >= 4.2.0
    "mmt_result_version": (4, 2, 0),
    "sccb_8_result_version": (4, 2, 0),
    "sccb_1_result_version": None,    
    "mmt_variants": {"version": "new", "boot": "yes"},
    "mmt_boot_variants": {"version": "new"},
    "sccb_8_variants": {"version": "new", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "new"},
    "sccb_1_variants": {"version": "new", "boot": "yes"},
    "sccb_1_boot_variants": {"version": "old"},    
    "consider_sccb_1": True  
},
{
#N°9
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  ASSENTE    >= 4.2.0
#SLAVE 8  |  >= 1.5.2   >= 4.2.0
#SLAVE 1  |  >= 1.5.2   >= 4.2.0
    "mmt_result_version": None,
    "sccb_8_result_version": None,
    "sccb_1_result_version": None,    
    "mmt_variants": {"version": "new", "boot": "no"},
    "mmt_boot_variants": None,
    "sccb_8_variants": {"version": "new", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "new"},
    "sccb_1_variants": {"version": "new", "boot": "yes"},
    "sccb_1_boot_variants": {"version": "new"},
    "consider_sccb_1": True  
},
{
#N°10
#            FW BOOT   FW APPLICATIVO
#---------+-------------------------
#MMT      |  >=1.9.3    >= 4.2.0
#SLAVE 8  |  >= 1.5.2   >= 4.2.0
#SLAVE 1  |  ASSENTE    >= 4.2.0
    "mmt_result_version": (4, 2, 0),
    "sccb_8_result_version": (4, 2, 0),
    "sccb_1_result_version": None,    
    "mmt_variants": {"version": "new", "boot": "yes"},
    "mmt_boot_variants":  {"version": "new"},
    "sccb_8_variants": {"version": "new", "boot": "yes"},
    "sccb_8_boot_variants": {"version": "new"},
    "sccb_1_variants": {"version": "new", "boot": "no"},
    "sccb_1_boot_variants": None,
    "consider_sccb_1": True  
}
]
