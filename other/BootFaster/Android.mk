#LOCAL_PATH := $(call my-dir)
#include $(LOCAL_PATH)/PreDex.mk
#$(shell dex2oat --image=$(OUT)/system/BootFaster/data/dalvik-cache/arm/boot.art --dex-file=$(OUT)/system/framework/core-libart.jar --dex-file=$(OUT)/system/framework/conscrypt.jar --dex-file=$(OUT)/system/framework/okhttp.jar --dex-file=$(OUT)/system/framework/core-junit.jar --dex-file=$(OUT)/system/framework/bouncycastle.jar --dex-file=$(OUT)/system/framework/ext.jar --dex-file=$(OUT)/system/framework/framework.jar --dex-file=$(OUT)/system/framework/telephony-common.jar --dex-file=$(OUT)/system/framework/voip-common.jar --dex-file=$(OUT)/system/framework/ims-common.jar --dex-file=$(OUT)/system/framework/mms-common.jar --dex-file=$(OUT)/system/framework/android.policy.jar --dex-file=$(OUT)/system/framework/apache-xml.jar --dex-file=$(OUT)/system/framework/mediatek-common.jar --dex-file=$(OUT)/system/framework/mediatek-framework.jar --dex-file=$(OUT)/system/framework/mediatek-telephony-common.jar --oat-file=$(OUT)/system/BootFaster/data/dalvik-cache/arm/boot.oat --instruction-set=arm --instruction-set-features=div --base=0x6f790000 --runtime-arg -Xms64m --runtime-arg -Xmx64m --compiler-filter=verify-none --android-root=$(OUT)/system)

#$(shell mkdir -p $(OUT)/system/framework/arm)
#$(shell cp  $(OUT)/system/BootFaster/data/dalvik-cache/arm/boot.art $(OUT)/system/framework)
#$(shell cp  $(OUT)/system/BootFaster/data/dalvik-cache/arm/boot.art $(OUT)/system/framework/arm)
#$(shell cp  $(OUT)/system/BootFaster/data/dalvik-cache/arm/boot.oat $(OUT)/system/framework)
#$(shell cp  $(OUT)/system/BootFaster/data/dalvik-cache/arm/boot.oat $(OUT)/system/framework/arm)

#$(info $(shell redex $(OUT)))

