#!/bin/bash

# Organise files of RecoComparison directory from shears framework by grouping them
# in subdirectories


die(){
    echo "$@" 1>&2
    exit 1
}

ckdir(){
    [ -d "$1" ] || mkdir "$1"
}

for f in *; do d=`echo $f | awk -F _ '{print $1}'`; [ -d $d ] || mkdir $d; [ -f $f ] && cp -a "$f" "$d"; done


ckdir Observables
for obs in Pt Eta Rapidity Phi HT Mass PtDiff; do
    ckdir "Observables/$obs"
    for d in *${obs}{,Full,High,LowPU}; do
	mv -i "$d" "Observables/$obs/"
    done
done

for obs in DPhi dEta dR tau PHI PU ZNGoodJets ptBal SpT SPhi; do
    ckdir Observables/$obs
    for d in ${obs}*; do
	[ -d "$d" ] &&  mv -i "$d" Observables/$obs/
    done
done

mv -i dPhi* Observables/DPhi/
mv -i ZMassFrom60 Observables/Mass/
mv -i ZEtaUpTo5 Observables/Eta/
mv -i BestPHI Observables/PHI/
mv -i BestdPhiJets Observables/dPhi/
mv -i BestSpT* Observables/SpT/
mv -i NVtx.pdf NVtx/
mv -i NVtx Observables/

rm -r Lumi.pdf Lumi
[ -f puMVA.pdf -a -f puMVA ] && mv -i puMVA.pdf  puMVA/
[ -f puMVA_Lin.pdf -a -f puMVA ] && mv -i puMVA_Lin.pdf  puMVA/
[ -d Observables/ZNGoodJets/ZNGoodJets ] && { mv -i Observables/ZNGoodJets/ZNGoodJets/* Observables/ZNGoodJets/ && rmdir Observables/ZNGoodJets/ZNGoodJets; }