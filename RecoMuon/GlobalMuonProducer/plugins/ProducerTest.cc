// -*- C++ -*-
//
// Package:    STASkim/ProducerTest
// Class:      ProducerTest
// 
/**\class ProducerTest ProducerTest.cc STASkim/ProducerTest/plugins/ProducerTest.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Federica Primavera
//         Created:  Thu, 27 Nov 2014 13:58:57 GMT
//
//


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
#include "RecoMuon/MuonIdentification/interface/MuonTimingFiller.h"

#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
//#include "RecoTracker/TrackProducer/interface/TrackProducerBase.h"

//
// class declaration
//

class ProducerTest : public edm::EDProducer {
public:
  explicit ProducerTest(const edm::ParameterSet&);
  ~ProducerTest();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
private:
  virtual void beginJob() override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  edm::InputTag labelStaMuonCollection_; 
  edm::EDGetTokenT<reco::TrackCollection> staMuonToken;
  edm::InputTag labelRecoMuonCollection_; 
  edm::EDGetTokenT<reco::MuonCollection> recoMuonToken;

   
}; // fine del .h


// constructors
ProducerTest::ProducerTest(const edm::ParameterSet& iConfig){

  labelStaMuonCollection_  = iConfig.getParameter < edm::InputTag > ("staMuons");
  labelRecoMuonCollection_ = iConfig.getParameter < edm::InputTag > ("recoMuons");
  
  //register product
  produces<reco::TrackCollection>();
  produces<reco::TrackExtraCollection>(); 
  produces<TrackingRecHitCollection>(); 
  // consumes
  staMuonToken = consumes<reco::TrackCollection>(labelStaMuonCollection_);
  recoMuonToken = consumes<reco::MuonCollection>(labelRecoMuonCollection_);

}

// and destructor
ProducerTest::~ProducerTest()
{}


// ------------ method called to produce the data  ------------
void
ProducerTest::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  using namespace edm;
  using namespace std;
  using namespace reco;
  
  Handle <reco::TrackCollection> staMuons;
  iEvent.getByToken(staMuonToken, staMuons);
  
  Handle <reco::MuonCollection> recoMuons;
  iEvent.getByToken(recoMuonToken, recoMuons);
  
  auto_ptr<reco::TrackCollection>      pOutputMuonCollection(new reco::TrackCollection);
  auto_ptr<reco::TrackExtraCollection> pOutputExtraMuonCollection(new reco::TrackExtraCollection() );  
  auto_ptr<TrackingRecHitCollection>   pOutputMuonHits( new TrackingRecHitCollection() );

  reco::TrackRefProd      rTracks      = iEvent.getRefBeforePut<reco::TrackCollection>();
  reco::TrackExtraRefProd rTrackExtras = iEvent.getRefBeforePut<reco::TrackExtraCollection>();
  TrackingRecHitRefProd   rHits        = iEvent.getRefBeforePut<TrackingRecHitCollection>();
     
  edm::Ref<reco::TrackExtraCollection>::key_type hidx = 0;
  edm::Ref<reco::TrackExtraCollection>::key_type idx = 0;
  
  //*******************************************************************************************

  for(reco::MuonCollection::size_type m(0); m<recoMuons->size(); ++m){    
    const reco::Muon* muon = &recoMuons->at(m);
    if(muon->isStandAloneMuon() && fabs(muon->time().timeAtIpInOut) < 25+fabs(muon->time().timeAtIpInOutErr) ){
      
      for(reco::TrackCollection::size_type s(0); s<staMuons->size(); ++s){    
	//	const reco::Track* sta = &staMuons->at(s);
	reco::Track sta = staMuons->at(s);

	//std::cout << " sta " << sta.pt() << " muon outertrack " << muon->outerTrack()->pt() << std::endl;
	
	if(sta.pt() == muon->outerTrack()->pt()){
	  
	  // pointer to old track:
	  reco::Track *newTrk = new reco::Track(sta);
	  newTrk->setExtra( reco::TrackExtraRef( rTrackExtras, idx++ ) );
	  PropagationDirection seedDir = sta.seedDirection();
	  // new copy of track Extras
	  reco::TrackExtra * newExtra = new reco::TrackExtra( sta.outerPosition(), sta.outerMomentum(), 
							      sta.outerOk(), sta.innerPosition(), 
							      sta.innerMomentum(), sta.innerOk(),
							      sta.outerStateCovariance(), sta.outerDetId(),
							      sta.innerStateCovariance(), sta.innerDetId() , seedDir ) ;
	  
	  // new copy of the silicon hits; add hit refs to Extra and hits to hit collection
	  
	  //      edm::LogVerbatim("MuonTrackProducer")<<"\n printing initial hit_pattern";
	  //      trk->hitPattern().print();
	  
	  for (trackingRecHit_iterator iHit = sta.recHitsBegin(); iHit != sta.recHitsEnd(); iHit++) {
	    TrackingRecHit* hit = (*iHit)->clone();
	    pOutputMuonHits->push_back( hit );
	    newExtra->add( TrackingRecHitRef( rHits, hidx++ ) );
	  }
	 
	  pOutputMuonCollection->push_back(*newTrk);
	  pOutputExtraMuonCollection->push_back(*newExtra);
	}
      }
    }
  }
  iEvent.put(pOutputMuonCollection);
  iEvent.put(pOutputExtraMuonCollection);
  iEvent.put(pOutputMuonHits);
}     

// ------------ method called once each job just before starting event loop  ------------
void 
ProducerTest::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ProducerTest::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
ProducerTest::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
ProducerTest::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
ProducerTest::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
ProducerTest::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ProducerTest::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProducerTest);
