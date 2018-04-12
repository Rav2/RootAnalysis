
#ifndef RootAnalysis_svfitAnalyzer_H
#define RootAnalysis_svfitAnalyzer_H

#include <string>
#include <vector>
#include <map>

#include "ObjectMessenger.h"
#include "EventProxyBase.h"
#include "EventProxyHTT.h"

#include "strbitset.h"
#include "TDirectory.h"

//ROOT includes
#include "TTree.h"
#include "TList.h"

#include "Analyzer.h"
#include "ChannelSpecifics.h"
#include "AnalysisEnums.h"

#include "TauAnalysis/ClassicSVfit/interface/ClassicSVfit.h"

class svfitHistograms;

class TH1F;
class TH2F;
class TH3F;
class TLorentzVector;
class TF1;

class svfitAnalyzer: public Analyzer{

  friend class ChannelSpecifics;
  friend class MuTauSpecifics;
  friend class TauTauSpecifics;
  friend class MuMuSpecifics;

 public:

  svfitAnalyzer(const std::string & aName, const std::string & aDecayMode = "None");

  virtual ~svfitAnalyzer();

  ///Initialize the analyzer
  virtual void initialize(TDirectory* aDir,
			  pat::strbitset *aSelections);

  virtual bool analyze(const EventProxyBase& iEvent);

  virtual bool analyze(const EventProxyBase& iEvent, ObjectMessenger *aMessenger){return analyze(iEvent); }

  virtual void finalize();

  virtual void clear(){;};

  virtual void addBranch(TTree *);

  Analyzer* clone() const;

  bool filter() const{ return filterEvent_;};

  void setAnalysisObjects(const EventProxyHTT & myEventProxy);

  ///Check it the event passes given category selections.
  bool passCategory(unsigned int iCategory);

  ///Return human readable sample name (Data, WJets, etc).
  std::string getSampleName(const EventProxyHTT & myEventProxy);

  ///Return human readable sample name (Data, WJets, etc).
  ///Make the methos static, so other modules can use it.
  ///Method used when sample coding in TTree is not present.
  ///In this case a ROOT file name is used to decode the sample type.
  std::string getSampleNameFromFileName(const EventProxyHTT & myEventProxy);

  ///Return sample name for DY. Name encoded jet bin, and decay mode.
  std::string getDYSampleName(const EventProxyHTT & myEventProxy);

  //Return name sample name suffix for different particles matched to reconstructed tau
  std::string getMatchingName(const EventProxyHTT & myEventProxy);

  ///Fill histograms for all control plots.
  ///Histogram names will end with hNameSuffix
  void fillControlHistos(const std::string & hNameSuffix);

  ///Get jets separated by deltaR from tau an muon.
  std::vector<HTTParticle> getSeparatedJets(const EventProxyHTT & myEventProxy,
    float deltaR);

 protected:

  pat::strbitset *mySelections_;

  ///Types of the selection flow
  std::vector<std::string> selectionFlavours_;

 private:

  void setHistos(svfitHistograms *histos) { myHistos_ = histos;};

  TLorentzVector computeSvFit();
  TLorentzVector runSVFitAlgo(const std::vector<classic_svFit::MeasuredTauLepton> & measuredTauLeptons,
                              const TVector2 &aMET, const TMatrixD &covMET);

  TLorentzVector runFastSVFitAlgo(const TLorentzVector & leg1P4,
                                  const TLorentzVector & leg2P4,
                                  const TLorentzVector &metP4,
                                  const TMatrixD &covMET);

  double EvalMET_TF(const TLorentzVector & metP4,
                                 const TLorentzVector & nuP4,
                                 const TMatrixD& covMET);

  ///Parts of code specific to give decay channel.
  ///In particular category and object selection.
  ChannelSpecifics *myChannelSpecifics;

  ///Histograms storage.
  svfitHistograms *myHistos_;

  ///ROOT file containing current TTree
  TFile *ntupleFile_;

  //should this HTTAnalyzer be able to filter events
  bool filterEvent_;

  ///Map from file name to sample name.
  std::map<std::string, std::string> fileName2sampleName;

  ///Reconstructed objects selected for given event.
  HTTEvent aEvent;
  HTTPair aPair;
  std::string sampleName;

  HTTParticle aLeg2, aLeg1, aMET;
  HTTParticle aGenLeg1, aGenLeg2;
  HTTParticle aJet1, aJet2, aBJet1;
  std::vector<HTTParticle> aSeparatedJets;
  int nJets30;
  int nJetsInGap30;

  std::vector<bool> categoryDecisions;
  unsigned int myNumberOfCategories;

  ClassicSVfit svFitAlgo;

  TF1 *fLikelihood;

};

#endif
