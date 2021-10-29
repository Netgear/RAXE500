#!/bin/sh

PROFILE=$1
echo "Link acos files, PROFILE=$PROFILE."
CURDIR=$(pwd)
USERAPPS_DIR=$CURDIR/userspace
ACOSTOPDIR=$USERAPPS_DIR/ap/acos
PROJECT_ACOSTOPDIR=$USERAPPS_DIR/project/acos

ln -sf $PROJECT_ACOSTOPDIR/include/ambitCfg_WW_$PROFILE.h $ACOSTOPDIR/include/ambitCfg.h
ln -sf $PROJECT_ACOSTOPDIR/include/projectConfig_$PROFILE.h $ACOSTOPDIR/include/projectConfig.h
cp -f $PROJECT_ACOSTOPDIR/config_$PROFILE.mk $PROJECT_ACOSTOPDIR/config.mk
#ln -sf $PROJECT_ACOSTOPDIR/wlanLib/rf_util_$PROFILE.c $ACOSTOPDIR/wlanLib/rf_util.c
#rm $ACOSTOPDIR/httpd/cgi/mnuCgi.c; ln -s $PROJECT_ACOSTOPDIR/httpd/cgi/mnuCgi_$PROFILE.c $ACOSTOPDIR/httpd/cgi/mnuCgi.c
#rm $ACOSTOPDIR/httpd_mt/cgi/mnuCgi.c; ln -s $PROJECT_ACOSTOPDIR/httpd_mt/cgi/mnuCgi_$PROFILE.c $ACOSTOPDIR/httpd_mt/cgi/mnuCgi.c

#if [ -f "$PROJECT_ACOSTOPDIR/lib/libproject_dep/shared_lib_project_dependent_$PROFILE.c" ]; then
#cp -f $PROJECT_ACOSTOPDIR/lib/libproject_dep/shared_lib_project_dependent_$PROFILE.c $PROJECT_ACOSTOPDIR/lib/libproject_dep/shared_lib_project_dependent.c
#fi

#feature_enabled=$(awk '/CONFIG_MULTI_THREAD_HTTPD=y/ {print}' $PROJECT_ACOSTOPDIR/config.mk)
#if [[ -n "$feature_enabled" ]]; then
#rm $ACOSTOPDIR/httpd_mt/cgi/mnuCgi.c; ln -s $PROJECT_ACOSTOPDIR/httpd_mt/cgi/mnuCgi_$PROFILE.c $ACOSTOPDIR/httpd_mt/cgi/mnuCgi.c
#fi

#feature_enabled=$(awk '/CONFIG_USE_KERNEL_NTFS=y/ {print}' $PROJECT_ACOSTOPDIR/config.mk)
#if [[ -n "$feature_enabled" ]]; then
#    cp -f $PROJECT_ACOSTOPDIR/ufsd/ufsd_${PROFILE}.ko $ACOSTOPDIR/ufsd/ufsd.ko
#    cp -f $PROJECT_ACOSTOPDIR/ufsd/jnl_${PROFILE}.ko $ACOSTOPDIR/ufsd/jnl.ko
#    cp -f $PROJECT_ACOSTOPDIR/ufsd/ufsd_${PROFILE} $ACOSTOPDIR/ufsd/ufsd
#fi

