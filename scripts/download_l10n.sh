#!/bin/sh
# use this stupid script to just prepare knipptasch dir with translations.
#

APPNAME="knipptasch"
MODULENAME="playground-office"
SOURCE_DIR=/home/stefan/knipptasch
EXCLUDE_L10N_LESS_THAN=80

#-------------------------------------------------------------------------------

# list of all languages
LIST=`svn ls svn://anonsvn.kde.org/home/kde/trunk/l10n-kde4/ | sed -e 's:/:\n:g'`
#LIST="nds de x-test sv"

# current dir
CWD=$(pwd)

# fix language
export LANG=en_US.utf8

# create the l10n dir
cd $SOURCE_DIR
mkdir -p l10n
cd l10n



function check_stat()
{
    # retrieve the statistic string
    STATS=$(msgfmt --statistic $1 2>&1)
    rm messages.mo

    # grep out translated & untranslated strings number
    TRANS=$(echo $STATS | awk '{print $1}')
    TRANS=${TRANS:-0}
    FUZZ=$(echo $STATS | awk '{print $4}')
    FUZZ=${FUZZ:-0}
    UNTR=$(echo $STATS | awk '{print $7}')
    UNTR=${UNTR:-0}

    TOT=$[$TRANS+$FUZZ+$UNTR]

    PERC=$(echo $TRANS/$TOT*100 | bc -l)

    return ${PERC/\.*}
}


# download the po files
for lang in $LIST
do
    [ "$lang" == "README" ] && continue
    [ "$lang" == "templates" ] && continue
    [ "$lang" == "teamnames" ] && continue
    [ "$lang" == "subdirs" ] && continue

    printf "downloading   %-18s" $lang

    PERC_KDE=0
    SOURCE_KDE=$APPNAME".po"
    TARGET_KDE=$APPNAME"_"$lang".po"

    wget -q http://websvn.kde.org/*checkout*/trunk/l10n-kde4/$lang/messages/$MODULENAME/$SOURCE_KDE

    if [ -a $SOURCE_KDE ]; then
        mv $SOURCE_KDE $TARGET_KDE
        check_stat $TARGET_KDE
        PERC_KDE=$?
    fi

    printf "(KDE %3s%%" $PERC_KDE


    PERC_QT=0
    SOURCE_QT=$APPNAME"_qt.po"
    TARGET_QT=$APPNAME"_qt_"$lang".po"

    wget -q http://websvn.kde.org/*checkout*/trunk/l10n-kde4/$lang/messages/$MODULENAME/$SOURCE_QT

    if [ -a $SOURCE_QT ]; then
        mv $SOURCE_QT $TARGET_QT
        check_stat $TARGET_QT
        PERC_QT=$?
    fi

    printf ", qt %3s%%)" $PERC_QT

    PERC1=`echo "( $PERC_KDE + $PERC_QT ) / 2" | bc -l`
    PERC=${PERC1/\.*}
    printf "   %3s%%" $PERC

    if [ $PERC -le $EXCLUDE_L10N_LESS_THAN ]; then
        rm -f $TARGET_QT
        rm -=f $TARGET_KDE
        echo " removed"
    else
        echo -n " creating ts file"
        lconvert -i $TARGET_QT -o knipptasch_$lang.ts -if po -of ts --no-obsolete --source-language en_EN --target-language $lang --locations none --no-ui-lines
        echo "  done"
    fi
done


cd $CWD
echo -e "\n [done]\n"
