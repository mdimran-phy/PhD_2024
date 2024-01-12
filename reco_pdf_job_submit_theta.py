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
    macrosTopDir='/work/eic3/users/nwickjlb/time_imaging_hpDIRC_ePIC_B_off/merged_eicrecon_tree'
    nEventsPerJob=50000
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
    slurmDir = os.path.join(pars.MYDIR, 'reco_pdf_slurmJobs')
    os.makedirs(slurmDir, exist_ok=True)
    submitScriptName = os.path.join(slurmDir, 'submitJobs.sh')
    submitScript = open( submitScriptName, "w")
    os.chmod(submitScriptName, 0o744)
    submitScript.write("#!/bin/bash\n")

    # Make output directories
    outputPath     = os.path.join( pars.MYDIR, 'reco_plots' )
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
    print(" Input phi list: {}".format(pars.inputPhiList))
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

            nEvents = nJobs*pars.nEventsPerJob
            if (pars.nTotalEvents>0) and (nEvents >= pars.nTotalEvents): break
                        
            fileTag = "theta_{0}deg".format(inputTheta)

            slurmOutputInfo = "{0}/slurm-{1}".format(outputLogPath, fileTag)

            recomacro = "/work/eic3/users/nwickjlb/epic_hpDIRC_reco/recoPdf_epic.cpp"
            hpmacro = "/work/eic3/users/nwickjlb/epic_hpDIRC_reco/draw_hp.C"
            fileinput = "{0}/eicrecon_{1}_theta_mix_pik_{2}_events.root".format(pars.macrosTopDir, inputTheta, pars.nEventsPerJob)
            filepdf = "{0}/eicrecon_{1}_theta_mix_pik_{2}_events.pdf.root".format(pars.macrosTopDir, inputTheta, pars.nEventsPerJob)

            #nph_fit_outputFile = outputPath + "/nph_fit_{}.png ".format(fileTag)
            nph_outputFile = outputPath + "/nph_{}.png ".format(fileTag) 
            sep_outputFile = outputPath + "/sep_{}.png ".format(fileTag)
            tim_outputFile = outputPath + "/tim_{}.png ".format(fileTag)
            hp_outputFile = outputPath + "/hp_{}.png ".format(fileTag)

            #envFile = "/group/halld/Software/build_scripts/gluex_env_jlab.sh"
            envFile = "/home/nwickjlb/EIC_env.sh"

            slurmFileName = "slurmJob_{}.job".format(fileTag)
            slurmFile = open("{0}/{1}".format(slurmDir, slurmFileName), "w")				
            slurmFile.write("#!/bin/bash\n")
            slurmFile.write("#\n")
            slurmFile.write("#SBATCH --account=eic\n")
            slurmFile.write("#SBATCH --nodes=1\n")
            slurmFile.write("#SBATCH --ntasks=1\n")
            slurmFile.write("#SBATCH --mem-per-cpu=1000\n")
            slurmFile.write("#SBATCH --job-name=slurm-{0}\n".format(fileTag))
            slurmFile.write("#SBATCH --time=0:15:00\n")
            slurmFile.write("#SBATCH --gres=disk:1000\n")
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

            slurmFile.write("source " + envFile + "\n")
            slurmFile.write("root -q -b \'{0}(\"{1}\", \"{2}\",{3})\'\n".format(recomacro, fileinput, filepdf, inputTheta))
            slurmFile.write("root -q -b \'{0}(\"{1}\")\'\n".format(hpmacro, fileinput))
            
            #slurmFile.write("mv -v nph_fit.png " + nph_fit_outputFile + "\n")
            slurmFile.write("mv -v nph_pik_6GeV_epic.png " + nph_outputFile + "\n")
            slurmFile.write("mv -v sep_pik_6GeV_epic.png " + sep_outputFile + "\n")
            slurmFile.write("mv -v tim_pik_6GeV_epic.png " + tim_outputFile + "\n")
            #slurmFile.write("mv -v nph_epi_1.2GeV_epic.png " + nph_outputFile + "\n")
            #slurmFile.write("mv -v sep_epi_1.2GeV_epic.png " + sep_outputFile + "\n")
            #slurmFile.write("mv -v tim_epi_1.2GeV_epic.png " + tim_outputFile + "\n")
            slurmFile.write("mv -v hp.png " + hp_outputFile + "\n")

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
