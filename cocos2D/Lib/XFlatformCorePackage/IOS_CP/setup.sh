#!/usr/bin/env bash
echo "Creating folder: iOSCorePackage_config ..."

DIRECTORY="../iOSCorePackage_config"

if [ ! -d "$DIRECTORY" ]; then
  mkdir $DIRECTORY
fi

if [ -f /../iOSCorePackage_config/iOSCorePackage_config.h ]; then
    while true; do
    	read -p "File iOSCorePackage_config.h already exists. Do you want to overwrite it (y/n) ?" yn
    	case $yn in
        	[Yy]* ) cp "include/iOSCorePackage_config.h.template" "../iOSCorePackage_config/iOSCorePackage_config.h"; break;;
        	[Nn]* ) exit;;
        	* ) echo "Please answer yes or no.";;
    	esac
    done
else
	echo "Copying iOSCorePackage_config.h ..."
	cp "include/iOSCorePackage_config.h.template" "../iOSCorePackage_config/iOSCorePackage_config.h"
fi

echo "Copying user .xcconfig files ..."
cd "../iOSCorePackage_config"
mkdir xcode4
cd xcode4
mkdir ios
cd "../../iOSCorePackage"
cp -a "project/xcode4/ios/xcconfigs_iOSCorePackage/user/." "../iOSCorePackage_config/xcode4/ios/xcconfigs_iOSCorePackage"
cp -a "project/xcode4/ios/xcconfigs_test/user/." "../iOSCorePackage_config/xcode4/ios/xcconfigs_test"