#rm -rf $ACOSTOPDIR/led_blinkd; ln -s $PROJECT_ACOSTOPDIR/led_blinkd_$PROFILE $ACOSTOPDIR/led_blinkd
#rm -rf $ACOSTOPDIR/te_test_daemon; ln -s $PROJECT_ACOSTOPDIR/te_test_daemon/te_test_daemon_$PROFILE $ACOSTOPDIR/te_test_daemon
#rm $ACOSTOPDIR/shared/lan_util.c; ln -s $PROJECT_ACOSTOPDIR/shared/lan_util_$PROFILE.c $ACOSTOPDIR/shared/lan_util.c
#rm $ACOSTOPDIR/shared/lan_util.h; ln -s $PROJECT_ACOSTOPDIR/shared/lan_util_$PROFILE.h $ACOSTOPDIR/shared/lan_util.h
#rm $ACOSTOPDIR/shared/led_util.c; ln -s $PROJECT_ACOSTOPDIR/shared/led_util_$PROFILE.c $ACOSTOPDIR/shared/led_util.c
#rm $ACOSTOPDIR/shared/led_util.h; ln -s $PROJECT_ACOSTOPDIR/shared/led_util_$PROFILE.h $ACOSTOPDIR/shared/led_util.h
#rm $ACOSTOPDIR/shared/i2c_util.c; ln -s $PROJECT_ACOSTOPDIR/shared/i2c_util.c $ACOSTOPDIR/shared/i2c_util.c
#rm $ACOSTOPDIR/shared/i2c_util.h; ln -s $PROJECT_ACOSTOPDIR/shared/i2c_util.h $ACOSTOPDIR/shared/i2c_util.h
#rm $ACOSTOPDIR/shared/thermal_util_PCT2075.c; ln -s $PROJECT_ACOSTOPDIR/shared/thermal_util_PCT2075.c $ACOSTOPDIR/shared/thermal_util_PCT2075.c
#rm $ACOSTOPDIR/shared/thermal_util_PCT2075.h; ln -s $PROJECT_ACOSTOPDIR/shared/thermal_util_PCT2075.h $ACOSTOPDIR/shared/thermal_util_PCT2075.h
#rm $ACOSTOPDIR/shared/thermal_util_m7533.c; ln -s $PROJECT_ACOSTOPDIR/shared/thermal_util_m7533.c $ACOSTOPDIR/shared/thermal_util_m7533.c
#rm $ACOSTOPDIR/shared/thermal_util_m7533.h; ln -s $PROJECT_ACOSTOPDIR/shared/thermal_util_m7533.h $ACOSTOPDIR/shared/thermal_util_m7533.h


#feature_enabled=$(awk '/CONFIG_ACOS_SHARED_NVRAM=y/ {print}' $PROJECT_ACOSTOPDIR/config.mk)
#if [[ -n "$feature_enabled" ]]; then
#    echo "######## CONFIG_ACOS_SHARED_NVRAM is enabled."
#    cp -f $PROJECT_ACOSTOPDIR/acos_nvram/project_defaults_$PROFILE.c $PROJECT_ACOSTOPDIR/acos_nvram/project_defaults.c
#    cp -f $PROJECT_ACOSTOPDIR/acos_nvram/psi_default_$PROFILE $PROJECT_ACOSTOPDIR/acos_nvram/psi_default
#else
#    echo "######## CONFIG_ACOS_SHARED_NVRAM is not enabled, use BRCM nvram."
#    cp -f $PROJECT_ACOSTOPDIR/nvram/project_defaults_$PROFILE.h $ACOSTOPDIR/nvram/project_defaults.h
#fi

#feature_enabled=$(awk '/CONFIG_ACOS_GPIO=y/ {print}' $PROJECT_ACOSTOPDIR/config.mk)
#if [[ -n "$feature_enabled" ]]; then
#    echo "######## CONFIG_ACOS_GPIO is enabled."
#    ln -sf ${PROJECT_ACOSTOPDIR}/acos_gpio/gpio_kern_${PROFILE} ${ACOSTOPDIR}/acos_gpio/gpio_kern
#    rm ${ACOSTOPDIR}/acos_gpio/gpio_kern/gpio_kern_${PROFILE}
#fi

#feature_enabled=$(awk '/CONFIG_DAL_FOR_ARMOR=y/ {print}' $PROJECT_ACOSTOPDIR/config.mk)
#if [[ -n "$feature_enabled" ]]; then
#    echo "######## CONFIG_DAL_FOR_ARMOR is enabled."
#    cp -f $PROJECT_ACOSTOPDIR/bitdefender/bd_version_$PROFILE $PROJECT_ACOSTOPDIR/bitdefender/bd_version
#    cp -f $PROJECT_ACOSTOPDIR/bitdefender/phase2_$PROFILE.tar.gz $ACOSTOPDIR/bitdefender/phase2.tar.gz
#	cp -f $PROJECT_ACOSTOPDIR/bitdefender/bd_version $ACOSTOPDIR/bitdefender/bd_version
#	cp -f $PROJECT_ACOSTOPDIR/bitdefender/change_cloud_server.sh $ACOSTOPDIR/bitdefender/change_cloud_server.sh
#	cp -f $PROJECT_ACOSTOPDIR/bitdefender/phase2-upd.tar.gz $ACOSTOPDIR/bitdefender/phase2-upd.tar.gz
#	cp -f $PROJECT_ACOSTOPDIR/bitdefender/bdupd/bdupd_start_schedule.sh $ACOSTOPDIR/bitdefender/bdupd_start_schedule.sh
#	cp -f $PROJECT_ACOSTOPDIR/bitdefender/bdupd/bdupd_start.sh $ACOSTOPDIR/bitdefender/bdupd_start.sh
#	cp -f $PROJECT_ACOSTOPDIR/bitdefender/hosts $ACOSTOPDIR/bitdefender/hosts
#	mkdir -p $ACOSTOPDIR/bitdefender/logging
#	cp -f $PROJECT_ACOSTOPDIR/bitdefender/logging/* $ACOSTOPDIR/bitdefender/logging/
#fi

