import math
import sys, os, getpass, shutil
from os import environ
from ROOT import TFile, TObjString

# MYDIR          : Output directory 
# inputFileList  : File containing list of eta values 
# macrosTopDir   : Directory for input files
# nEventsPerJob  : Jobs will be broken up into this many events per job
# nTotalEvents   : Maximum number of events to process when summing all jobs.

class pars:
    MYDIR='/work/eic3/users/nwickjlb/time_imaging_hpDIRC_ePIC_B_off'
    inputThetaList='theta_values.list'
    inputPhiList='phi_value.list'
    macrosTopDir='/work/eic3/users/nwickjlb/hpDIRC_ePIC_B_off_sim'
    nEventsPerJob=25000
    nTotalEvents=900000


nArgs = len(sys.argv)
if nArgs != 1:
    print("Usage: python3 job_submit.py")
    sys.exit()


def makeSLURMJob():

    print("Creating SLURM submission files")

    # Open top-level submit file to write command to
    infilephi = open(pars.inputPhiList, "r")
    linephi = infilephi.readline()
    slurmDir = os.path.join(pars.MYDIR, 'slurmJobs')
    os.makedirs(slurmDir, exist_ok=True)
    submitScriptName = os.path.join(slurmDir, 'submitJobs.sh')
    submitScript = open( submitScriptName, "w")
    os.chmod(submitScriptName, 0o744)
    submitScript.write("#!/bin/bash\n")

    # Make output directories
    outputPath     = os.path.join( pars.MYDIR, 'merged_eicrecon_tree' )
    outputLogPath  = os.path.join( pars.MYDIR, 'log')
    os.makedirs(outputPath, exist_ok=True)
    os.makedirs(outputLogPath, exist_ok=True)
    
    # If pars.macrosTopDir is a relative path, then prepend the cwd to it.
    if not pars.macrosTopDir.startswith('/'):
        pars.macrosTopDir = os.path.join(os.getcwd(), pars.macrosTopDir)
                
    # Print input/output info
    print()
    print("Input directory: {}".format(pars.macrosTopDir))
    print(" Input phi list: {}".format(pars.inputPhiList))
    print(" Input theta list: {}".format(pars.inputThetaList))
    print("Output directory: {}".format(outputPath))
    print("   Log directory: {}".format(outputLogPath))
    print()

    # Loop over all input trees and make a submission script that fits the request
    nJobs = 0
    while linephi:
        inputphi = linephi.replace("\n", "")
        infile = open(pars.inputThetaList, "r")        
        line = infile.readline()
        while line:
            inputTheta = line.replace("\n", "")

            nEvents = nJobs*2*pars.nEventsPerJob
            if (pars.nTotalEvents>0) and (nEvents >= pars.nTotalEvents): break

            fileTag = "{0}_theta_mix_pik_{1}_events".format( inputTheta, 2*pars.nEventsPerJob)

            slurmOutputInfo = "{0}/slurm-{1}".format(outputLogPath, fileTag)

            file1 = "{0}/EICReconOut_pi+_{1}evts_theta_{2}deg.root ".format(pars.macrosTopDir, pars.nEventsPerJob, inputTheta)
            file2 = "{0}/EICReconOut_kaon+_{1}evts_theta_{2}deg.root ".format(pars.macrosTopDir, pars.nEventsPerJob, inputTheta)
            outputFile = outputPath + "/eicrecon_{}.root ".format(fileTag)

            slurmFileName = "slurmJob_{}.job".format(fileTag)
            slurmFile = open("{0}/{1}".format(slurmDir, slurmFileName), "w")				
            slurmFile.write("#!/bin/bash\n")
            slurmFile.write("#\n")
            slurmFile.write("#SBATCH --account=eic\n")
            slurmFile.write("#SBATCH --nodes=1\n")
            slurmFile.write("#SBATCH --ntasks=1\n")
            slurmFile.write("#SBATCH --mem-per-cpu=100\n")
            slurmFile.write("#SBATCH --job-name=slurm-{0}\n".format(fileTag))
            slurmFile.write("#SBATCH --time=0:30:00\n")
            slurmFile.write("#SBATCH --gres=disk:500\n")
            slurmFile.write("#SBATCH --output=" + slurmOutputInfo + ".out\n")
            slurmFile.write("#SBATCH --error=" + slurmOutputInfo + ".err\n")
            slurmFile.write("#SBATCH --chdir=/u/scratch/" + getpass.getuser() + "\n")
            slurmFile.write("#\n")
            slurmFile.write("\n")
            slurmFile.write("printf \"Start time: \"; /bin/date\n")
            slurmFile.write("printf \"host: \"; /bin/hostname -A\n")
            slurmFile.write("printf \"user: \"; /usr/bin/id\n")
            slurmFile.write("printf \"cwd: \"; /bin/pwd\n")
            slurmFile.write("printf \"os: \"; /bin/lsb_release -a\n")
            slurmFile.write("echo \n")
            slurmFile.write("echo \"----------------------------------------------\"\n")
            slurmFile.write("echo \n")
            slurmFile.write("\n")
            slurmFile.write("\n")
            slurmFile.write("# Create and cd to dedicated working directory\n")
            slurmFile.write("mkdir -p workdir_${SLURM_JOBID}\n")
            slurmFile.write("cd workdir_${SLURM_JOBID}\n")
            slurmFile.write("printf \"cwd: \"; /bin/pwd\n")
            slurmFile.write("\n")
            slurmFile.write("hadd " + outputFile + file1 + file2 + "\n")

            slurmFile.write("echo \n")
            slurmFile.write("echo \"----------------------------------------------\"\n")
            slurmFile.write("rm -rf workdir_${SLURM_JOBID}\n")
            slurmFile.write("printf \"End time: \"; /bin/date\n")
            slurmFile.close()

            submitScript.write("sbatch {}\n".format(os.path.join(slurmDir, slurmFileName)))

            nJobs += 1
            if (pars.nTotalEvents>0) and (nEvents >= pars.nTotalEvents):
                submitScript.close()
                break
            line = infile.readline()
        infile.close()
        linephi = infilephi.readline()
    infilephi.close()

    print("\nSLURM submission files have been written to:\n{}".format(slurmDir))
    print("\nThis setup will submit {} jobs".format(nJobs))
    print("\nYou can submit your jobs with the script:\n{}".format(submitScriptName))


makeSLURMJob()
