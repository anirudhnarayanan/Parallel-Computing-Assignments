#!/bin/sh

RESULTDIR=result/
h=`hostname`

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

# common params
THREADS="1 2 4 8 12 16"

# reduce params
REDUCE_NS="10000 1000000 100000000" 

# mergesort params
MERGESORT_NS="10000 1000000 1000000000"

# bubblesort params
BUBBLESORT_NS="1000 10000 1000000"

# lcs params
LCS_NS="100 1000 100000" 

# bench
echo starting time is $(date)

for m in ${LCS_NS};
do
   for n in ${LCS_NS};
   do
      for t in $THREADS;
      do
         if [ "${m}" -le "${n}" ] ;
         then
	    ./lcs $m $n $t >/dev/null 2> ${RESULTDIR}/lcs_${m}_${n}_${t}
         fi
      done
   done
done

echo ending time is $(date)
