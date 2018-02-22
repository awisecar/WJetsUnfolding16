Quick start
============

Set up a CMSSW environment:

```
   $ cmsrel CMSSW_7_4_11
   $ cd CMSSW_7_4_11
   $ cmsenv
   $ cd ..
```

Clone the project to your working directory:

    git clone https://github.com/awisecar/WJetsUnfolding16.git

Build the code:

```
    $ cd WJets13TeVUnfolding
    $ make clean
    $ make
```

Configuration is stored in vjet.cfg. A commented example can be found in example.cfg
vjets.cfg is used for customizing the parameters, the one stored on git should be fine.
(Unless you have already your own configuration file, copy it from the example.cfg)


Run the unfolding
=================

Before running the unfolding you need to produce the histogran for all the variations of the systematic sources. 
The directory HistoFiles containing these histogram root files should be present.


```
./runUnfoldingZJets lepSel=SMu variable=ZNGoodJetsFull_Zexc
```

The unfolded result can then be found in the UnfoldedFiles directory and the control plots in the UnfoldingCheck.




Code organization
=================

The directory contains several runXXX.cc files, which have been compiles in executable called runXXX. Each of these executable runs the code implemented in the class XXX with the corresponding name and defined in Includes/XXX.h and Sources/XXX.cc.  The list of runXXX applications is provided below.