#feature_enabled=$(awk '/CONFIG_ROUTER_AX_GUI=y/ {print}' $PROJECT_ACOSTOPDIR/config.mk)
#if [[ -n "$feature_enabled" ]]; then
#    echo "######## CONFIG_ROUTER_AX_GUI is enabled."
#    rm $ACOSTOPDIR/www_router_ax/DashBoard.htm; ln -sf $PROJECT_ACOSTOPDIR/www_router_ax/DashBoard_$PROFILE.htm $ACOSTOPDIR/www_router_ax/DashBoard.htm
#	rm $ACOSTOPDIR/www_router_ax/utility.js; ln -sf $PROJECT_ACOSTOPDIR/www_router_ax/utility_$PROFILE.js $ACOSTOPDIR/www_router_ax/utility.js
#	rm $ACOSTOPDIR/www_router_ax/start.htm; ln -sf $PROJECT_ACOSTOPDIR/www_router_ax/start_${PROFILE}_noDownloader.htm $ACOSTOPDIR/www_router_ax/start.htm
#	cp -f $ACOSTOPDIR/www_router_ax/UPNP_media_$PROFILE.htm $ACOSTOPDIR/www_router_ax/UPNP_media.htm
	
	#if [ $PROFILE = "RAX50" ]; then
#	if [ -f "$ACOSTOPDIR/www_router_ax/WLG_wireless_dual_band_r10_$PROFILE.htm" ]; then
#        cp -f $ACOSTOPDIR/www_router_ax/WLG_wireless_dual_band_r10_$PROFILE.htm $ACOSTOPDIR/www_router_ax/WLG_wireless_dual_band_r10.htm
#    fi
    #fi
#	if [ -f "$ACOSTOPDIR/www_router_ax/WLG_wireless_tri_band_$PROFILE.htm" ]; then
#        cp -f $ACOSTOPDIR/www_router_ax/WLG_wireless_tri_band_$PROFILE.htm $ACOSTOPDIR/www_router_ax/WLG_wireless_tri_band.htm
#	fi
#fi

#feature_enabled=$(awk '/CONFIG_ORBI_GUI=y/ {print}' $PROJECT_ACOSTOPDIR/config.mk)
#if [[ -n "$feature_enabled" ]]; then
#    echo "######## CONFIG_ORBI_GUI is enabled."
#    rm $ACOSTOPDIR/www_orbi/DashBoard.htm; ln -sf $PROJECT_ACOSTOPDIR/www_orbi/DashBoard_$PROFILE.htm $ACOSTOPDIR/www_orbi/DashBoard.htm
#	rm $ACOSTOPDIR/www_orbi/utility.js; ln -sf $PROJECT_ACOSTOPDIR/www_orbi/utility_$PROFILE.js $ACOSTOPDIR/www_orbi/utility.js
#	rm $ACOSTOPDIR/www_orbi/start.htm; ln -sf $PROJECT_ACOSTOPDIR/www_orbi/start_${PROFILE}_noDownloader.htm $ACOSTOPDIR/www_orbi/start.htm
#	rm $ACOSTOPDIR/www_orbi/WLG_wireless2.htm; ln -sf $PROJECT_ACOSTOPDIR/www_orbi/WLG_wireless2_$PROFILE.htm $ACOSTOPDIR/www_orbi/WLG_wireless2.htm
#	rm $ACOSTOPDIR/www_orbi/WLG_wireless_tri_band.htm; ln -sf $PROJECT_ACOSTOPDIR/www_orbi/WLG_wireless_tri_band_$PROFILE.htm $ACOSTOPDIR/www_orbi/WLG_wireless_tri_band.htm
#	rm $ACOSTOPDIR/www_orbi/WLG_adv_dual_band2.htm; ln -sf $PROJECT_ACOSTOPDIR/www_orbi/WLG_adv_dual_band2_$PROFILE.htm $ACOSTOPDIR/www_orbi/WLG_adv_dual_band2.htm
#	rm $ACOSTOPDIR/www_orbi/WLG_adv_tri_band2.htm; ln -sf $PROJECT_ACOSTOPDIR/www_orbi/WLG_adv_tri_band2_$PROFILE.htm $ACOSTOPDIR/www_orbi/WLG_adv_tri_band2.htm
#	rm $ACOSTOPDIR/www_orbi/RST_stattbl.htm; ln -sf $PROJECT_ACOSTOPDIR/www_orbi/RST_stattbl_$PROFILE.htm $ACOSTOPDIR/www_orbi/RST_stattbl.htm
#	rm $ACOSTOPDIR/www_orbi_satellite/start.htm; ln -sf $PROJECT_ACOSTOPDIR/www_orbi_satellite/start_$PROFILE.htm $ACOSTOPDIR/www_orbi_satellite/start.htm
#	rm $ACOSTOPDIR/www_orbi_satellite/ADVANCED_home2.htm; ln -sf $PROJECT_ACOSTOPDIR/www_orbi_satellite/ADVANCED_home2_$PROFILE.htm $ACOSTOPDIR/www_orbi_satellite/ADVANCED_home2.htm
#	cp -f $ACOSTOPDIR/www_orbi/UPNP_media_$PROFILE.htm $ACOSTOPDIR/www_orbi/UPNP_media.htm
#fi

