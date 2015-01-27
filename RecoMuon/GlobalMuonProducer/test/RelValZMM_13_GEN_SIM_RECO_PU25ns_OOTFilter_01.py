# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions auto:run2_mc --magField 38T_PostLS1 -s RAW2DIGI,L1Reco,RECO,EI,VALIDATION,DQM -n 10 --eventcontent RECOSIM,DQM --datatier GEN-SIM-RECO,DQMIO --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('CommonTools.ParticleFlow.EITopPAG_cff')
process.load('Configuration.StandardSequences.Validation_cff')
process.load('DQMOffline.Configuration.DQMOfflineMC_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.load("STASkim.ProducerTest.standAloneTiming_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(150)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
                            fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_7_3_0_pre1/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_PRE_LS172_V15-v1/00000/08595E23-A65E-E411-B2BF-02163E00E8C5.root')

                            )



process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('step3 nevts:10'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('step3_RAW2DIGI_L1Reco_RECO_EI_VALIDATION_DQM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    )
)

#process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
#   splitLevel = cms.untracked.int32(0),
#  outputCommands = process.DQMEventContent.outputCommands,
# fileName = cms.untracked.string('step3_RAW2DIGI_L1Reco_RECO_EI_VALIDATION_DQM_inDQM.root'),
    #dataset = cms.untracked.PSet(
    #   filterName = cms.untracked.string(''),
    #  dataTier = cms.untracked.string('DQMIO')
    #)
#)

# Additional output definition
#process.TFileService = cms.Service("TFileService",
 #                                  fileName = cms.string('SingleMu_Tree_noRecoOutput.root'),
  #                                 closeFileFast = cms.untracked.bool(False)
   #                                )
# Other statements
process.mix.playback = True
process.mix.digitizers = cms.PSet()
for a in process.aliases: delattr(process, a)
process.RandomNumberGeneratorService.restoreStateLabel=cms.untracked.string("randomEngineStateProducer")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
#process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
#process.eventinterpretaion_step = cms.Path(process.EIsequence)
#process.prevalidation_step = cms.Path(process.prevalidation)
#process.dqmoffline_step = cms.Path(process.DQMOffline)
#process.validation_step = cms.EndPath(process.validation)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)
#process.DQMoutput_step = cms.EndPath(process.DQMoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.eventinterpretaion_step,process.prevalidation_step,process.validation_step,process.dqmoffline_step,process.RECOSIMoutput_step,process.DQMoutput_step)

process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.RECOSIMoutput_step)
#process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)

# Automatic addition of the customisation function from SimGeneral.MixingModule.fullMixCustomize_cff
from SimGeneral.MixingModule.fullMixCustomize_cff import setCrossingFrameOn 

#call to customisation function setCrossingFrameOn imported from SimGeneral.MixingModule.fullMixCustomize_cff
process = setCrossingFrameOn(process)

# End of customisation functions
#f = file('outfile_MuPt10', 'w')
#f.write(process.dumpPython())
#f.close() 
