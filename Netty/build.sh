TARGET_DIR="/Users/LeeHyungRae/Desktop/socket_server_bin"
SOURCE_DIR="/Users/LeeHyungRae/Desktop/socket_server_java"

if [[ -e $SOURCE_DIR ]]
then
	echo "Move To $SOURCE_DIR"
else
	exit 9
fi

cd $SOURCE_DIR

function cleanBuild(){
	`echo ./gradlew clean`
	`echo ./gradlew build`
}

function copyLibs(){
	`echo ./gradlew copyToLib`
	return $?;
}

function copyConfs(){	
	CONF_DIR="/Users/LeeHyungRae/Desktop/socket_server_java/src/main/resources"
	TARGET_CONF_DIR="$TARGET_DIR/conf/"
	if [[ -e $TARGET_CONF_DIR ]]
	then
		cp $CONF_DIR/* $TARGET_CONF_DIR
	else
   		mkdir $TARGET_CONF_DIR
		cp $CONF_DIR/* $TARGET_CONF_DIR
	fi
}
cleanBuild
copyLibs

if [[ $? -eq 0 ]]
then
	echo 'CopyToLib Completed'
else
	echo 'CopyToLib Failed'
	exit 9
fi

copyConfs

if [[ $? -eq 0 ]]
then
	echo 'CopyConfs Completed'
else
	echo 'CopyConfs Failed'
	exit 9
fi

if [[ -e $TARGET_DIR ]]
then
	echo "Move To $TARGET_DIR"
	cp $SOURCE_DIR/build/libs/* $TARGET_DIR/
	cd $TARGET_DIR
else
	echo "Create $TARGET_DIR"
	mkdir $TARGET_DIR
	cp $SOURCE_DIR/build/libs/* $TARGET_DIR/
	cd $TARGET_DIR
fi

TARGET_LIB_DIR="$TARGET_DIR/lib"

if [[ -e $TARGET_LIB_DIR ]]
then
	`echo cp $SOURCE_DIR/lib/*.jar $TARGET_LIB_DIR`
else
	mkdir $TARGET_LIB_DIR	
	`echo cp $SOURCE_DIR/lib/*.jar $TARGET_LIB_DIR`
fi

