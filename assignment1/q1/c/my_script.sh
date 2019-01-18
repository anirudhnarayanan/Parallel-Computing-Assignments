#!/bin/bash
# ===== PBS OPTIONS =====
### Set the job name
#PBS -N "MyJob"
### Run in the queue named "mamba"
#PBS -q mamba
### Specify the number of cpus for your job.
#PBS -l nodes=4:ppn=4
# ==== Main ======
/users/anaraya7/firstsubmit
