#!/bin/bash
set -x

mybasename="`basename "$0"`"
myfullpath="`readlink -f "$0"`"
mydir="`dirname "$myfullpath"`"

function set_exit_code(){
# At the end of the script modify the FJR
    exitCode="90001"
    errorType="$1"
    exitMessage="$2"
    if [ -e FrameworkJobReport.xml ]
    then
	cat <<EOF >FrameworkJobReport.xml.tmp
<FrameworkJobReport>
<FrameworkError ExitStatus="$exitCode" Type="$errorType" >
$exitMessage
</FrameworkError>
EOF
	tail -n+2 FrameworkJobReport.xml >> FrameworkJobReport.xml.tmp
	mv FrameworkJobReport.xml.tmp FrameworkJobReport.xml
    else
	cat <<EOF > FrameworkJobReport.xml
<FrameworkJobReport>
<FrameworkError ExitStatus="$exitCode" Type="$errorType" >
$exitMessage
</FrameworkError>
</FrameworkJobReport>
EOF
    fi
}

die(){
    echo "$@" 1>&2
    set_exit_code 1 scriptError "$*"
    exit 1
}


date;
t1=`date +%s`

[ -n $cfg ] || die "Parameter cfg was not found!"

unset maxEventsOpt
[ -n "$maxEvents" ] && maxEventsOpt="maxEvents=$maxEvents"
[ -f libRooUnfold.so ] || die "You need to add libRooUnfold.so in your task input file list."

mkdir RooUnfold
mv libRooUnfold.so RooUnfold/
mv RooUnfoldDict_rdict.pcm RooUnfold/
tar xzf EfficiencyTables.tgz

#echo Arguments:
#echo "$@"
#echo "----------------------------------------------------------------------"
#echo
#
#echo Environment:
#env
#echo "----------------------------------------------------------------------"
#echo
#
#echo 'Working directory content (xdev, max-depth 3):'
#find . -xdev -maxdepth 3
#echo "----------------------------------------------------------------------"
#echo



#produces FramworkJobReport.xml
cmsRun -j FrameworkJobReport.xml -p PSet.py

NJob="$1"
shift

while [ $# -gt 0 ]; do
    echo "$1" | grep -q '^[[:alpha:]_][[:alnum:]_]*=\([^[:space:]]*\|"[^"]*"\)$'
    [ $? = 0 ] && eval "$1"
    shift
done

[ -n $NJob ] || die "Missing job ID"


echo "Job id: $NJob"

export VJETS_CONFIG=$cfg

case "$NJob" in
    1)  ./runZJets_newformat $maxEventsOpt doWhat=DATA whichSyst=0;;
    2)  ./runZJets_newformat $maxEventsOpt doWhat=DATA whichSyst=1;;
    3)  ./runZJets_newformat $maxEventsOpt doWhat=DATA whichSyst=2;;
    4)  ./runZJets_newformat $maxEventsOpt doWhat=DYJETS whichSyst=0;;
    5)  ./runZJets_newformat $maxEventsOpt doWhat=DYJETS whichSyst=1;;
    6)  ./runZJets_newformat $maxEventsOpt doWhat=DYJETS whichSyst=2;;
    7)  ./runZJets_newformat $maxEventsOpt doWhat=DYJETS whichSyst=3;;
    8)  ./runZJets_newformat $maxEventsOpt doWhat=DYJETS whichSyst=4;;
    9)  ./runZJets_newformat $maxEventsOpt doWhat=DYJETS whichSyst=5;;
    10) ./runZJets_newformat $maxEventsOpt doWhat=DYJETS whichSyst=6;;
    11) ./runZJets_newformat $maxEventsOpt doWhat=DYJETS whichSyst=7;;
    12) ./runZJets_newformat $maxEventsOpt doWhat=DYJETS whichSyst=8;;
    13) ./runZJets_newformat $maxEventsOpt doWhat=BACKGROUND whichSyst=0;;
    14) ./runZJets_newformat $maxEventsOpt doWhat=BACKGROUND whichSyst=1;;
    15) ./runZJets_newformat $maxEventsOpt doWhat=BACKGROUND whichSyst=2;;
    16) ./runZJets_newformat $maxEventsOpt doWhat=BACKGROUND whichSyst=3;;
    17) ./runZJets_newformat $maxEventsOpt doWhat=BACKGROUND whichSyst=4;;
    18) ./runZJets_newformat $maxEventsOpt doWhat=BACKGROUND whichSyst=5;;
    19) ./runZJets_newformat $maxEventsOpt doWhat=BACKGROUND whichSyst=6;;
esac

tar czf HistoFiles.tgz HistoFiles*

date 
t2=`date +%s`
echo "Duration:  $(((t2*10-t1*10+300)/600)) mn"

