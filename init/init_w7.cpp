/*
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (C) 2017, The LineageOS Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <fstream>
#include <string>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/properties.h>

#include "property_service.h"
#include "vendor_init.h"
#include "log.h"

using android::base::GetProperty;
using namespace android::init;

bool check_cmdline(const std::string &param){
    std::ifstream file("/proc/cmdline");
    if (file.good()) {
        std::string line;
        while (std::getline(file, line, ' ')) {
            if(line == param)
                return true;
        }
    }
    return false;
}

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_dual(char const system_prop[], char const vendor_prop[], char const value[])
{
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

void vendor_load_properties() {
    std::string serial = GetProperty("ro.boot.serialno", "");

    if (serial.substr(0,6) ==  "LGD410") {
        /* D410, D410hn */
        if (check_cmdline("model.name=LG-D410hn")) {
                property_override_dual("ro.product.device", "ro.vendor.product.device", "w7nds");
                property_override_dual("ro.product.model", "ro.vendor.product.model", "LG-D410hn");
                property_set("ro.nfc.port", "I2C");
        } else {
                property_override("ro.product.device", "w7ds");
                property_override("ro.product.model", "LG-D410");
        }
        property_override("ro.build.description", "w7ds_global_com-user 5.0.2 LRX22G.A1423481010 15040202274a7 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "lge/w7ds_global_com/w7ds:5.0.2/LRX22G.A1423481010/15040202274a7:user/release-keys");
        property_set("persist.multisim.config", "dsds");
        property_set("persist.radio.multisim.config", "dsds");
        property_set("persist.radio.dont_use_dsd", "true");
        property_set("ro.telephony.ril.config", "simactivation");
    } else if (serial.substr(0,6) ==  "LGD405") {
        /* D405, D405n */
        if (check_cmdline("model.name=LG-D405n")) {
                property_override_dual("ro.product.model", "ro.vendor.product.model", "LG-D405n");
                property_override_dual("ro.product.device", "ro.vendor.product.device", "w7n");
                property_set("ro.nfc.port", "I2C");
        } else {
                property_override_dual("ro.product.model", "ro.vendor.product.model", "LG-D405");
                property_override_dual("ro.product.device", "ro.vendor.product.device", "w7");
        }
        property_override("ro.build.description", "w7_global_com-user 5.0.2 LRX22G.A1423481010 15040202274a7 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "lge/w7_global_com/w7ds:5.0.2/LRX22G.A1423481010/15040202274a7:user/release-keys");
        property_set("persist.radio.multisim.config", "");
        property_set("persist.multisim.config", "");
    } else if (serial.substr(0,6) ==   "LGD415") {
        /* D415 */
        property_override_dual("ro.product.model", "ro.vendor.product.model", "LG-D415");
        property_override_dual("ro.product.device", "ro.vendor.product.device", "w7");
        property_override("ro.build.description", "w7_global_com-user 5.0.2 LRX22G.A1423481010 15040202274a7 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "lge/w7_global_com/w7ds:5.0.2/LRX22G.A1423481010/15040202274a7:user/release-keys");
        property_set("persist.radio.multisim.config", "");
        property_set("persist.multisim.config", "");
    } else {
        /* XXX */
        property_override_dual("ro.product.device", "ro.vendor.product.device", "w7");
        property_override_dual("ro.product.model", "ro.vendor.product.model", "Please write your model name to agent00791@gmail.com");
        property_set("persist.radio.multisim.config", "");
        property_set("persist.multisim.config", "");
    }

    std::string device = GetProperty("ro.product.device", "");

    //LOG(ERROR) << "Found hardware id: '" << serial.c_str() << "' setting build properties for '" << device.c_str() << "' device\n";
}
