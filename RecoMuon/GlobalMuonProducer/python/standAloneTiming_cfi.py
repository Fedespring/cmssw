import FWCore.ParameterSet.Config as cms

#from RecoMuon.MuonIdentification.standAloneTimingFiller_cfi import *
from RecoMuon.MuonIdentification.DTTimingExtractor_cfi import *
from RecoMuon.MuonIdentification.CSCTimingExtractor_cfi import *


staUpdAtVtx = cms.EDProducer(
    'StandAloneTimingProducer',
    #TimingFillerBlock,
    TimingFillerParameters = cms.PSet(DTTimingExtractorBlock,
                                      CSCTimingExtractorBlock,
                                      # On/off switches for combined time measurement
                                      UseDT  = cms.bool(True),
                                      UseCSC = cms.bool(True)
                                      ),
    MuonCollection = cms.InputTag("standAloneMuons","UpdatedAtVtx")
    )

staRegular = staUpdAtVtx.clone(
    MuonCollection = cms.InputTag("standAloneMuons"))

staSETUpdAtVtx = staUpdAtVtx.clone(
    MuonCollection = cms.InputTag("standAloneSETMuons"))

staSETRegular = staUpdAtVtx.clone(
    MuonCollection = cms.InputTag("standAloneSETMuons"))

