#ifndef ProducerTest_Time_h
#define ProducerTest_Time_h
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtra.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtraMap.h"
#include "RecoMuon/MuonIdentification/interface/DTTimingExtractor.h"
#include "RecoMuon/MuonIdentification/interface/CSCTimingExtractor.h"

#include "RecoMuon/MuonIdentification/interface/TimeMeasurementSequence.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

namespace edm {
  class ParameterSet;
  class EventSetup;
  class InputTag;
  class CSCTimingExtractor;
  class DTTimingExtractor;
};
namespace std{};
namespace reco{};
//
// class declaration
//

class Time{
 public:
  Time(const edm::ParameterSet&, edm::ConsumesCollector&& iC);
  ~Time();
  
  void fillTime(const reco::TrackRef& muon, reco::MuonTimeExtra& dtTime, 
		 reco::MuonTimeExtra& cscTime, reco::MuonTimeExtra& combinedTime, 
		 edm::Event& iEvent, const edm::EventSetup& iSetup );
  
 private:
  void fillTimeFromMeasurements( const TimeMeasurementSequence& tmSeq, reco::MuonTimeExtra &muTime );
  void rawFit(double &a, double &da, double &b, double &db, const std::vector<double>& hitsx, const std::vector<double>& hitsy);
  void combineTMSequences( const reco::TrackRef& muon, const TimeMeasurementSequence& dtSeq,const TimeMeasurementSequence& cscSeq, TimeMeasurementSequence &cmbSeq);
  
  DTTimingExtractor* theDTTimingExtractor_;
  CSCTimingExtractor* theCSCTimingExtractor_;
 
  bool useDT_, useCSC_;
   
}; 

#endif
