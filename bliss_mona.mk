#
# Copyright (C) 2022 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit_only.mk)
TARGET_SUPPORTS_OMX_SERVICE := false
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit from mona device
$(call inherit-product, device/motorola/mona/device.mk)

# foss / microg apps
#$(call inherit-product-if-exists, vendor/foss/foss.mk)
PRODUCT_BROKEN_VERIFY_USES_LIBRARIES := true
BUILD_BROKEN_SRC_DIR_IS_WRITABLE := true 

#  Gapps Pkg
#TARGET_GAPPS_VARIANT := pico
#TARGET_GAPPS_VARIANT := nano
#TARGET_GAPPS_VARIANT := micro
TARGET_GAPPS_VARIANT := full
#TARGET_GAPPS_VARIANT := stock

# MicroG
#ifeq ($BLISS-BUILD-VARIANT), microg)@
#$(call inherit-product-if-exists, vendor/partner_gms)
#endif

# UDFPS icon + animation pickers (vendor/addons)
TARGET_HAS_UDFPS := true

# Inherit some common Lineage stuff.
$(call inherit-product, vendor/bliss/config/common_full_phone.mk)

PRODUCT_NAME := bliss_mona
PRODUCT_DEVICE := mona
PRODUCT_MANUFACTURER := motorola
PRODUCT_BRAND := motorola
PRODUCT_MODEL := moto g stylus - 2025

PRODUCT_GMS_CLIENTID_BASE := android-motorola

PRODUCT_BUILD_PROP_OVERRIDES += \
    DeviceProduct="mona_g" \
    BuildDesc="mona_g-user 16 W1VAS36.62-22-16-4 6f094-a1197 release-keys MW-292" \
    BuildFingerprint="motorola/mona_g/mona:16/W1VAS36.62-22-16-4/6f094-a1197:user/release-keys"
