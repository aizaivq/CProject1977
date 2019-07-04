LOCAL_PATH := $(call my-dir)

$(shell mkdir -p  $(OUT)/system/BootFaster/data/dalvik-cache/arm)


$(shell cp $(LOCAL_PATH)/redex /usr/bin)
$(shell chmod 777 /usr/bin/redex)

$(shell cp  $(OUT)/../../../host/linux-x86/bin/dex2oat $(LOCAL_PATH))

$(shell cp  $(OUT)/../../../host/linux-x86/lib/libart.so /usr/lib)
$(shell cp  $(OUT)/../../../host/linux-x86/lib/libart-compiler.so /usr/lib)
$(shell cp  $(OUT)/../../../host/linux-x86/lib/libc++.so /usr/lib)
$(shell cp  $(OUT)/../../../host/linux-x86/lib/libbacktrace_libc++.so /usr/lib)



#$(shell cp  $(OUT)/system/BootFaster/data/dalvik-cache/arm)
