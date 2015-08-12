#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BASENAME=`basename "$DIR"`
if [ "$BASENAME" = "Resources" ]; then
    DIR=`dirname "$DIR"`
    DIR=`dirname "$DIR"`
    DIR=`dirname "$DIR"`
elif [ "$BASENAME" = "bin" ]; then
    DIR=`dirname "$DIR"`
fi

if [ "$2" != "" ]; then
    CALL_BY_PKG=YES
    DIR=$2
fi;

QMM_ROOT="$DIR"

echo ""
echo "QMM_ROOT = \"$QMM_ROOT\""
echo ""

# set Xcode
#defaults write com.apple.dt.Xcode IDEApplicationwideBuildSettings -dict-add QMM_ROOT "$QMM_ROOT"
#defaults write com.apple.dt.Xcode IDESourceTreeDisplayNames -dict-add QMM_ROOT QMM_ROOT
#IDEApplicationwideBuildSettings=`defaults read com.apple.dt.Xcode IDEApplicationwideBuildSettings`
#IDESourceTreeDisplayNames=`defaults read com.apple.dt.Xcode IDESourceTreeDisplayNames`
#
#echo "> Xcode settings updated."

# set quick player
#defaults write org.cocos.quick.player QMM_ROOT "$QMM_ROOT"
#echo "> quick player settings updated."

# set .bash_profile or .profile
if [ -f ~/.bash_profile ]; then
PROFILE_NAME=~/.bash_profile
else
PROFILE_NAME=~/.profile
fi

sed -e '/QMM_ROOT/d' $PROFILE_NAME | sed -e '/add by quick-cocos2d-x setup/d' > $PROFILE_NAME.tmp

DATE=`date "+DATE: %Y-%m-%d TIME: %H:%M:%S"`
echo "# add by quick-cocos2d-x setup, $DATE" >> $PROFILE_NAME.tmp
echo "export QMM_ROOT=\`cat ~/.QMM_ROOT\`" >> $PROFILE_NAME.tmp

DATE=`date "+%Y-%m-%d-%H%M%S"`
cp $PROFILE_NAME $PROFILE_NAME-$DATE.bak
cp $PROFILE_NAME.tmp $PROFILE_NAME
rm $PROFILE_NAME.tmp

echo "> $PROFILE_NAME updated."
echo "$QMM_ROOT" > ~/.QMM_ROOT
echo "> ~/.QMM_ROOT updated."
echo ""

if [ "$CALL_BY_PKG" != "" ]; then

    $QMM_ROOT/bin/install_luajit.sh
    ln -s $QMM_ROOT/player/mac/player.app $QMM_ROOT/player.app

# else

#     while true; do
#         read -p "Do you wish to install LuaJIT (Y/N) ? " yn
#         case $yn in
#             [Yy]* ) echo ""; $QMM_ROOT/bin/install_luajit.sh; break;;
#             [Nn]* ) exit;;
#             * ) echo "Please answer yes or no.";;
#         esac
#     done

fi;

echo ""
echo ""

echo "done."
echo ""
