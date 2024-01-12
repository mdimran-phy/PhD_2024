import math
import sys, os, getpass, shutil
from os import environ
from ROOT import TFile, TObjString

# MYDIR          : Output directory 
# inputThetaList  : File containing list of theta values 
# macrosTopDir   : Directory for input files
# dircmacrosTopDir : Directory containing eicdirc code
# nEventsPerJob  : Jobs will be broken up into this many events per job
# nTotalEvents   : Maximum number of events to process when summing all jobs.

class pars:
    MYDIR='/w/halld-scshelf2101/halld3/home/ihossain/ti_reconstruction_tc_0.25_tr_0.1'
    inputThetaList='theta_values.list'
    macrosTopDir='/w/halld-scshelf2101/halld3/home/ihossain/t_output/hits_root_files'
    dircmacrosTopDir='/w/halld-scshelf2101/halld3/home/ihossain/eicdirc/build'
    nEventsPerJob=50000
    nTotalEvents=1250000

nArgs = len(sys.argv)
if nArgs != 1:
    print("Usage: python3 geo_reco_job_submit.py")
    sys.exit()


def makeSLURMJob():

    print("Creating SLURM submission files")

    # Open top-level submit file to write command to
    infile = open(pars.inputThetaList, "r")
    line = infile.readline()
    slurmDir = os.path.join(pars.MYDIR, 'geo_reco_slurmJobs')
    os.makedirs(slurmDir, exist_ok=True)
    submitScriptName = os.path.join(slurmDir, 'submitJobs.sh')
    submitScript = open( submitScriptName, "w")
    os.chmod(submitScriptName, 0o744)
    submitScript.write("#!/bin/bash\n")

    # Make output directories
    outputPath     = os.path.join( pars.MYDIR, 'reco_files' )
    outputLogPath  = os.path.join( pars.MYDIR, 'reco_log')

    os.makedirs(outputPath, exist_ok=True)
    os.makedirs(outputLogPath, exist_ok=True)
    
    # If pars.macrosTopDir is a relative path, then prepend the cwd to it.
    if not pars.macrosTopDir.startswith('/'):
        pars.macrosTopDir = os.path.join(os.getcwd(), pars.macrosTopDir)
                
    # Print input/output info
    print()
    print("Input directory: {}".format(pars.macrosTopDir))
    print(" Input theta list: {}".format(pars.inputThetaList))
    print("Output directory: {}".format(outputPath))
    print("   Log directory: {}".format(outputLogPath))
    print()

    # Loop over all input theta values and make a submission script that fits the request
    nJobs = 0
    while line:
       inputTheta = line.replace("\n", "")

       nEvents = nJobs*pars.nEventsPerJob
       if (pars.nTotalEvents>0) and (nEvents >= pars.nTotalEvents): break

       inputfile = "{0}/hits_{1}_deg_mix_pik_{2}_events.root ".format(pars.macrosTopDir, inputTheta, pars.nEventsPerJob)
       pdffile = "{0}/hits_{1}_deg_mix_pik_{2}_events.pdf.root ".format(pars.macrosTopDir, inputTheta, pars.nEventsPerJob)
       lutfile = "/w/halld-scshelf2101/halld3/home/ihossain/eicdirc/data/lut.avr.root "
       fileTag = "{0}_deg".format(inputTheta)

       slurmOutputInfo = "{0}/slurm-{1}".format(outputLogPath, fileTag)

       outputFile = outputPath + "/reco_{}.root ".format(fileTag)

       reco_command1 = "{0}/eicdirc ".format(pars.dircmacrosTopDir) + "-i " + inputfile + "-u " + lutfile + "-r 2 -timecut 0.25 -v 2 -e 0 -trackingres 0.0005 -x mix_pik -timeres 0.1"

       reco_command2 = "{0}/eicdirc ".format(pars.dircmacrosTopDir) + "-i " + inputfile + "-u " + lutfile + "-r 4 -timecut 0.25 -v 2 -e 0 -trackingres 0.0005 -x mix_pik -timeres 0.1"
 
       reco_command3 = "{0}/eicdirc ".format(pars.dircmacrosTopDir) + "-i " + inputfile + "-u " + lutfile + "-pdf " + pdffile + "-r 2 -timecut 0.25 -v 2 -e 0 -trackingres 0.0005 -x mix_pik -timeres 0.1"

       env_command = "source /site/12gev_phys/softenv.sh 2.6"

       slurmFileName = "slurmJob_{}.job".format(fileTag)
       slurmFile = open("{0}/{1}".format(slurmDir, slurmFileName), "w")				
       slurmFile.write("#!/bin/bash\n")
       slurmFile.write("#\n")
       slurmFile.write("#SBATCH --account=halld\n")
       slurmFile.write("#SBATCH --nodes=1\n")
       slurmFile.write("#SBATCH --ntasks=1\n")
       slurmFile.write("#SBATCH --mem-per-cpu=2000\n")
       slurmFile.write("#SBATCH --job-name=slurm-{0}\n".format(fileTag))
       slurmFile.write("#SBATCH --time=0:10:00\n")
       slurmFile.write("#SBATCH --gres=disk:2000\n")
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
       slurmFile.write(env_command + "\n")
       slurmFile.write(reco_command1 + "\n")
       slurmFile.write(reco_command2 + "\n")
       slurmFile.write(reco_command3 + "\n")
       slurmFile.write("mv reco.root " + outputFile + "\n")
       slurmFile.write("")

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

    print("\nSLURM submission files have been written to:\n{}".format(slurmDir))
    print("\nThis setup will submit {} jobs".format(nJobs))
    print("\nYou can submit your jobs with the script:\n{}".format(submitScriptName))


makeSLURMJob()
