//
// Original Author:  Federica Primavera
//         Created:  Thu, 27 Nov 2014 13:58:57 GMT
//
//

#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/Muon.h" 
#include "DataFormats/MuonReco/interface/MuonFwd.h" 
#include "DataFormats/MuonReco/interface/MuonTimeExtra.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtraMap.h"

#include "RecoMuon/GlobalMuonProducer/plugins/StandAloneTimingProducer.h"
#include "RecoMuon/MuonIdentification/interface/TimeMeasurementSequence.h"

namespace edm {
  class ParameterSet;
  class EventSetup;
  class InputTag;
  class Time;
}

// constructors
StandAloneTimingProducer::StandAloneTimingProducer(const edm::ParameterSet& iConfig){

  produces<reco::MuonTimeExtraMap>("combined");
  produces<reco::MuonTimeExtraMap>("dt");
  produces<reco::MuonTimeExtraMap>("csc");
  
  m_muonCollection = iConfig.getParameter<edm::InputTag>("MuonCollection");
  muonToken_ = consumes<reco::TrackCollection>(m_muonCollection);
  // Load parameters for the TimingFiller
  edm::ParameterSet fillerParameters = iConfig.getParameter<edm::ParameterSet>("TimingFillerParameters");
  theTimingFiller_ = new Time(fillerParameters,consumesCollector());
}

// and destructor
StandAloneTimingProducer::~StandAloneTimingProducer(){
  if (theTimingFiller_) delete theTimingFiller_;
}


// ------------ method called to produce the data  ------------
void
StandAloneTimingProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  std::auto_ptr<reco::MuonTimeExtraMap> muonTimeMap(new reco::MuonTimeExtraMap());
  reco::MuonTimeExtraMap::Filler filler(*muonTimeMap);
  std::auto_ptr<reco::MuonTimeExtraMap> muonTimeMapDT(new reco::MuonTimeExtraMap());
  reco::MuonTimeExtraMap::Filler fillerDT(*muonTimeMapDT);
  std::auto_ptr<reco::MuonTimeExtraMap> muonTimeMapCSC(new reco::MuonTimeExtraMap());
  reco::MuonTimeExtraMap::Filler fillerCSC(*muonTimeMapCSC);
  
  edm::Handle<reco::TrackCollection> muons; 
  iEvent.getByToken(muonToken_, muons);
  
  unsigned int nMuons = muons->size();
  

  std::vector<reco::MuonTimeExtra> dtTimeColl(nMuons);
  std::vector<reco::MuonTimeExtra> cscTimeColl(nMuons);
  std::vector<reco::MuonTimeExtra> combinedTimeColl(nMuons);
  
  for ( unsigned int i=0; i<nMuons; ++i ) {
    reco::MuonTimeExtra dtTime;
    reco::MuonTimeExtra cscTime;
    reco::MuonTimeExtra combinedTime;
    
    const reco::TrackRef muonref(muons,i);  
       
    theTimingFiller_->fillTime(muonref, dtTime, cscTime, combinedTime, iEvent, iSetup);

    dtTimeColl[i] = dtTime;
    cscTimeColl[i] = cscTime;
    combinedTimeColl[i] = combinedTime;
  }
  
  //muons 
  filler.insert(muons, combinedTimeColl.begin(), combinedTimeColl.end());
  filler.fill();
  fillerDT.insert(muons, dtTimeColl.begin(), dtTimeColl.end());
  fillerDT.fill();
  fillerCSC.insert(muons, cscTimeColl.begin(), cscTimeColl.end());
  fillerCSC.fill();

  iEvent.put(muonTimeMap,"combined");
  iEvent.put(muonTimeMapDT,"dt");
  iEvent.put(muonTimeMapCSC,"csc");
}     

DEFINE_FWK_MODULE(StandAloneTimingProducer);

