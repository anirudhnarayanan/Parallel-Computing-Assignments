#!/bin/sh

RESULTDIR=result/
h=`hostname`

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

# import params
#source params.sh
MERGESORT_NS="10000 1000000 1000000000"
THREADS="1 2 4 8 12 16"

# bench
echo starting time is $(date)

for n in ${MERGESORT_NS};
do
    for t in ${THREADS};
    do
	./mergesort $n $t >/dev/null 2> ${RESULTDIR}/mergesort_${n}_${t}
    done
done
	     
echo ending time is $(date)
