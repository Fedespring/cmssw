#ifndef ProducerTest_StanAloneTimingProducer_h
#define ProducerTest_StanAloneTimingProducer_h

#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/MuonTimeExtra.h"

#include "RecoMuon/GlobalMuonProducer/plugins/Time.h"
 
class StandAloneTimingProducer : public edm::stream::EDProducer<> {
public:
  explicit StandAloneTimingProducer(const edm::ParameterSet&);
  ~StandAloneTimingProducer();
 
private:
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
   
  // ----------member data ---------------------------
  edm::InputTag m_muonCollection;
  edm::EDGetTokenT<reco::TrackCollection> muonToken_;
  
  Time* theTimingFiller_;
  
};
 
#endif