#feature_enabled=$(awk '/CONFIG_LED_CONTROL_PAGE=y/ {print}' $PROJECT_ACOSTOPDIR/config.mk)
#if [[ -n "$feature_enabled" ]]; then
#    echo "######## CONFIG_LED_CONTROL_PAGE is enabled."
#  	ln -sf $PROJECT_ACOSTOPDIR/www_router_ax/LED_settings_$PROFILE.htm $ACOSTOPDIR/www_router_ax/LED_settings.htm
#fi

#if [ -f "${CURDIR}/targets/fs.src/etc/JP_string_table" ]; then
#    echo "######## Preload JP_string_table exist, skip..."
#else
#    echo "######## Copy preload string_table - Spanish, Chinese, French, Germany, Italian, Dutch, Korea, Swedish, Russian, Portuguese, Japanese"
#    ${USERAPPS_DIR}/project/acos/strings/gen_stringtable.sh ${USERAPPS_DIR}/project/acos/strings ${PROFILE}
#    cd ${CURDIR}/targets/fs.src/etc; rm -f string_table*;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*Eng-Language-table ${CURDIR}/targets/fs.src/etc/Eng_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*SP-Language-table ${CURDIR}/targets/fs.src/etc/SP_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*PR-Language-table ${CURDIR}/targets/fs.src/etc/PR_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*FR-Language-table ${CURDIR}/targets/fs.src/etc/FR_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*GR-Language-table ${CURDIR}/targets/fs.src/etc/GR_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*IT-Language-table ${CURDIR}/targets/fs.src/etc/IT_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*NL-Language-table ${CURDIR}/targets/fs.src/etc/NL_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*KO-Language-table ${CURDIR}/targets/fs.src/etc/KO_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*SV-Language-table ${CURDIR}/targets/fs.src/etc/SV_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*RU-Language-table ${CURDIR}/targets/fs.src/etc/RU_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*PT-Language-table ${CURDIR}/targets/fs.src/etc/PT_string_table;
#    cp -f ${USERAPPS_DIR}/project/acos/strings/${PROFILE}-preload-stringtable/*JP-Language-table ${CURDIR}/targets/fs.src/etc/JP_string_table;
#fi
	
echo "######## Copy files for old design, to be modified..."
#if [ -f "$PROJECT_ACOSTOPDIR/nvram/defaults_$PROFILE.c" ]; then
#    cp -f $PROJECT_ACOSTOPDIR/nvram/defaults_$PROFILE.c $PROJECT_ACOSTOPDIR/nvram/defaults.c
#fi
#rm ${ACOSTOPDIR}/shared/rf_util.c; ln -s ${PROJECT_ACOSTOPDIR}/shared/rf_util_${PROFILE}.c ${ACOSTOPDIR}/shared/rf_util.c
#rm ${ACOSTOPDIR}/www/start.htm; ln -s ${PROJECT_ACOSTOPDIR}/www/start_${PROFILE}_noDownloader.htm ${ACOSTOPDIR}/www/start.htm
